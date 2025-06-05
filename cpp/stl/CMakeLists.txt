cmake_minimum_required (VERSION 3.16)

project(stl)

add_executable(
    array
    array.cpp
)

add_executable(
    vector
    vector.cpp
)

add_executable(
    list
    list.cpp
)

add_executable(
    deque
    deque.cpp
)

add_executable(
    map
    map.cpp
)

add_executable(
    set_test
    set.cpp
)

add_executable(
    unordered_map
    unordered_map.cpp
)

add_executable(
    unordered_set
    unordered_set.cpp
)

add_executable(
    stack
    stack.cpp
)

add_executable(
    queue
    queue.cpp
)

add_executable(
    priority_queue
    priority_queue.cpp
)

install(TARGETS array vector list map deque set_test unordered_map unordered_set stack queue priority_queue RUNTIME DESTINATION bin/cpp/stl)
