/*#include <RgaApi.h>
#include <drm_alloc.h>
#include <RgaUtils.h>
#include <im2d.hpp>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cstring>
#include <gif_lib.h>
#include <drm/drm.h>
#include <drm/drm_mode.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

#define GIF_FILE "/userdata/two_block.gif"
#define WIDTH 426
#define HEIGHT 694
#define DST_WIDTH 694
#define DST_HEIGHT 426

// ================= DRM 上下文 =================
struct DrmContext {
    int fd{-1};
    uint32_t fb_id{0};
    uint32_t crtc_id{0};
    uint32_t conn_id{0};
    uint32_t handle{0};
    uint32_t pitch{0};
    uint64_t size{0};
    uint32_t* fb_base{nullptr};

    ~DrmContext() {
        if (fb_base) {
            munmap(fb_base, size);
        }
        if (fb_id) {
            drmModeRmFB(fd, fb_id);
        }
        if (handle) {
            struct drm_mode_destroy_dumb dreq{0};
            dreq.handle = handle;
            ioctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, &dreq);
        }
        if (fd >= 0) close(fd);
    }
};

bool drm_init(DrmContext& ctx) {
    ctx.fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
    if (ctx.fd < 0) { perror("open drm"); return false; }

    drmModeRes* res = drmModeGetResources(ctx.fd);
    if (!res) { perror("drmModeGetResources"); return false; }

    ctx.conn_id = res->connectors[0];
    drmModeConnector* conn = drmModeGetConnector(ctx.fd, ctx.conn_id);
    if (!conn || conn->connection != DRM_MODE_CONNECTED) {
        std::cerr << "No connected connector\n";
        return false;
    }

    ctx.crtc_id = res->crtcs[0];
    drmModeModeInfo mode = conn->modes[0];

    drm_mode_create_dumb creq{};
    creq.width = WIDTH;
    creq.height = HEIGHT;
    creq.bpp = 32;
    if (ioctl(ctx.fd, DRM_IOCTL_MODE_CREATE_DUMB, &creq) < 0) {
        perror("create dumb"); return false;
    }

    ctx.handle = creq.handle;
    ctx.pitch  = creq.pitch;
    ctx.size   = creq.size;

    if (drmModeAddFB(ctx.fd, WIDTH, HEIGHT, 24, 32, ctx.pitch, ctx.handle, &ctx.fb_id)) {
        perror("drmModeAddFB"); return false;
    }

    if (drmModeSetCrtc(ctx.fd, ctx.crtc_id, ctx.fb_id, 0, 0, &ctx.conn_id, 1, &mode)) {
        perror("drmModeSetCrtc"); return false;
    }

    drm_mode_map_dumb mreq{};
    mreq.handle = ctx.handle;
    if (ioctl(ctx.fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq)) {
        perror("map dumb"); return false;
    }

    ctx.fb_base = static_cast<uint32_t*>(mmap(nullptr, ctx.size,
                                              PROT_READ | PROT_WRITE, MAP_SHARED,
                                              ctx.fd, mreq.offset));
    if (ctx.fb_base == MAP_FAILED) {
        perror("mmap"); return false;
    }

    std::memset(ctx.fb_base, 0, ctx.size);
    return true;
}

uint32_t *offscreen_buf;

// ================= GIF 渲染 =================
void render_frame(SavedImage* frame, ColorMapObject* cmap, DrmContext& ctx) {
    auto& desc = frame->ImageDesc;
    int left = desc.Left;
    int top  = desc.Top;
    int w    = desc.Width;
    int h    = desc.Height;

    if (left + w > WIDTH || top + h > HEIGHT) {
        std::cerr << "Frame out of bounds " << w << "x" << h << " @ " << left << "," << top << "\n";
        return;
    }

    for (int y = 0; y < h; y++) {
        uint32_t* dst = offscreen_buf + (y + top) * WIDTH + left;
        uint8_t* src = frame->RasterBits + y * w;
        for (int x = 0; x < w; x++) {
            int idx = src[x];
            GifColorType c = cmap->Colors[idx];
            dst[x] = 0xFF000000 | (c.Red << 16) | (c.Green << 8) | c.Blue;
        }
    }
}

int main() {
    DrmContext ctx;
    if (!drm_init(ctx)) return -1;

    offscreen_buf = new uint32_t [WIDTH * HEIGHT]{};

    // 打开 GIF 文件
    int err;
    GifFileType* gif = DGifOpenFileName(GIF_FILE, &err);
    if (!gif) {
        std::cerr << "DGifOpenFileName error " << err << "\n";
        return -1;
    }
    if (DGifSlurp(gif) != GIF_OK) {
        std::cerr << "DGifSlurp error\n";
        DGifCloseFile(gif, &err);
        return -1;
    }

    std::cout << "GIF 帧数: " << gif->ImageCount << "\n";

    int ret = c_RkRgaInit(); 
    if (ret != 0) { std::cerr << "RGA init failed!" << std::endl; return -1; }

    struct { 
        uint8_t *drm_buf; 
        int drm_fd; 
        int drm_handle; 
        size_t size; 
    } drm_dst; 

    drm_dst.drm_buf = (uint8_t*)drm_buf_alloc(DST_WIDTH, DST_HEIGHT, 32, 
                                              &drm_dst.drm_fd, &drm_dst.drm_handle, &drm_dst.size, 0);
    if (!drm_dst.drm_buf) { 
        std::cerr << "drm_buf_alloc failed" << std::endl; 
        return -1;  
    }

    uint32_t* tmp_buf = new uint32_t[gif->SWidth * gif->SHeight]{};

    while (true) {
        for (int f = 0; f < gif->ImageCount; f++) {
           //SavedImage* frame = &gif->SavedImages[f];
            uint32_t * frame = &gif->SavedImages[f];
            ColorMapObject* cmap = frame->ImageDesc.ColorMap ? frame->ImageDesc.ColorMap : gif->SColorMap;

            // 默认延迟
            int delay = 100;
            for (int e = 0; e < frame->ExtensionBlockCount; e++) {
                ExtensionBlock* ext = &frame->ExtensionBlocks[e];
                if (ext->Function == GRAPHICS_EXT_FUNC_CODE && ext->ByteCount >= 4) {
                    delay = (ext->Bytes[1] | (ext->Bytes[2] << 8)) * 10;
                }
            }

            // 1️⃣ 清理临时 buffer
            std::memset(tmp_buf, 0, gif->SWidth * gif->SHeight * 4);

            // 2️⃣ 渲染 GIF 帧到临时 buffer
            for (int y = 0; y < frame->ImageDesc.Height; y++) {
                uint32_t* dst = tmp_buf + (y + frame->ImageDesc.Top) * gif->SWidth + frame->ImageDesc.Left;
                uint8_t* src = frame->RasterBits + y * frame->ImageDesc.Width;
                for (int x = 0; x < frame->ImageDesc.Width; x++) {
                    int idx = src[x];
                    GifColorType c = cmap->Colors[idx];
                    dst[x] = 0xFF000000 | (c.Red << 16) | (c.Green << 8) | c.Blue;
                }
            }
	    std::cout<< gif->SWidth<<gif->SHeight<<std::endl;
            // 3️⃣ RGA 旋转临时 buffer到 framebuffer
            rga_buffer_t src = wrapbuffer_virtualaddr(tmp_buf, gif->SWidth, gif->SHeight, RK_FORMAT_BGRA_8888);
            rga_buffer_t dst = wrapbuffer_virtualaddr(offscreen_buf, DST_WIDTH, DST_HEIGHT, RK_FORMAT_BGRA_8888);
	    
            IM_STATUS status = imrotate(src, dst, IM_HAL_TRANSFORM_ROT_90);
            if (status != IM_STATUS_SUCCESS) {
                std::cerr << "imrotate failed: " << status << std::endl;
            }
            render_frame(, cmap, &ctx);

            // 4️⃣ 延时等待下一帧
            usleep(delay * 1000);
        }
    }


    // 5️⃣ 循环外关闭 GIF
    DGifCloseFile(gif, &err);
    delete[] tmp_buf;

    return 0;
}
*/
#include <RgaApi.h>
#include <im2d.hpp>
#include <gif_lib.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <drm/drm.h>
#include <drm/drm_mode.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

