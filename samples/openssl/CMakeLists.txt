cmake_minimum_required (VERSION 3.16)

project(openssl_sample)

find_package(OpenSSL REQUIRED)

if(OpenSSL_FOUND)
    message(STATUS "Found OpenSSL ${OpenSSL_VERSION}")
    message(STATUS "OpenSSL include dir: ${OpenSSL_INCLUDE_DIR}")
    message(STATUS "OpenSSL libraries: ${OpenSSL_LIBRARIES}")
endif()

add_executable(
  sign_test
  sign_test.cpp
)
# target_include_directories(sign_test PRIVATE ${OpenSSL_INCLUDE_DIR})
target_link_libraries(sign_test PRIVATE  OpenSSL::Crypto OpenSSL::SSL)
install(TARGETS sign_test RUNTIME DESTINATION bin/samples/openssl)

add_executable(
  encrypt_test
  encrypt_test.cpp
)
# target_include_directories(sign_test PRIVATE ${OpenSSL_INCLUDE_DIR})
target_link_libraries(encrypt_test PRIVATE  OpenSSL::Crypto OpenSSL::SSL)
install(TARGETS encrypt_test RUNTIME DESTINATION bin/samples/openssl)