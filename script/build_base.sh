#! /usr/bin/env bash

set -e

CUR_DIR=$(readlink -e ${BASH_SOURCE})
CUR_DIR_NAME=$(dirname ${CUR_DIR})

CVI_BUILD_PROJECT_PATH=${CUR_DIR_NAME}/../

SUPPORTED_BUILD_TYPE=(
    debug
    release
    reldeb
    min
)

SUPPORTED_BUILD_PLATFORM=(
    x86
)

# shellcheck disable=SC2034
declare -A CVI_CMAKE_BUILD_TYPE_MAP=(
    ["debug"]="Debug"
    ["release"]="Release"
    ["reldeb"]="RelWithDebInfo"
    ["min"]="MinSizeRel"
)

export CVI_BUILD_TYPE="release"
export CVI_BUILD_MODULE="."
export CVI_BUILD_PLATFORM="x86"
export CVI_BUILD_TARGET="install"
export CVI_CLEAN_REBUILD=0

export CVI_BUILD_SYSTEM="linux"
export CVI_BUILD_TOOLCHAIN=
export CVI_CMAKE_BUILD_TYPE=${CVI_CMAKE_BUILD_TYPE_MAP[${CVI_BUILD_TYPE}]}

function print_usage() {
    local IFS='/'
    local prog
    prog="$(basename "$0")"
    echo "Usage:"
    echo "${TAB}${prog} [options]"
    echo "Available options:"
    echo "${TAB}-b,--build    Currently ${SUPPORTED_BUILD_TYPE[*]} types are supported, default: ${CVI_BUILD_TYPE}"
    echo "${TAB}-m,--module   Module that need to be compiled, default: ${CVI_BUILD_MODULE}"
    echo "${TAB}-t,--target   Build target that requires compiling the module, default: ${CVI_BUILD_TARGET}"
    echo "${TAB}-p,--platform Currently ${SUPPORTED_BUILD_PLATFORM[*]} platforms are supported, default: ${CVI_BUILD_PLATFORM}"
    echo "${TAB}-r,--rebuild  Clean and rebuild, default: ${CVI_CLEAN_REBUILD}"
    echo "${TAB}-h,--help     Show this message and exit"
}

function check_opt_arg() {
    local opt="$1"
    local arg="$2"
    if [[ -z "${arg}" || "${arg}" =~ ^-.* ]]; then
        echo "Argument missing for option ${opt}, exiting..."
        exit 1
    fi
}

function parse_arguments() {
    if [[ "$1" == "--help" ]]; then
        print_usage
        exit 0
    fi

    while [[ $# -gt 0 ]]; do
        local opt="$1"
        shift
        case $opt in
        -b | --build)
            check_opt_arg "${opt}" "$1"
            CVI_BUILD_TYPE="$1"
            shift
            ;;
        -m | --module)
            check_opt_arg "${opt}" "$1"
            CVI_BUILD_MODULE="$1"
            shift
            ;;
        -t | --target)
            check_opt_arg "${opt}" "$1"
            CVI_BUILD_TARGET="$1"
            shift
            ;;
        -p | --platform)
            check_opt_arg "${opt}" "$1"
            CVI_BUILD_PLATFORM="$1"
            shift
            ;;
        -r | --rebuild)
            CVI_CLEAN_REBUILD=1
            ;;
        -h | --help)
            print_usage
            exit 0
            ;;
        *)
            echo "Unknown option: ${opt}"
            print_usage
            exit 1
            ;;
        esac
    done

    CVI_INSTALL_PREFIX="$CVI_BUILD_PROJECT_PATH/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}"
}

function build_type_support_check() {
    for entry in "${SUPPORTED_BUILD_TYPE[@]}"; do
        if [[ "${entry}" == "${CVI_BUILD_TYPE}" ]]; then
            return
        fi
    done

    echo "Unsupported build type: ${CVI_BUILD_TYPE}, exiting..."
    exit 1
}

function build_platform_support_check() {
    for entry in "${SUPPORTED_BUILD_PLATFORM[@]}"; do
        if [[ "${entry}" == "${CVI_BUILD_PLATFORM}" ]]; then
            return
        fi
    done

    echo "Unsupported build platform: ${CVI_BUILD_PLATFORM}, exiting..."
    exit 1
}

function check_arguments() {
    build_type_support_check
    build_platform_support_check
}

function setup_environment() {
    CVI_CMAKE_BUILD_TYPE=${CVI_CMAKE_BUILD_TYPE_MAP[${CVI_BUILD_TYPE}]}

    if [[ ${CVI_BUILD_PLATFORM} != "x86" ]]; then
        CVI_BUILD_TOOLCHAIN=${CVI_BUILD_PROJECT_PATH}/cmake_setting/cmake/${CVI_BUILD_PLATFORM}_toolchain_config.cmake

        # shellcheck source=/dev/null
        source "${CVI_BUILD_PROJECT_PATH}/cmake_setting/script/env/${CVI_BUILD_PLATFORM}_env.sh"
    fi
}
