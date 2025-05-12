#!/bin/bash


echo "$0 file is running"

echo "BUILD_MODULE_DIR: ${BUILD_MODULE_DIR}"
echo "BUILD_TYPE: ${BUILD_TYPE}"
echo "BUILD_INSTALL_DIR: ${BUILD_INSTALL_DIR}"
echo "PLATFORM_TYPE: ${PLATFORM_TYPE}"

cd ${BUILD_MODULE_DIR}
rm -rf bin.v2
if [ ! -e "${BUILD_MODULE_DIR}/b2" ];then
    echo "bootstrap.sh will run, ubuntu will done"
    ./bootstrap.sh
fi
  
cp project-config-${PLATFORM_TYPE}.jam project-config.jam
./b2 clean

./b2 install --with-system --with-thread --with-filesystem --prefix=${BUILD_INSTALL_DIR}

echo "$0 file is exit"