#define GIF_FILE "/userdata/two_block.gif"

// GIF 原始尺寸（横屏）
#define GIF_WIDTH 694
#define GIF_HEIGHT 426

// framebuffer 尺寸（纵屏，旋转后）
#define FB_WIDTH GIF_HEIGHT
#define FB_HEIGHT GIF_WIDTH

uint32_t* temp_buf = nullptr;

// ================= DRM framebuffer 上下文 =================
struct DrmContext {
    int fd{-1};
    uint32_t fb_id{0};
    uint32_t crtc_id{0};
    uint32_t conn_id{0};
    uint32_t handle{0};
    uint32_t pitch{0};
    uint64_t size{0};
    uint32_t* fb_base{nullptr};

    ~DrmContext() {
        if (fb_base) munmap(fb_base, size);
        if (fb_id) drmModeRmFB(fd, fb_id);
        if (handle) {
            struct drm_mode_destroy_dumb dreq{0};
            dreq.handle = handle;
            ioctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, &dreq);
        }
        if (fd >= 0) close(fd);
    }
};

bool drm_init(DrmContext& ctx, int width, int height) {
    ctx.fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
    if (ctx.fd < 0) { perror("open drm"); return false; }

    drmModeRes* res = drmModeGetResources(ctx.fd);
    if (!res) { perror("drmModeGetResources"); return false; }

    ctx.conn_id = res->connectors[0];
    drmModeConnector* conn = drmModeGetConnector(ctx.fd, ctx.conn_id);
    if (!conn || conn->connection != DRM_MODE_CONNECTED) { std::cerr << "No connected connector\n"; return false; }

    ctx.crtc_id = res->crtcs[0];
    drmModeModeInfo mode = conn->modes[0];

    drm_mode_create_dumb creq{};
    creq.width = width;
    creq.height = height;
    creq.bpp = 32;
    if (ioctl(ctx.fd, DRM_IOCTL_MODE_CREATE_DUMB, &creq) < 0) { perror("create dumb"); return false; }

    ctx.handle = creq.handle;
    ctx.pitch  = creq.pitch;
    ctx.size   = creq.size;

    if (drmModeAddFB(ctx.fd, width, height, 24, 32, ctx.pitch, ctx.handle, &ctx.fb_id)) { perror("drmModeAddFB"); return false; }

    if (drmModeSetCrtc(ctx.fd, ctx.crtc_id, ctx.fb_id, 0, 0, &ctx.conn_id, 1, &mode)) { perror("drmModeSetCrtc"); return false; }

    drm_mode_map_dumb mreq{};
    mreq.handle = ctx.handle;
    if (ioctl(ctx.fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq)) { perror("map dumb"); return false; }

    ctx.fb_base = static_cast<uint32_t*>(mmap(nullptr, ctx.size, PROT_READ | PROT_WRITE, MAP_SHARED, ctx.fd, mreq.offset));
    if (ctx.fb_base == MAP_FAILED) { perror("mmap"); return false; }

    std::memset(ctx.fb_base, 0, ctx.size);
    return true;
}

