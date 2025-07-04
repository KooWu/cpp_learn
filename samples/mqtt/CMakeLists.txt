cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(
    mqtt_test
    VERSION 1.0.0
    LANGUAGES CXX
)

find_package(OpenSSL REQUIRED)


# 添加可执行文件
add_executable(mqtt_test cpp/mqtt_test.cpp)

# 包含库的头文件路径
target_include_directories(mqtt_test PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/include/)
target_link_libraries(mqtt_test PRIVATE ${CVI_PRJ_PATH}/install/${CVI_BUILD_PLATFORM}_sysroot/${CVI_BUILD_TYPE}/lib/libmosquitto.so)
install(TARGETS mqtt_test RUNTIME DESTINATION bin/mqtt)
install(FILES etc/broker.config DESTINATION bin/mqtt)
