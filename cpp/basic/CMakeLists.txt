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

add_executable(
    alias
    alias.cpp
)

add_executable(
    noexcept
    noexcept.cpp
)

add_executable(
    override_final
    override_final.cpp
)

add_executable(
    decltype
    decltype.cpp
)

add_executable(
    lambda
    lambda.cpp
)

add_executable(
    refer
    refer.cpp
)

add_executable(
    move_func
    move_func.cpp
)

add_executable(
    tuple
    tuple.cpp
)

add_executable(
    functional
    functional.cpp
)

add_executable(
    assert
    assert.cpp
)

add_executable(
    using
    using.cpp
)

add_executable(
    macro_test
    macro_test.cpp
)

add_executable(
    operator
    operator.cpp
)

add_executable(
    exception
    exception.cpp
)

add_executable(
    file_stream
    file_stream.cpp
)

add_executable(
    file_op
    file_op.cpp
)

add_executable(
    time_op
    time_op.cpp
)

add_executable(
    type_cast
    type_cast.cpp
)

add_executable(
    smart_pointer
    smart_pointer.cpp
)

install(TARGETS nullptr auto uniform_init explicit default_delete alias noexcept override_final decltype lambda refer move_func tuple functional assert using macro_test operator exception file_stream file_op time_op type_cast smart_pointer RUNTIME DESTINATION bin/cpp/basic)
