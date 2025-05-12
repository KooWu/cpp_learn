#!/bin/bash


echo "$0 file is running"

echo "BUILD_MODULE_DIR: ${BUILD_MODULE_DIR}"
echo "BUILD_TYPE: ${BUILD_TYPE}"
echo "BUILD_INSTALL_DIR: ${BUILD_INSTALL_DIR}"
echo "PLATFORM_TYPE: ${PLATFORM_TYPE}"
echo "PLATFORM_TYPE: ${CC}"

cd ${BUILD_MODULE_DIR}
if [ ${PLATFORM_TYPE} = ubuntu ]; then
./configure --prefix=${BUILD_INSTALL_DIR}  --disable-unicode PKG_CONFIG_PATH=${PRJ_DIR}/install/${PLATFORM_TYPE}/lib/pkgconfig HTOP_NCURSES_CONFIG_SCRIPT=${PRJ_DIR}/install/${PLATFORM_TYPE}/bin/ncurses6-config
else
./configure --prefix=${BUILD_INSTALL_DIR}  --host=arm-linux PKG_CONFIG_PATH=${PRJ_DIR}/install/${PLATFORM_TYPE}/lib/pkgconfig HTOP_NCURSES_CONFIG_SCRIPT=${PRJ_DIR}/install/${PLATFORM_TYPE}/bin/ncurses6-config
fi

make clean
make -j ${CORE_NUM}
make install

echo "$0 file is exit"