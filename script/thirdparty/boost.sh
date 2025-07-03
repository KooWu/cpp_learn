#!/bin/bash


echo "$0 file is running"

echo "CVI_BUILD_MODULE: ${CVI_BUILD_MODULE}"
cd ${CVI_BUILD_MODULE}
rm -rf bin.v2
if [ ! -e "b2" ];then
    echo "bootstrap.sh will run, x86 will done"
    ./bootstrap.sh
fi
  
cp project-config-${CVI_BUILD_PLATFORM}.jam project-config.jam
./b2 clean

./b2 install --with-system --with-thread --with-filesystem --prefix=${CVI_INSTALL_PREFIX}

echo "$0 file is exit"