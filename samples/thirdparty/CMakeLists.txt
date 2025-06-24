cmake_minimum_required (VERSION 3.16)

project(samples_thirdparty)

find_package(tomlplusplus REQUIRED)
add_executable(
  tomlplusplus_test
  tomlplusplus_test.cpp
)
target_link_libraries(tomlplusplus_test PRIVATE tomlplusplus::tomlplusplus)

find_package(spdlog REQUIRED)
add_executable(
  spdlog_test
  spdlog_test.cpp
)
target_link_libraries(spdlog_test PRIVATE spdlog::spdlog)

find_package(GTest REQUIRED)
add_executable(
  googletest_test
  googletest_test.cpp
)
target_link_libraries(googletest_test PRIVATE GTest::GTest)

find_package(benchmark REQUIRED)
add_executable(
  benchmark_test
  benchmark_test.cpp
)
target_link_libraries(benchmark_test PRIVATE benchmark::benchmark)
target_link_libraries(benchmark_test PRIVATE GTest::GTest)

find_package(nameof REQUIRED)
add_executable(
  nameof_test
  nameof_test.cpp
)
target_link_libraries(nameof_test PRIVATE nameof::nameof)

add_executable(
  jemalloc_test
  jemalloc_test.cpp
)
target_compile_options(jemalloc_test PRIVATE -g -O0)
target_include_directories(jemalloc_test PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/include/)
target_link_libraries(jemalloc_test PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/lib/libjemalloc.so)


add_executable(
  gperftools_test
  gperftools_test.cpp
)
target_compile_options(gperftools_test PRIVATE -g -O0)
target_include_directories(gperftools_test PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/include/)
target_link_libraries(gperftools_test PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/lib/libtcmalloc.so)
target_link_libraries(gperftools_test PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/lib/libprofiler.so)

add_executable(
  libunwind_test
  libunwind_test.cpp
)
target_include_directories(libunwind_test PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/include/)
target_link_libraries(libunwind_test PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/lib/libunwind.so)
target_link_libraries(libunwind_test PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/lib/libunwind-x86_64.so)

install(TARGETS tomlplusplus_test spdlog_test googletest_test benchmark_test nameof_test jemalloc_test gperftools_test libunwind_test RUNTIME DESTINATION bin/samples/thirdparty)

find_package(protobuf REQUIRED)
add_compile_definitions(CONFIG_PROTOBUF_4_0_X)
add_executable(
    fdbus_client
    fdbus/client.cpp
    fdbus/proto/test.pb.cc
    fdbus/proto/test.pb.h
)
target_include_directories(
  fdbus_client PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/usr/include/
)
target_include_directories(
  fdbus_client PRIVATE fdbus/proto
)

target_link_libraries(fdbus_client PRIVATE protobuf::libprotobuf)
target_link_libraries(fdbus_client PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/usr/lib/libfdbus.so)

add_executable(
  fdbus_server
  fdbus/server.cpp
  fdbus/proto/test.pb.cc
  fdbus/proto/test.pb.h
)

target_include_directories(
  fdbus_server PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/usr/include/
)
target_include_directories(
  fdbus_server PRIVATE fdbus/proto
)

target_link_libraries(fdbus_server PRIVATE protobuf::libprotobuf)
target_link_libraries(fdbus_server PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/usr/lib/libfdbus.so)

install(TARGETS fdbus_server fdbus_client RUNTIME DESTINATION bin/samples/thirdparty)