cmake_minimum_required(VERSION 3.10)
project(zbar_cam CXX)

#add_executable(camera_zbar camera_zbar.cpp)
add_executable(camera_zbar wifi_zbar.cpp)
# 如果需要 OpenCV 头文件
include_directories(${CMAKE_SYSROOT}/usr/include/opencv4)

# 链接库
target_link_libraries(camera_zbar
    ${CMAKE_SYSROOT}/usr/lib/libopencv_core.so
    ${CMAKE_SYSROOT}/usr/lib/libopencv_imgproc.so
    ${CMAKE_SYSROOT}/usr/lib/libopencv_imgcodecs.so
    ${CMAKE_SYSROOT}/usr/lib/libzbar.so
    v4l2
)

