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


add_executable(
  asymmetric_encrypt_test
  asymmetric_encrypt_test.cpp
)
# target_include_directories(sign_test PRIVATE ${OpenSSL_INCLUDE_DIR})
target_link_libraries(asymmetric_encrypt_test PRIVATE  OpenSSL::Crypto OpenSSL::SSL)
install(TARGETS asymmetric_encrypt_test RUNTIME DESTINATION bin/samples/openssl)

add_executable(
  cert_hld
  cert_hld.cpp
)
# target_include_directories(sign_test PRIVATE ${OpenSSL_INCLUDE_DIR})
target_link_libraries(cert_hld PRIVATE  OpenSSL::Crypto OpenSSL::SSL)
install(TARGETS cert_hld RUNTIME DESTINATION bin/samples/openssl)

add_executable(
  https_cm
  https_cm.cpp
)
# target_include_directories(sign_test PRIVATE ${OpenSSL_INCLUDE_DIR})
target_link_libraries(https_cm PRIVATE  OpenSSL::Crypto OpenSSL::SSL)
install(TARGETS https_cm RUNTIME DESTINATION bin/samples/openssl)