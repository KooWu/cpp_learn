cmake_minimum_required (VERSION 3.16)

project(samples_work)

option(ENABLE_TC "ENABLE_TC" OFF)

add_executable(
  xxmalloc_test
  xxmalloc_test.cpp
)

if (ENABLE_TC)
    add_definitions("-DENABLE_TC_DUMP")
endif()

target_compile_options(xxmalloc_test PRIVATE -g -O0)
target_include_directories(xxmalloc_test PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/include/)
if (ENABLE_TC)
target_link_libraries(xxmalloc_test PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/lib/libtcmalloc.so)
target_link_libraries(xxmalloc_test PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/lib/libprofiler.so)
else()
target_link_libraries(xxmalloc_test PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/lib/libjemalloc.so)
endif()

install(TARGETS xxmalloc_test RUNTIME DESTINATION bin/samples/work)

# 设置默认值为 "all"
if(NOT DEFINED MY_VARIABLE)
    set(MY_VARIABLE "all" CACHE STRING "A variable that can be set to 'all' or other values.")
endif()

# 输出变量值以确认
message(STATUS "MY_VARIABLE=${MY_VARIABLE}")

# 使用该变量
add_executable(cmake_test cmake_test.cpp)
target_compile_definitions(cmake_test PRIVATE MY_VARIABLE="${MY_VARIABLE}")
install(TARGETS cmake_test RUNTIME DESTINATION bin/samples/work)
