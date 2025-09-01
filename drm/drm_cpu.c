#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

struct buffer_object {
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint32_t handle;
    uint32_t size;
    uint8_t *vaddr;
    uint32_t fb_id;
};

struct buffer_object buf;

static int modeset_create_fb(int fd, struct buffer_object *bo)
{
    struct drm_mode_create_dumb create = {};
    struct drm_mode_map_dumb map = {};

    create.width = bo->width;
    create.height = bo->height;
    create.bpp = 32;
    drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &create);

    bo->pitch = create.pitch;
    bo->size = create.size;
    bo->handle = create.handle;
    drmModeAddFB(fd, bo->width, bo->height, 24, 32, bo->pitch, bo->handle, &bo->fb_id);

    map.handle = create.handle;
    drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &map);

    bo->vaddr = mmap(0, create.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, map.offset);
    memset(bo->vaddr, 0x00, bo->size); // 初始化为黑色背景

    return 0;
}

static void modeset_destroy_fb(int fd, struct buffer_object *bo)
{
    struct drm_mode_destroy_dumb destroy = {};
    drmModeRmFB(fd, bo->fb_id);
    munmap(bo->vaddr, bo->size);
    destroy.handle = bo->handle;
    drmIoctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, &destroy);
}

static uint32_t random_color()
{
    // 随机生成 RGB 颜色
    uint8_t r = rand() % 256;
    uint8_t g = rand() % 256;
    uint8_t b = rand() % 256;
    return (r << 16) | (g << 8) | b;
}

int main(int argc, char **argv)
{
    int fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        perror("open /dev/dri/card0");
        return -1;
    }

    drmModeRes *res = drmModeGetResources(fd);
    uint32_t crtc_id = res->crtcs[0];
    uint32_t conn_id = res->connectors[0];

    drmModeConnector *conn = drmModeGetConnector(fd, conn_id);
    buf.width = conn->modes[0].hdisplay;
    buf.height = conn->modes[0].vdisplay;

    modeset_create_fb(fd, &buf);
    drmModeSetCrtc(fd, crtc_id, buf.fb_id, 0, 0, &conn_id, 1, &conn->modes[0]);

    uint32_t *pixel = (uint32_t *)buf.vaddr;

    printf("Press Enter to exit\n");

    // 持续刷新帧，直到按下 Enter 键
    while (1) {
        uint32_t color = random_color(); // 获取随机颜色

        // 填充屏幕为纯色
        for (int i = 0; i < buf.width * buf.height; i++) {
            pixel[i] = color;
        }

        // 刷新显示，每帧持续约 40ms，确保帧率为 24fps
        usleep(1000000 / 24); // 约 41.67ms 每帧

        // 检查是否按下 Enter 键退出
        if (getchar() == '\n') {
            break;
        }
    }

    modeset_destroy_fb(fd, &buf);
    drmModeFreeConnector(conn);
    drmModeFreeResources(res);
    close(fd);
    return 0;
}
