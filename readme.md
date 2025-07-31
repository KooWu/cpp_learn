# 仓库说明

用于记录cpp的学习&开源库学习&实现自己3方库

# 目录说明

## cmake
    代码检查/文档生成/交叉编译等cmake配置
## script
    编译脚本依赖目录
## samples
    例子目录
    thirdparty：3方库例子
    lib：自研库例子
## thirdparty
   3方库目录
## cpp
    c++学习目录
## lib
    自研库目录
## tool
    工具仓

# 如何生成文档
./build.sh -m . -t docs

# 执行时环境变量
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../../lib

