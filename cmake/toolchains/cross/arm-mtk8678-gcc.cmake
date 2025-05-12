SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)

# specify the cross compiler
SET(TOOLCHAIN_PATH /opt/poky/5.0/)
SET(CMAKE_SYSROOT ${TOOLCHAIN_PATH}/sysroots/aarch64-poky-linux)

set(CROSS_COMPILE_FLAGS "-march=armv8-a+crc -fstack-protector-strong -pie -fPIE -D_FORTIFY_SOURCE=2 -Wa,--noexecstack -Wformat -Wformat-security -Werror=format-security")

SET(CMAKE_C_COMPILER   ${TOOLCHAIN_PATH}/sysroots/x86_64-pokysdk-linux/usr/bin/aarch64-poky-linux/aarch64-poky-linux-gcc ${CROSS_COMPILE_FLAGS})
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/sysroots/x86_64-pokysdk-linux/usr/bin/aarch64-poky-linux/aarch64-poky-linux-g++ ${CROSS_COMPILE_FLAGS})

set(CMAKE_CXX_FLAGS "-fPIC -O2 -pipe -g -feliminate-unused-debug-types")
set(CMAKE_C_FLAGS "-fPIC -O2 -pipe -g -feliminate-unused-debug-types")
set(CMAKE_LD_FLAGS "-Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed  -Wl,-z,relro,-z,now")

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
# SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)