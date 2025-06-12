cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(
    cvi-lib-log
    VERSION 1.0.0
    LANGUAGES CXX C
)

option(ENABLE_TEST "compiler test executable" ON)

add_definitions(-Wall)
add_definitions("-fPIC -Wno-deprecated -Werror -Wno-error=stringop-truncation")

set(CMAKE_CXX_STANDARD 17)

find_package(tomlplusplus REQUIRED)
find_package(spdlog REQUIRED)

add_library(
    ${PROJECT_NAME}
    SHARED
    src/log_cfg.cpp 
    src/log_service.cpp 
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include/cvi/log>
)

target_link_libraries(${PROJECT_NAME} spdlog::spdlog)

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

install(FILES "${PROJECT_SOURCE_DIR}/include/log_service.h"
        DESTINATION include/cvi/log)

install(FILES "${PROJECT_SOURCE_DIR}/include/MWPLog.h"
        DESTINATION include/)

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
    add_executable(cpplog_test
        test/test_log_main.cpp
    )
    target_link_libraries(cpplog_test PRIVATE ${PROJECT_NAME})
    target_link_libraries(cpplog_test PRIVATE spdlog::spdlog)
    install(TARGETS cpplog_test RUNTIME DESTINATION bin/cvi-lib-log)

    add_executable(clog_test
        test/test_clog.c
    )
    target_link_libraries(clog_test PRIVATE ${PROJECT_NAME})
    target_link_libraries(clog_test PRIVATE spdlog::spdlog)
    install(TARGETS clog_test RUNTIME DESTINATION bin/cvi-lib-log)
endif()
