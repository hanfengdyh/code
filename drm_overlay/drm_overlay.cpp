#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <drm_fourcc.h>
#include <png.h>
#include <string.h>
#include <errno.h>

struct drm_fb {
    uint32_t fb_id;
    uint32_t handle;
    uint32_t pitch;
    uint32_t size;
    uint32_t *map;
};

// 创建 dumb buffer
int create_fb(int drm_fd, int width, int height, drm_fb *fb)
{
    struct drm_mode_create_dumb creq = {};
    creq.width = width;
    creq.height = height;
    creq.bpp = 32;

    if (ioctl(drm_fd, DRM_IOCTL_MODE_CREATE_DUMB, &creq) < 0) {
        perror("DRM_IOCTL_MODE_CREATE_DUMB");
        return -1;
    }

    fb->handle = creq.handle;
    fb->pitch = creq.pitch;
    fb->size = creq.size;

    if (drmModeAddFB(drm_fd, width, height, 24, 32, fb->pitch,
                     fb->handle, &fb->fb_id)) {
        perror("drmModeAddFB");
        return -1;
    }

    struct drm_mode_map_dumb mreq = {};
    mreq.handle = fb->handle;
    if (ioctl(drm_fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq)) {
        perror("DRM_IOCTL_MODE_MAP_DUMB");
        return -1;
    }

    fb->map = (uint32_t*) mmap(0, fb->size, PROT_READ | PROT_WRITE,
                               MAP_SHARED, drm_fd, mreq.offset);
    if (fb->map == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    memset(fb->map, 0, fb->size);
    return 0;
}

// 释放 dumb buffer
void destroy_fb(int drm_fd, drm_fb *fb)
{
    drmModeRmFB(drm_fd, fb->fb_id);
    struct drm_mode_destroy_dumb dreq = {};
    dreq.handle = fb->handle;
    ioctl(drm_fd, DRM_IOCTL_MODE_DESTROY_DUMB, &dreq);
}

// PNG 加载到 ARGB buffer
int load_png(const char *filename, uint32_t *dst, int stride)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) { perror("open png"); return -1; }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) return -1;
    png_infop info = png_create_info_struct(png);
    if (!info) return -1;

    if (setjmp(png_jmpbuf(png))) { fclose(fp); return -1; }
    png_init_io(png, fp);
    png_read_info(png, info);

    int w = png_get_image_width(png, info);
    int h = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && png_get_bit_depth(png, info) < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    png_read_update_info(png, info);

    png_bytep row = (png_bytep) malloc(4 * w);
    for (int y = 0; y < h; y++) {
        png_read_row(png, row, NULL);
        memcpy((uint8_t*)dst + y * stride, row, w * 4);
    }
    free(row);
    fclose(fp);
    return 0;
}

// 找 Overlay plane
uint32_t find_overlay_plane(int drm_fd, uint32_t crtc_id)
{
    drmModePlaneRes *pres = drmModeGetPlaneResources(drm_fd);
    if (!pres) { perror("drmModeGetPlaneResources"); return 0; }

    for (uint32_t i = 0; i < pres->count_planes; i++) {
        drmModePlane *plane = drmModeGetPlane(drm_fd, pres->planes[i]);
        if (!plane) continue;

        drmModeObjectProperties *props =
            drmModeObjectGetProperties(drm_fd, plane->plane_id,
                                       DRM_MODE_OBJECT_PLANE);

        for (uint32_t j = 0; j < props->count_props; j++) {
            drmModePropertyRes *prop = drmModeGetProperty(drm_fd, props->props[j]);
            if (strcmp(prop->name, "type") == 0) {
                if (props->prop_values[j] == DRM_PLANE_TYPE_OVERLAY) {
                    printf("Found overlay plane: %u\n", plane->plane_id);
                    drmModeFreeProperty(prop);
                    drmModeFreeObjectProperties(props);
                    drmModeFreePlane(plane);
                    drmModeFreePlaneResources(pres);
                    return plane->plane_id;
                }
            }
            drmModeFreeProperty(prop);
        }
        drmModeFreeObjectProperties(props);
        drmModeFreePlane(plane);
    }
    drmModeFreePlaneResources(pres);
    return 0;
}

int main()
{
    int drm_fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
    if (drm_fd < 0) { perror("open drm"); return -1; }

    drmModeRes *res = drmModeGetResources(drm_fd);
    if (!res) { perror("drmModeGetResources"); return -1; }

    drmModeConnector *conn = drmModeGetConnector(drm_fd, res->connectors[0]);
    drmModeEncoder *enc = drmModeGetEncoder(drm_fd, conn->encoder_id);
    drmModeCrtc *crtc = drmModeGetCrtc(drm_fd, enc->crtc_id);

    drmModeModeInfo mode = conn->modes[0];

    // Primary FB (背景)
    drm_fb primary = {};
    create_fb(drm_fd, mode.hdisplay, mode.vdisplay, &primary);

    // 填充背景为蓝色
    for (int y = 0; y < mode.vdisplay; y++) {
        for (int x = 0; x < mode.hdisplay; x++) {
            primary.map[y * (primary.pitch/4) + x] = 0xFF0000FF; // ARGB 蓝
        }
    }

    drmModeSetCrtc(drm_fd, crtc->crtc_id, primary.fb_id, 0, 0,
                   &conn->connector_id, 1, &mode);

    // Overlay FB (图标)
    drm_fb overlay = {};
    int icon_w = 64, icon_h = 64;
    create_fb(drm_fd, icon_w, icon_h, &overlay);

    load_png("wifi_icon.png", overlay.map, overlay.pitch);

    uint32_t plane_id = find_overlay_plane(drm_fd, crtc->crtc_id);
    if (plane_id == 0) {
        fprintf(stderr, "No overlay plane found\n");
        return -1;
    }

    // 放右上角
    int dst_x = mode.hdisplay - icon_w - 10;
    int dst_y = 10;

    drmModeSetPlane(drm_fd, plane_id, crtc->crtc_id,
                    overlay.fb_id, 0,
                    dst_x, dst_y, icon_w, icon_h,
                    0, 0, icon_w << 16, icon_h << 16);

    printf("Overlay icon shown. Press Enter to exit...\n");
    getchar();

    destroy_fb(drm_fd, &overlay);
    destroy_fb(drm_fd, &primary);

    drmModeFreeConnector(conn);
    drmModeFreeEncoder(enc);
    drmModeFreeCrtc(crtc);
    drmModeFreeResources(res);
    close(drm_fd);
    return 0;
}

