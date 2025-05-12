#!/bin/bash

echo "$0 file is running"
#传入参数检查
if [ $# -ne 6 ];then
    echo "invalid param num: $#"
    exit 1
fi
#获取传入参数
export PLATFORM_TYPE=$1
MODULE_DIR=$2
export TARGET_NAME=$3
export IS_FORCE=$4
INSTALL_DIR=$5
SYSROOT_DIR=$6
# export BUILD_TYPE=RelWithDebInfo
export BUILD_TYPE=Debug
# export BUILD_TYPE=Release
#获取当前工程名
CUR_DIR=$(readlink -e ${BASH_SOURCE})
CUR_DIR_NAME=$(dirname ${CUR_DIR})
echo "build CUR_DIR_NAME: ${CUR_DIR_NAME}"
export PRJ_DIR=${CUR_DIR_NAME}/..
#安装和检查编译环境
source ${PRJ_DIR}/script/setenv.sh ${PLATFORM_TYPE} ${PRJ_DIR}
#获取cpu个数，进行多线程编译
let CORE_NUM=$(cat /proc/cpuinfo | grep processor | wc -l)/2
if [ ${CORE_NUM} -eq 0 ];then
    CORE_NUM=1;
fi
echo "CORE_NUM: ${CORE_NUM}"
export CORE_NUM=${CORE_NUM}
#编译路径检查
export BUILD_MODULE_DIR=${PRJ_DIR}/${MODULE_DIR}/
if [ ! -e ${BUILD_MODULE_DIR} ];then
    echo "not exsit module dir:${BUILD_MODULE_DIR}"
    exit 1
fi
#安装目录创建
export BUILD_INSTALL_DIR=${PRJ_DIR}/${INSTALL_DIR}/${PLATFORM_TYPE}/
# export PKG_CONFIG_PATH=${BUILD_INSTALL_DIR}
mkdir -p ${BUILD_INSTALL_DIR}
#依赖目录
export BUILD_DEPEND_DIR="${PRJ_DIR}/${INSTALL_DIR}/${PLATFORM_TYPE};"
BUILD_DEPEND_DIR+="${PRJ_DIR}/${SYSROOT_DIR}/${PLATFORM_TYPE};"
#交叉编译设置
export BUILD_TOOLCHAIN_FILE=
BUILD_CC_DIR=gcc
BUILD_CXX_DIR=g++
if [ ${PLATFORM_TYPE} = ubuntu ];then
    echo "ubuntu build PLATFORM_TYPE: ${PLATFORM_TYPE}"
elif [ ${PLATFORM_TYPE} = mtk8678 ];then
    echo "mtk8678 build PLATFORM_TYPE: ${PLATFORM_TYPE}"
    BUILD_TOOLCHAIN_FILE=${PRJ_DIR}/cmake/toolchains/cross/arm-mtk8678-gcc.cmake
elif [ ${PLATFORM_TYPE} = mtk8676t ];then
    echo "mtk8676t build PLATFORM_TYPE: ${PLATFORM_TYPE}"
    BUILD_TOOLCHAIN_FILE=${PRJ_DIR}/cmake/toolchains/cross/arm-mtk8676t-gcc.cmake
elif [ ${PLATFORM_TYPE} = qcm6125 ];then
    echo "qcm6125 build PLATFORM_TYPE: ${PLATFORM_TYPE}"
    BUILD_TOOLCHAIN_FILE=${PRJ_DIR}/cmake/toolchains/cross/arm-qcm6125-gcc.cmake
else
    echo "ubuntu build PLATFORM_TYPE: ${PLATFORM_TYPE}"
fi

if [[ $MODULE_DIR =~ thirdparty/.* ]]; then
    echo "Detected thirdyparty module ${MODULE_DIR}."
    # 执行针对 thirdyparty 的特定编译步骤
    source ${PRJ_DIR}/script/${MODULE_DIR}.sh
else
    echo "Detected no-thirdyparty module."
    #编译
    if [ "${IS_FORCE}" = "True" -o ! -e ${BUILD_MODULE_DIR}/${PLATFORM_TYPE}_build ];then
        rm -rf ${BUILD_MODULE_DIR}/${PLATFORM_TYPE}_build
        cmake -S ${BUILD_MODULE_DIR}/ \
        -DBUILD_PRJ_DIR=${PRJ_DIR} \
        -DBUILD_PLATFORM_TYPE=${PLATFORM_TYPE} \
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
        -DCMAKE_MODULE_PATH=${BUILD_DEPEND_DIR} \
        -DCMAKE_PREFIX_PATH=${BUILD_DEPEND_DIR} \
        -DCMAKE_TOOLCHAIN_FILE=${BUILD_TOOLCHAIN_FILE} \
        -DCMAKE_INSTALL_PREFIX=${BUILD_INSTALL_DIR} \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DMY_VARIABLE=hello \
        -B ${BUILD_MODULE_DIR}/${PLATFORM_TYPE}_build
    fi
    #build & install
    echo "Detected no-thirdyparty 111module."
    cmake --build ${BUILD_MODULE_DIR}/${PLATFORM_TYPE}_build --target ${TARGET_NAME} -j 1
fi

echo "$0 file is exit"
