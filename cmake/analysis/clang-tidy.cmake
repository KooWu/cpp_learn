find_program(CLANG_TIDY clang-tidy)

if(CLANG_TIDY)
  set(SOURCE_DIRS "demo" "lib")
  # set(SOURCE_DIRS "demo" "lib/static" "lib/shared/python_fun" "lib/shared/shell_fun" "lib/shared/lang_fun" )
  # 找到所有源文件
  file(GLOB_RECURSE SOURCES "")
  foreach(SOURCE_DIR ${SOURCE_DIRS})
      file(GLOB_RECURSE DIR_SOURCES "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.c" "${SOURCE_DIR}/*.h")
      list(APPEND SOURCES ${DIR_SOURCES})
  endforeach()

  add_custom_target(tidy-check
  COMMAND /home/yxw/share/tidy/clang/linux/x64/clang-tidy
  -p=${CMAKE_BINARY_DIR} -header-filter=* ${SOURCES}
 )

  add_custom_target(tidy-fix
  COMMAND /home/yxw/share/tidy/clang/linux/x64/clang-tidy
  -p=${CMAKE_BINARY_DIR} -fix -header-filter=* ${SOURCES}
 )
else()
  message("[WARNING] Clang-tidy is not installed. Clang-tidy targets are disabled.")
endif()
