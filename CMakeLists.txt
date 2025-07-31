cmake_minimum_required(VERSION 3.16)
#cmake_minimum_required(VERSION <major>[.<minor>[.<patch>[.<tweak>]]] [FATAL_ERROR])
#指定cmake版本的最低要求

project(cpp_learn VERSION 1.0 LANGUAGES CXX)
#project(<project_name> [VERSION <version>] [LANGUAGES <language>...])
#指定工程名、版本、语言类型

include(cmake/analysis/clang-tidy.cmake)
include(cmake/format/clang-format.cmake)
# sudo apt install doxygen
# sudo apt install graphviz
include(cmake/documentation/doxygen.cmake)

add_subdirectory(lib/cvi-lib-log)
# add_subdirectory(lib/shared/python_fun)
# add_subdirectory(lib/shared/shell_fun)
# add_subdirectory(lib/shared/lang_fun)

# add_subdirectory(lib/shared/ad_utility)
# add_subdirectory(lib/static)
# add_subdirectory(demo)

# enable_testing()
# add_subdirectory(test/lib/static)

# include(packaging.cmake)
