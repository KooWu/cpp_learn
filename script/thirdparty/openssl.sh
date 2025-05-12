#!/bin/bash


echo "$0 file is running"

echo "BUILD_MODULE_DIR: ${BUILD_MODULE_DIR}"
echo "BUILD_TYPE: ${BUILD_TYPE}"
echo "BUILD_INSTALL_DIR: ${BUILD_INSTALL_DIR}"
echo "PLATFORM_TYPE: ${PLATFORM_TYPE}"
echo "PLATFORM_TYPE: ${CC}"

cd ${BUILD_MODULE_DIR}
if [ ${PLATFORM_TYPE} = ubuntu ]; then
./Configure  linux-x86_64 no-asm shared no-async --prefix=${BUILD_INSTALL_DIR}
else
./Configure  linux-aarch64 no-asm shared no-async --prefix=${BUILD_INSTALL_DIR}
fi

make clean
make -j ${CORE_NUM}
make install

echo "$0 file is exit"