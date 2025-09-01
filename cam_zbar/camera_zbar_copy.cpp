#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <signal.h>
#include <zbar.h>
#include <opencv2/opencv.hpp>

#define WIDTH 1920
#define HEIGHT 1080

volatile bool stop_flag = false;
void sigint_handler(int) { stop_flag = true; }

void parse_wifi(const std::string& text) {
    if (text.substr(0,5) != "WIFI:") {
        std::cout << "Not a WiFi QR code" << std::endl;
        return;
    }
    std::string ssid, passwd, enc;
    size_t pos = 5;
    while (pos < text.size()) {
        if (text.substr(pos,2) == "S:") {
            pos += 2;
            size_t end = text.find(';', pos);
            ssid = text.substr(pos, end - pos);
            pos = (end != std::string::npos) ? end+1 : text.size();
        } else if (text.substr(pos,2) == "P:") {
            pos += 2;
            size_t end = text.find(';', pos);
            passwd = text.substr(pos, end - pos);
            pos = (end != std::string::npos) ? end+1 : text.size();
        } else if (text.substr(pos,2) == "T:") {
            pos += 2;
            size_t end = text.find(';', pos);
            enc = text.substr(pos, end - pos);
            pos = (end != std::string::npos) ? end+1 : text.size();
        } else {
            size_t end = text.find(';', pos);
            pos = (end != std::string::npos) ? end+1 : text.size();
        }
    }
    std::cout << "SSID: " << ssid << ", Encryption: " << enc << ", Password: " << passwd << std::endl;
}

struct Buffer {
    void* start;
    size_t length;
};

int main() {
    signal(SIGINT, sigint_handler);

    int fd = open("/dev/video11", O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return -1;
    }

    v4l2_format fmt{};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    fmt.fmt.pix_mp.width = WIDTH;
    fmt.fmt.pix_mp.height = HEIGHT;
    fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_UYVY;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0) {
        perror("VIDIOC_S_FMT failed");
        return -1;
    }

    v4l2_requestbuffers req{};
    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    req.memory = V4L2_MEMORY_MMAP;
    if(ioctl(fd, VIDIOC_REQBUFS, &req) < 0) {
        perror("VIDIOC_REQBUFS failed");
        return -1;
    }

    std::vector<Buffer> buffers(req.count);
    for (int i = 0; i < req.count; i++) {
        v4l2_buffer buf{};
        v4l2_plane planes[VIDEO_MAX_PLANES]{};
        buf.type = req.type;
        buf.memory = req.memory;
        buf.index = i;
        buf.m.planes = planes;
        buf.length = 1;

        if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) {
            perror("VIDIOC_QUERYBUF failed");
            return -1;
        }

        buffers[i].length = buf.m.planes[0].length;
        buffers[i].start = mmap(nullptr, buf.m.planes[0].length,
                                PROT_READ | PROT_WRITE, MAP_SHARED, fd,
                                buf.m.planes[0].m.mem_offset);
        if (buffers[i].start == MAP_FAILED) {
            perror("mmap failed");
            return -1;
        }

        if (ioctl(fd, VIDIOC_QBUF, &buf) < 0) {
            perror("VIDIOC_QBUF failed");
            return -1;
        }
    }

    v4l2_buf_type type = static_cast<v4l2_buf_type>(req.type);
    if (ioctl(fd, VIDIOC_STREAMON, &type) < 0) {
        perror("VIDIOC_STREAMON failed");
        return -1;
    }

    std::cout << "Video stream started." << std::endl;

    zbar::ImageScanner scanner;
    scanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);

    while (!stop_flag) {
        v4l2_buffer buf{};
        v4l2_plane planes[VIDEO_MAX_PLANES]{};
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.m.planes = planes;
        buf.length = 1;

        if (ioctl(fd, VIDIOC_DQBUF, &buf) < 0) {
            perror("VIDIOC_DQBUF failed");
            continue;
        }

        cv::Mat uyvy(HEIGHT, WIDTH, CV_8UC2, buffers[buf.index].start);
        cv::Mat gray;
        cv::cvtColor(uyvy, gray, cv::COLOR_YUV2GRAY_UYVY);

        zbar::Image zimage(WIDTH, HEIGHT, "Y800", gray.data, WIDTH * HEIGHT);
        int n = scanner.scan(zimage);
        if (n > 0) {
            for (auto symbol = zimage.symbol_begin(); symbol != zimage.symbol_end(); ++symbol) {
                std::string data = symbol->get_data();
                std::cout << "QR: " << data << std::endl;
                parse_wifi(data);
            }
        }

        // 入队缓冲区
        if (ioctl(fd, VIDIOC_QBUF, &buf) < 0) {
            perror("VIDIOC_QBUF failed");
        }

        // 可选显示窗口
        // cv::imshow("Gray", gray);
        // if (cv::waitKey(1) == 27) break; // ESC退出
    }

    ioctl(fd, VIDIOC_STREAMOFF, &type);

    for (auto& b : buffers) {
        munmap(b.start, b.length);
    }
    close(fd);
    std::cout << "Resources released successfully." << std::endl;

    return 0;
}

