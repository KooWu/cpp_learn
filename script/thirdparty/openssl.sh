#!/bin/bash


echo "$0 file is running"

cd ${CVI_BUILD_MODULE}
if [ ${CVI_BUILD_PLATFORM} = x86 ]; then
./Configure  linux-x86_64 no-asm shared no-async --prefix=${CVI_INSTALL_PREFIX}
else
./Configure  CFLAGS="$CFLAGS -S" $CONFIGURE_FLAGS linux-aarch64 no-asm shared no-async --prefix=${CVI_INSTALL_PREFIX}
fi

make clean
make -j 8
make install

echo "$0 file is exit"