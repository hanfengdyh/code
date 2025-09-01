cmake_minimum_required(VERSION 3.10)
project(drm_overlay CXX)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/host/bin/aarch64-buildroot-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/host/bin/aarch64-buildroot-linux-gnu-g++)

set(CMAKE_SYSROOT /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/host/aarch64-buildroot-linux-gnu/sysroot)

# 包含头文件
include_directories(${CMAKE_SYSROOT}/usr/include)
include_directories(${CMAKE_SYSROOT}/usr/include/drm)

# 链接库目录
link_directories(${CMAKE_SYSROOT}/usr/lib)

# 编译可执行文件
add_executable(drm_overlay drm_overlay.cpp)

# 链接 libdrm 和 libgif
target_link_libraries(drm_overlay PRIVATE drm gif png)

