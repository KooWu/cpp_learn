if(DEFINED ENV{QCM6125_SDK})
set(SDK_PATH $ENV{QCM6125_SDK})
else()
set(SDK_PATH /opt/qcm6125)
endif()
set(SDKTARGETSYSROOT ${SDK_PATH}/sysroots/aarch64-oe-linux)
set(CROSS_ROOTFS ${SDK_PATH}/sysroots/x86_64-qtisdk-linux)
set(CROSS_COMPILE_FLAGS "-march=armv8-a+crc -fstack-protector-strong -pie -fPIE -Wa,--noexecstack -Wformat -Wformat-security -Werror=format-security --sysroot=${SDKTARGETSYSROOT}")

set(ENV{PATH} $ENV{PATH}:${CROSS_ROOTFS}/usr/bin)
set(ENV{PATH} $ENV{PATH}:${CROSS_ROOTFS}/usr/sbin)
set(ENV{PATH} $ENV{PATH}:${CROSS_ROOTFS}/bin)
set(ENV{PATH} $ENV{PATH}:${CROSS_ROOTFS}/sbin)
set(ENV{PATH} $ENV{PATH}:${CROSS_ROOTFS}/usr/bin/aarch64-oe-linux)
set(ENV{PATH} $ENV{PATH}:${CROSS_ROOTFS}/usr/bin/aarch64-oe-linux-musl)

set(ENV{PKG_CONFIG_SYSROOT_DIR} ${SDKTARGETSYSROOT})
set(ENV{PKG_CONFIG_PATH} ${SDKTARGETSYSROOT}/usr/lib/pkgconfig:${SDKTARGETSYSROOT}/usr/share/pkgconfig)
set(ENV{CONFIG_SITE} ${SDK_PATH}/site-config-aarch64-oe-linux)
set(ENV{SDKTARGETSYSROOT} ${SDKTARGETSYSROOT})

#---------

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_GENERATOR "CodeBlocks - Unix Makefiles")
#set(CMAKE_MAKE_PROGRAM "/usr/bin/make")

#-------
set(CMAKE_SYSROOT ${SDKTARGETSYSROOT})

set(CMAKE_FIND_ROOT_PATH ${SDKTARGETSYSROOT})
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

set(CMAKE_C_COMPILER aarch64-oe-linux-gcc ${CROSS_COMPILE_FLAGS})
set(CMAKE_CXX_COMPILER aarch64-oe-linux-g++ ${CROSS_COMPILE_FLAGS})
set(CMAKE_C_COMPILER_AR aarch64-oe-linux-ar)
set(CMAKE_CXX_COMPILER_AR aarch64-oe-linux-ar)
set(CMAKE_C_COMPILER_RANLIB aarch64-oe-linux-ranlib)
set(CMAKE_LD aarch64-oe-linux-ld "--sysroot=${SDKTARGETSYSROOT}")
set(CMAKE_NM aarch64-oe-linux-nm)
set(CMAKE_OBJCOPY aarch64-oe-linux-objcopy)
set(CMAKE_OBJDUMP aarch64-oe-linux-objdump)
set(CMAKE_RANLIB aarch64-oe-linux-ranlib)
set(CMAKE_STRIP aarch64-oe-linux-strip)

set(Protobuf_PROTOC_EXECUTABLE ${CROSS_ROOTFS}/usr/bin/protoc)

#set(CMAKE_PREFIX_PATH aarch64-oe-linux-)
 
set(CMAKE_CXX_FLAGS "-fPIC    -g -Wa,--noexecstack -fexpensive-optimizations   -frename-registers -ftree-vectorize   -finline-functions -finline-limit=64   -Wno-error=maybe-uninitialized -Wno-error=unused-result")
set(CMAKE_C_FLAGS "-fPIC    -g -Wa,--noexecstack -fexpensive-optimizations   -frename-registers -ftree-vectorize   -finline-functions -finline-limit=64   -Wno-error=maybe-uninitialized -Wno-error=unused-result")
set(CMAKE_LD_FLAGS "-Wl, -Wl,--hash-style=gnu -Wl,--as-needed -Wl,-z,relro,-z,now,-z,noexecstack")