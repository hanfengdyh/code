set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER   /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/host/bin/aarch64-buildroot-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/host/bin/aarch64-buildroot-linux-gnu-g++)

set(CMAKE_SYSROOT /home/zext/RV1126B/buildroot/output/rockchip_rv1126b/host/aarch64-buildroot-linux-gnu/sysroot)

# 关键：把 sysroot 路径传给编译器
set(CMAKE_C_FLAGS "--sysroot=${CMAKE_SYSROOT}")
set(CMAKE_CXX_FLAGS "--sysroot=${CMAKE_SYSROOT}")

# 查找路径设置
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

