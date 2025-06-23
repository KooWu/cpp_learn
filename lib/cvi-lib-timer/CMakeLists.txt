cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(
    cvi-lib-timer
    VERSION 1.0.0
    LANGUAGES CXX C
)

option(ENABLE_TEST "compiler test executable" ON)

add_definitions(-Wall)
add_definitions("-fPIC -Wno-deprecated -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

add_library(
    ${PROJECT_NAME}
    SHARED
    src/accurate_timer.cpp
    src/accurate_timer_interface.cpp
    src/normal_timer.cpp
    src/normal_timer_interface.cpp 
)

target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
target_include_directories(
    ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include/cvi/timer>
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
    NO_SET_AND_CHECK_MACRO
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    COMPATIBILITY ExactVersion
)

install(FILES "${PROJECT_SOURCE_DIR}/include/accurate_timer_interface.h"
              "${PROJECT_SOURCE_DIR}/include/accurate_timer.h"
              "${PROJECT_SOURCE_DIR}/include/normal_timer_interface.h"
              "${PROJECT_SOURCE_DIR}/include/normal_timer.h"
        DESTINATION include/cvi/timer)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
        DESTINATION lib/cmake/${PROJECT_NAME}
)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}_targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(EXPORT ${PROJECT_NAME}_targets
        DESTINATION lib/cmake/${PROJECT_NAME}
)

if(ENABLE_TEST)
    add_executable(cpp_timer_test
        test/cpp_timer_test.cpp
    )
    target_link_libraries(cpp_timer_test PRIVATE ${PROJECT_NAME})
    install(TARGETS cpp_timer_test RUNTIME DESTINATION bin/cvi-lib-timer)

    add_executable(thread_timer_test
        test/timer_thread_test.cpp
    )
    target_link_libraries(thread_timer_test PRIVATE ${PROJECT_NAME})
    install(TARGETS thread_timer_test RUNTIME DESTINATION bin/cvi-lib-timer)

    add_executable(normal_timer_gtest
        test/normal_timer_test.cpp
    )
    target_link_libraries(normal_timer_gtest PRIVATE ${PROJECT_NAME})
    install(TARGETS normal_timer_gtest RUNTIME DESTINATION bin/cvi-lib-timer)
endif()
