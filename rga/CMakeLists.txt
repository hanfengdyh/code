cmake_minimum_required(VERSION 3.10)
project(grga C CXX)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/host/bin/aarch64-buildroot-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/host/bin/aarch64-buildroot-linux-gnu-g++)

set(CMAKE_SYSROOT /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/host/aarch64-buildroot-linux-gnu/sysroot)

# 头文件
include_directories(
    ${CMAKE_SYSROOT}/usr/include
    ${CMAKE_SYSROOT}/usr/include/drm
    /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/build/rockchip-rga-master/samples/utils/allocator/include
    /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/build/rockchip-rga-master/include
    /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/build/rockchip-rga-master/im2d_api
)

# 链接目录：sysroot 下
link_directories(${CMAKE_SYSROOT}/usr/lib)

# 可执行文件
add_executable(rga_rotation rga.cpp /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/build/rockchip-rga-master/samples/utils/allocator/drm_alloc.cpp)

# 链接库，不要写绝对路径，直接写库名就行
target_link_libraries(rga_rotation
PRIVATE ${CMAKE_SYSROOT}/usr/lib/librga.so 
drm 
gif)

