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

add_executable(
    this_pointer
    this_pointer.cpp
)

add_executable(
    abstract_class
    abstract_class.cpp
)

add_executable(
    rtti
    rtti.cpp
)

install(TARGETS class_size single_inherit multi_inherit diamond_inherit this_pointer abstract_class rtti RUNTIME DESTINATION bin/cpp/class)
