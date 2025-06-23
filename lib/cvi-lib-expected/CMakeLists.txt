cmake_minimum_required(VERSION 3.14)
project(cvi-lib-expected
  DESCRIPTION "C++11/14/17 std::expected with functional-style extensions"
  VERSION 1.0.0
  LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(CMakeDependentOption)
include(GNUInstallDirs)
find_package(GTest REQUIRED)

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

cmake_dependent_option(EXPECTED_BUILD_TESTS
  "Enable cvi::expected tests" OFF
  "BUILD_TESTING" ON)

add_library(cvi-lib-expected INTERFACE)
target_include_directories(cvi-lib-expected
  INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/cvi/expected>)

# Installation help
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
  INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}")

write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  COMPATIBILITY SameMajorVersion
  ARCH_INDEPENDENT)

install(TARGETS cvi-lib-expected
  EXPORT ${PROJECT_NAME}-targets
  INCLUDES DESTINATION lib)

install(EXPORT ${PROJECT_NAME}-targets
  DESTINATION "lib/cmake/${PROJECT_NAME}"
  FILE "${PROJECT_NAME}-targets.cmake")

install(FILES
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
  DESTINATION "lib/cmake/${PROJECT_NAME}")

file(GLOB HEADER_FILES "${PROJECT_SOURCE_DIR}/include/*.h")
install(FILES ${HEADER_FILES} DESTINATION include/cvi/expected)

if(EXPECTED_BUILD_TESTS)
  add_executable(test_expected test/test_expected.cpp)
  target_link_libraries(test_expected PRIVATE cvi-lib-expected)
  target_link_libraries(test_expected PRIVATE GTest::GTest)
  install(TARGETS test_expected RUNTIME DESTINATION bin/test_expected)
endif()