// ================= 渲染函数 =================
void render_frame(SavedImage* frame, ColorMapObject* cmap, uint32_t* buf, int buf_width, int buf_height) {
    int left = frame->ImageDesc.Left;
    int top  = frame->ImageDesc.Top;
    int w    = frame->ImageDesc.Width;
    int h    = frame->ImageDesc.Height;

    if (left + w > buf_width || top + h > buf_height) return;

    for (int y = 0; y < h; y++) {
        uint32_t* dst = buf + (top + y) * buf_width + left;
        uint8_t* src = frame->RasterBits + y * w;
        for (int x = 0; x < w; x++) {
            int idx = src[x];
            GifColorType c = cmap->Colors[idx];
            dst[x] = 0xFF000000 | (c.Red << 16) | (c.Green << 8) | c.Blue;
        }
    }
}

// ================= 主函数 =================
int main() {
    DrmContext ctx;
    if (!drm_init(ctx, FB_WIDTH, FB_HEIGHT)) return -1;

    temp_buf = new uint32_t[GIF_WIDTH * GIF_HEIGHT]{};

    // 打开 GIF
    int err;
    GifFileType* gif = DGifOpenFileName(GIF_FILE, &err);
    if (!gif || DGifSlurp(gif) != GIF_OK) { std::cerr << "Open GIF failed\n"; return -1; }

    // 初始化 RGA
    if (c_RkRgaInit() != 0) { std::cerr << "RGA init failed\n"; return -1; }

    std::cout << "GIF 帧数: " << gif->ImageCount << std::endl;

    while (true) {
        for (int f = 0; f < gif->ImageCount; f++) {
            SavedImage* frame = &gif->SavedImages[f];
            ColorMapObject* cmap = frame->ImageDesc.ColorMap ? frame->ImageDesc.ColorMap : gif->SColorMap;

            // ✅ 清空 buffer，保证只显示当前帧
            std::memset(temp_buf, 0, GIF_WIDTH * GIF_HEIGHT * 4);

            // 渲染当前帧
            render_frame(frame, cmap, temp_buf, GIF_WIDTH, GIF_HEIGHT);

            // RGA 旋转 90°到 framebuffer
            rga_buffer_t src = wrapbuffer_virtualaddr(temp_buf, GIF_WIDTH, GIF_HEIGHT, RK_FORMAT_BGRA_8888);
            rga_buffer_t dst = wrapbuffer_virtualaddr(ctx.fb_base, FB_WIDTH, FB_HEIGHT, RK_FORMAT_BGRA_8888);
            IM_STATUS status = imrotate(src, dst, IM_HAL_TRANSFORM_ROT_90);
            if (status != IM_STATUS_SUCCESS) std::cerr << "imrotate failed\n";

            // 获取延迟
            int delay = 100;
            for (int e = 0; e < frame->ExtensionBlockCount; e++) {
                ExtensionBlock* ext = &frame->ExtensionBlocks[e];
                if (ext->Function == GRAPHICS_EXT_FUNC_CODE && ext->ByteCount >= 4) {
                    delay = (ext->Bytes[1] | (ext->Bytes[2] << 8)) * 10;
                }
            }
            usleep(delay * 1000);
        }
    }

    delete[] temp_buf;
    DGifCloseFile(gif, &err);
    return 0;
}






