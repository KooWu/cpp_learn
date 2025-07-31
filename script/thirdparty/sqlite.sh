#!/bin/bash


echo "$0 file is running"

cd ${CVI_BUILD_MODULE}
if [ ${CVI_BUILD_PLATFORM} = x86 ]; then
./configure --prefix=${CVI_INSTALL_PREFIX}
else
./configure  --host=arm-linux --prefix=${CVI_INSTALL_PREFIX}
fi
make clean
make -j "$(nproc)"
make install

echo "$0 file is exit"