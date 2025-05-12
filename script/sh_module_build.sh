#!/bin/bash

# 初始化变量
PLATFORM_TYPE=ubuntu
MODULE_DIR=
TARGET_NAME=install
IS_FORCE=False
INSTALL_DIR=install
SYSROOT_DIR=install

# 处理选项
while getopts "p:d:o:s:t:fh" opt; do
  case $opt in
    p)
      PLATFORM_TYPE=$OPTARG
      ;;
    d)
      MODULE_DIR=$OPTARG
      ;;
    o)
      INSTALL_DIR=$OPTARG
      ;;
    s)
      SYSROOT_DIR=$OPTARG
      ;;
    t)
      TARGET_NAME=$OPTARG
      ;;
    f)
      IS_FORCE=True
      ;;
    h)
      echo "Usage: $0 [-p platform] [-d module_dir] [-o install_dir] [-s sysroot_dir] [-t target_name] [-f]"
      echo "Options:"
      echo "  -p          Specify the platform type (default: ubuntu)"
      echo "  -d          Specify the module directory (required)"
      echo "  -o          Specify the installation directory (default: install)"
      echo "  -s          Specify the sysroot directory (default: install)"
      echo "  -t          Specify the target name (default: install)"
      echo "  -f          Force the build operation"
      echo "  -h          Print this help message"
      exit 0
      ;;
    \?)
      echo "未知选项: -$OPTARG"
      exit 1
      ;;
  esac
done

# 跳过已处理的选项
shift $((OPTIND - 1))

# 打印选项值
echo "PLATFORM_TYPE: $PLATFORM_TYPE"
echo "MODULE_DIR: $MODULE_DIR"
echo "TARGET_NAME: $TARGET_NAME"
echo "INSTALL_DIR: $INSTALL_DIR"
echo "SYSROOT_DIR: $SYSROOT_DIR"
echo "IS_FORCE: $IS_FORCE"

if [ "${MODULE_DIR}" = "" ];then
    echo "module dir must not is empty"
fi
./module_build.sh $PLATFORM_TYPE $MODULE_DIR $TARGET_NAME $IS_FORCE $INSTALL_DIR $SYSROOT_DIR
