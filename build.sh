#! /usr/bin/env bash

set -e

CUR_DIR=$(readlink -e ${BASH_SOURCE})
CUR_DIR_NAME=$(dirname ${CUR_DIR})

BUILD_BASE_PATH="${CUR_DIR_NAME}/script/build_base.sh"

# shellcheck source=/dev/null
source "${BUILD_BASE_PATH}" ${CUR_DIR_NAME}

function main() {
    parse_arguments "$@"
    check_arguments

    setup_environment

    echo "CVI_BUILD_TYPE: ${CVI_BUILD_TYPE}"
    echo "CVI_BUILD_MODULE: ${CVI_BUILD_MODULE}"
    echo "CVI_BUILD_TARGET: ${CVI_BUILD_TARGET}"
    echo "CVI_BUILD_PLATFORM: ${CVI_BUILD_PLATFORM}"
    echo "CVI_CLEAN_REBUILD: ${CVI_CLEAN_REBUILD}"
    echo "CVI_INSTALL_PREFIX: ${CVI_INSTALL_PREFIX}"
    echo "CVI_BUILD_SYSTEM: ${CVI_BUILD_SYSTEM}"
    echo "CVI_BUILD_TOOLCHAIN: ${CVI_BUILD_TOOLCHAIN}"
    echo "CVI_CMAKE_BUILD_TYPE: ${CVI_CMAKE_BUILD_TYPE}"

    if [[ ${CVI_CLEAN_REBUILD} -eq 1 ]]; then
        rm -rf ${CVI_BUILD_MODULE}/${CVI_BUILD_PLATFORM}_build
    fi

if [[ $CVI_BUILD_MODULE =~ thirdparty/.* ]]; then
    echo "Detected thirdyparty module ${CVI_BUILD_MODULE}."
    # 执行针对 thirdyparty 的特定编译步骤
    source script/${CVI_BUILD_MODULE}.sh
else
    cmake -S ${CVI_BUILD_MODULE}/ \
    -DCVI_BUILD_TYPE="${CVI_BUILD_TYPE}" \
    -DCVI_BUILD_PLATFORM="${CVI_BUILD_PLATFORM}" \
    -DCMAKE_BUILD_TYPE="${CVI_CMAKE_BUILD_TYPE}" \
    -DCMAKE_TOOLCHAIN_FILE="${CVI_BUILD_TOOLCHAIN}" \
    -DCMAKE_INSTALL_PREFIX="${CVI_INSTALL_PREFIX}" \
    -DCMAKE_MODULE_PATH=${CVI_INSTALL_PREFIX} \
    -DCMAKE_PREFIX_PATH=${CVI_INSTALL_PREFIX} \
    -DBUILD_SHARED_LIBS=ON \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -B ${CVI_BUILD_MODULE}/${CVI_BUILD_PLATFORM}_build

    cmake --build ${CVI_BUILD_MODULE}/${CVI_BUILD_PLATFORM}_build --target "${CVI_BUILD_TARGET}" -j "$(nproc)"
fi
}

main "$@"
