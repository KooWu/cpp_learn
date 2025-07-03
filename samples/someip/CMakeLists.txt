cmake_minimum_required (VERSION 3.16)

project(someip)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)

set(server_src
    server/server.cpp
    src_gen/soa/sample/CustomTypesSomeIPDeployment.cpp
    src_gen/v1//soa/sample/TestSrvIfSomeIPStubAdapter.cpp
    src_gen/v1/soa/sample/TestSrvIfSomeIPDeployment.cpp
)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system thread filesystem REQUIRED)
find_package(CommonAPI 3.2.0 REQUIRED CONFIG NO_CMAKE_PACKAGE_REGISTRY)
find_package(CommonAPI-SomeIP 3.2.0 REQUIRED)
find_package(vsomeip3 3.3.8 REQUIRED)
find_package(automotive-dlt REQUIRED)

add_executable(soa_server ${server_src})

target_include_directories(soa_server PRIVATE src_gen)
target_link_libraries(soa_server PRIVATE CommonAPI CommonAPI-SomeIP vsomeip3 Boost::system Boost::thread Boost::filesystem Genivi::dlt)

install(TARGETS soa_server RUNTIME DESTINATION bin/someip/)

set(client_src
    client/client.cpp
    src_gen/soa/sample/CustomTypesSomeIPDeployment.cpp
    src_gen/v1/soa/sample/TestSrvIfSomeIPProxy.cpp
    src_gen/v1/soa/sample/TestSrvIfSomeIPDeployment.cpp
)

add_executable(soa_client ${client_src})

target_include_directories(soa_client PRIVATE src_gen)
target_link_libraries(soa_client PRIVATE pthread CommonAPI CommonAPI-SomeIP vsomeip3 Boost::system Boost::thread Boost::filesystem Genivi::dlt)

install(TARGETS soa_client RUNTIME DESTINATION bin/someip/)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/script/ DESTINATION bin/someip/ FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)