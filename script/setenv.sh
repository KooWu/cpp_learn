#!/bin/bash

PLATFORM_TYPE=$1
PRJ_DIR=$2

if [ "${PLATFORM_TYPE}" = "mtk8678" ];then
    if [  ! -e /opt/poky/5.0/ ];then
        echo "error: please install mtk8678 compiler toolchain, exit"
        exit 1
    else
        echo "mtk8678 compiler toolchain is installed, path: /opt/poky/5.0/"
    fi
    source /opt/poky/5.0/environment-setup-aarch64-poky-linux
    echo "source done"
elif [ "${PLATFORM_TYPE}" = "mtk8676t" ];then
    if [  ! -e /opt/poky/5.0/ ];then
        echo "error: please install mtk8676t compiler toolchain, exit"
        exit 1
    else
        echo "mtk8676t compiler toolchain is installed, path: /opt/poky/5.0/"
    fi
    source /opt/poky/5.0/environment-setup-aarch64-poky-linux
    echo "source done"
elif [ "${PLATFORM_TYPE}" = "qcm6125" ];then
    if [  ! -e /opt/qcm6125/ ];then
        echo "error: please install qcm6125 compiler toolchain, exit"
        exit 1
    else
        echo "qcm6125 compiler toolchain is installed, path: //opt/qcm6125//"
    fi
    source //opt/qcm6125//environment-setup-aarch64-oe-linux
    echo "source done"
fi
