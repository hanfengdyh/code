cmake_minimum_required(VERSION 3.10)
project(drm_single C)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# 交叉编译工具链
set(CMAKE_C_COMPILER /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/host/bin/aarch64-buildroot-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/host/bin/aarch64-buildroot-linux-gnu-g++)

# 指定 sysroot
set(CMAKE_SYSROOT /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/host/aarch64-buildroot-linux-gnu/sysroot)

# 包含 libdrm 头文件
include_directories(${CMAKE_SYSROOT}/usr/include)
include_directories(${CMAKE_SYSROOT}/usr/include/drm)
# 链接 libdrm 库
link_directories(${CMAKE_SYSROOT}/usr/lib)

add_executable(drm-single drm-single.c)

target_link_libraries(drm-single drm)  # libdrm.so

