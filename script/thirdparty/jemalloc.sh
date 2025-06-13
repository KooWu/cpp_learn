#!/bin/bash


echo "$0 file is running"

cd ${CVI_BUILD_MODULE}

# ./autogen.sh
if [ -f "configure" ]; then
    echo "configure is exist"
else
./autogen.sh
fi

if [ ${CVI_BUILD_PLATFORM} = x86 ]; then
./configure --prefix=${CVI_INSTALL_PREFIX} --enable-prof
else
echo "CONFIGURE_FLAGS: $CONFIGURE_FLAGS"
./configure  CFLAGS="$CFLAGS -S" $CONFIGURE_FLAGS --prefix=${CVI_INSTALL_PREFIX} --enable-prof
fi

make clean
make -j 8
make install

# echo "$0 file is exit"