cmake_minimum_required (VERSION 3.16)

project(hello)

add_executable(
  hello
  hello.cpp
)
target_link_libraries(hello PRIVATE pthread)
install(TARGETS hello RUNTIME DESTINATION bin/common/hello)
