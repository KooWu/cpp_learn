cmake_minimum_required (VERSION 3.16)

project(class)

add_executable(
    class_size
    class_size.cpp
)

add_executable(
    single_inherit
    single_inherit.cpp
)

add_executable(
    multi_inherit
    multi_inherit.cpp
)

add_executable(
    diamond_inherit
    diamond_inherit.cpp
)

install(TARGETS class_size single_inherit multi_inherit diamond_inherit RUNTIME DESTINATION bin/cpp/class)
