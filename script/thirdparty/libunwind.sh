#!/bin/bash


echo "$0 file is running"

echo "BUILD_MODULE_DIR: ${BUILD_MODULE_DIR}"
echo "BUILD_TYPE: ${BUILD_TYPE}"
echo "BUILD_INSTALL_DIR: ${BUILD_INSTALL_DIR}"
echo "PLATFORM_TYPE: ${PLATFORM_TYPE}"
echo "PLATFORM_TYPE: ${CC}"
echo "CONFIGURE_FLAGS: ${CONFIGURE_FLAGS}"

cd ${BUILD_MODULE_DIR}
autoreconf -i
if [ ${PLATFORM_TYPE} = ubuntu ]; then
./configure --prefix=${BUILD_INSTALL_DIR}
else
# ./configure --target=aarch64-linux-gun --host=aarch64-linux-gun --build=x86_64-linux --prefix=${BUILD_INSTALL_DIR}
./configure ${CONFIGURE_FLAGS} --prefix=${BUILD_INSTALL_DIR}
fi

make clean
make -j ${CORE_NUM}
make install

echo "$0 file is exit"