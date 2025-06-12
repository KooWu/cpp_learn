cmake_minimum_required (VERSION 3.16)

project(samples_thirdparty)

find_package(tomlplusplus)
add_executable(
  tomlplusplus_test
  tomlplusplus_test.cpp
)
target_link_libraries(tomlplusplus_test PRIVATE tomlplusplus::tomlplusplus)

find_package(spdlog)
add_executable(
  spdlog_test
  spdlog_test.cpp
)
target_link_libraries(spdlog_test PRIVATE spdlog::spdlog)
install(TARGETS tomlplusplus_test spdlog_test RUNTIME DESTINATION bin/samples/thirdparty)
