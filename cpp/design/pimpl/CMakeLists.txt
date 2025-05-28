cmake_minimum_required (VERSION 3.16)

project(pimpl)

add_executable(
    pimpl_main
    pimpl_main.cpp
    string_wrapper.cpp
)

install(TARGETS pimpl_main RUNTIME DESTINATION bin/cpp/design)
