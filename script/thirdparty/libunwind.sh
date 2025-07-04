#!/bin/bash


echo "$0 file is running"

cd ${CVI_BUILD_MODULE}

autoreconf -i

if [ ${CVI_BUILD_PLATFORM} = x86 ]; then
./configure --prefix=${CVI_INSTALL_PREFIX}
else
# ./configure  CFLAGS="$CFLAGS -S" $CONFIGURE_FLAGS --prefix=${CVI_INSTALL_PREFIX}
./configure --prefix=${CVI_INSTALL_PREFIX}
fi

make clean
make -j 8
make install

# echo "$0 file is exit"