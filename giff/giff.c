#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <gif_lib.h>
#include <drm/drm.h>
#include <drm/drm_mode.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

#define GIF_FILE "two_blocks.gif"

// 屏幕/动图分辨率
#define WIDTH 426
#define HEIGHT 694

// ================= DRM 初始化 =================
struct drm_context {
    int fd;
    uint32_t fb_id;
    uint32_t crtc_id;
    uint32_t conn_id;
    uint32_t handle;
    uint32_t pitch;
    uint64_t size;
    uint32_t *fb_base;
};

int drm_init(struct drm_context *ctx) {
    ctx->fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
    if (ctx->fd < 0) { perror("open drm"); return -1; }

    drmModeRes *res = drmModeGetResources(ctx->fd);
    if (!res) { perror("drmModeGetResources"); return -1; }

    ctx->conn_id = res->connectors[0]; // 默认第一个
    drmModeConnector *conn = drmModeGetConnector(ctx->fd, ctx->conn_id);
    if (!conn || conn->connection != DRM_MODE_CONNECTED) {
        fprintf(stderr, "No connected connector\n");
        return -1;
    }

    ctx->crtc_id = res->crtcs[0];
    drmModeModeInfo mode = conn->modes[0];

    // 创建 dumb buffer
    struct drm_mode_create_dumb creq = {0};
    creq.width = WIDTH;
    creq.height = HEIGHT;
    creq.bpp = 32;
    if (ioctl(ctx->fd, DRM_IOCTL_MODE_CREATE_DUMB, &creq) < 0) {
        perror("create dumb"); return -1;
    }
    ctx->handle = creq.handle;
    ctx->pitch  = creq.pitch;
    ctx->size   = creq.size;

    // 建立 framebuffer
    if (drmModeAddFB(ctx->fd, WIDTH, HEIGHT, 24, 32,
                     ctx->pitch, ctx->handle, &ctx->fb_id)) {
        perror("drmModeAddFB"); return -1;
    }

    // 设置 CRTC 输出
    if (drmModeSetCrtc(ctx->fd, ctx->crtc_id, ctx->fb_id, 0, 0,
                       &ctx->conn_id, 1, &mode)) {
        perror("drmModeSetCrtc"); return -1;
    }

    // 映射
    struct drm_mode_map_dumb mreq = {0};
    mreq.handle = ctx->handle;
    if (ioctl(ctx->fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq)) {
        perror("map dumb"); return -1;
    }

    ctx->fb_base = mmap(0, ctx->size,
                        PROT_READ | PROT_WRITE, MAP_SHARED,
                        ctx->fd, mreq.offset);
    if (ctx->fb_base == MAP_FAILED) {
        perror("mmap"); return -1;
    }

    // 黑屏清理
    memset(ctx->fb_base, 0, ctx->size);
    return 0;
}

// ================= GIF 渲染 =================
void render_frame(SavedImage *frame, ColorMapObject *cmap,
                  struct drm_context *ctx) {
    GifImageDesc *desc = &frame->ImageDesc;
    int left = desc->Left;
    int top  = desc->Top;
    int w    = desc->Width;
    int h    = desc->Height;

    if (left + w > WIDTH || top + h > HEIGHT) {
        fprintf(stderr, "Frame out of bounds %dx%d @ %d,%d\n", w,h,left,top);
        return;
    }

    for (int y = 0; y < h; y++) {
        uint32_t *dst = (uint32_t*)((uint8_t*)ctx->fb_base + (y + top) * ctx->pitch) + left;
        uint8_t *src  = frame->RasterBits + y * w;
        for (int x = 0; x < w; x++) {
            int idx = src[x];
            GifColorType c = cmap->Colors[idx];
            dst[x] = 0xFF000000 | (c.Red << 16) | (c.Green << 8) | c.Blue;
        }
    }
}

int main() {
    struct drm_context ctx;
    if (drm_init(&ctx) < 0) return -1;

    // 打开 GIF
    int err;
    GifFileType *gif = DGifOpenFileName(GIF_FILE, &err);
    if (!gif) { fprintf(stderr, "DGifOpenFileName error %d\n", err); return -1; }
    if (DGifSlurp(gif) != GIF_OK) { fprintf(stderr, "DGifSlurp error\n"); return -1; }

    printf("GIF 帧数: %d\n", gif->ImageCount);

    // 循环播放
    while (1) {
        for (int f = 0; f < gif->ImageCount; f++) {
            SavedImage *frame = &gif->SavedImages[f];
            ColorMapObject *cmap = frame->ImageDesc.ColorMap ? frame->ImageDesc.ColorMap : gif->SColorMap;

            // 延迟
            int delay = 100;
            for (int e = 0; e < frame->ExtensionBlockCount; e++) {
                ExtensionBlock *ext = &frame->ExtensionBlocks[e];
                if (ext->Function == GRAPHICS_EXT_FUNC_CODE && ext->ByteCount >= 4) {
                    delay = (ext->Bytes[1] | (ext->Bytes[2] << 8)) * 10;
                }
            }

            render_frame(frame, cmap, &ctx);

            usleep(delay * 1000);
        }
    }

    DGifCloseFile(gif, &err);
    return 0;
}

