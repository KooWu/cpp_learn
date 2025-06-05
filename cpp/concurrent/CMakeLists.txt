cmake_minimum_required (VERSION 3.16)

project(concurrent)

add_executable(
    thread
    thread.cpp
)
set_property(TARGET thread PROPERTY CXX_STANDARD 20)

add_executable(
    mutex
    mutex.cpp
)

add_executable(
    condition
    condition.cpp
)

add_executable(
    sem
    sem.cpp
)
set_property(TARGET sem PROPERTY CXX_STANDARD 20)

add_executable(
    atomic
    atomic.cpp
)

add_executable(
    async
    async.cpp
)
set_property(TARGET async PROPERTY CXX_STANDARD 20)

add_executable(
    practical
    practical.cpp
)

install(TARGETS thread mutex condition sem atomic async practical RUNTIME DESTINATION bin/cpp/concurrent)
