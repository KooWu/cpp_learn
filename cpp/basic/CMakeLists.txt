cmake_minimum_required (VERSION 3.16)

project(basic)

add_executable(
    nullptr
    nullptr.cpp
)

add_executable(
    auto
    auto.cpp
)


add_executable(
    uniform_init
    uniform_init.cpp
)

add_executable(
    explicit
    explicit.cpp
)

add_executable(
    default_delete
    default_delete.cpp
)

install(TARGETS nullptr auto uniform_init explicit default_delete RUNTIME DESTINATION bin/cpp/basic)
