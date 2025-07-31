cmake_minimum_required (VERSION 3.16)

project(sqlite_test)

# 查找 pkg-config
find_package(PkgConfig REQUIRED)

# 使用 pkg_check_modules 宏查找库
pkg_check_modules(SQLITE REQUIRED IMPORTED_TARGET sqlite3)

# 包含库的头文件路径
include_directories(${SQLITE_INCLUDE_DIRS})

# 添加可执行文件
add_executable(sqlite_test sqlite_test.cpp)
add_executable(long_sql long_sqlite.cpp)

# 链接库
target_link_libraries(sqlite_test PRIVATE PkgConfig::SQLITE)
target_link_libraries(long_sql PRIVATE PkgConfig::SQLITE)
install(TARGETS sqlite_test long_sql RUNTIME DESTINATION bin/sqlite_test)
