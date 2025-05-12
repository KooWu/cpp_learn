SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)

# specify the cross compiler
SET(TOOLCHAIN_PATH /opt/poky/5.0/)
SET(CMAKE_SYSROOT ${TOOLCHAIN_PATH}/sysroots/aarch64-poky-linux)
SET(CMAKE_C_COMPILER   ${TOOLCHAIN_PATH}/sysroots/x86_64-pokysdk-linux/usr/bin/aarch64-poky-linux/aarch64-poky-linux-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/sysroots/x86_64-pokysdk-linux/usr/bin/aarch64-poky-linux/aarch64-poky-linux-g++)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
# SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)