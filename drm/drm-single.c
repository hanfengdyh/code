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
    memset(bo->vaddr, 0x00, bo->size); // 黑色背景

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

    // 黑色背景
    for (int i = 0; i < buf.width * buf.height; i++) {
        pixel[i] = 0x00000000;
    }

    // 横向粗线
    int line_y_start = buf.height / 2 - 10; // 屏幕中间，线宽 20 像素
    int line_y_end = line_y_start + 20;
    for (int y = line_y_start; y < line_y_end; y++) {
        for (int x = 0; x < buf.width; x++) {
            pixel[y * buf.width + x] = 0x00FFFF00; // 黄色
        }
    }

    printf("Press Enter to exit\n");
    getchar();

    modeset_destroy_fb(fd, &buf);
    drmModeFreeConnector(conn);
    drmModeFreeResources(res);
    close(fd);
    return 0;
}
