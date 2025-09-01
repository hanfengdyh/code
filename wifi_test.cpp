#include <zbar.h>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
/*
    二维码解析wifi数据
*/
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

    std::cout << "SSID: " << ssid << std::endl;
    std::cout << "Encryption: " << enc << std::endl;
    std::cout << "Password: " << passwd << std::endl;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <image_file>" << std::endl;
        return 1;
    }

    // 使用 C++ 风格 OpenCV 读取图片 (灰度)
    cv::Mat img = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cerr << "Cannot load image " << argv[1] << std::endl;
        return 1;
    }

    int width = img.cols;
    int height = img.rows;

    // 初始化 zbar
    zbar::ImageScanner scanner;
    scanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);

    // 创建 zbar image
    zbar::Image zimage(width, height, "Y800", img.data, width * height);

    // 扫描二维码
    int n = scanner.scan(zimage);
    if (n > 0) {
        for (auto symbol = zimage.symbol_begin(); symbol != zimage.symbol_end(); ++symbol) {
            std::string data = symbol->get_data();
            std::cout << "Raw QR content: " << data << std::endl;
            parse_wifi(data);
        }
    } else {
        std::cout << "No QR code found" << std::endl;
    }

    return 0;
}
