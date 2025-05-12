find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
  set(
    SOURCES_TO_CHECK 
    "${CMAKE_SOURCE_DIR}/demo/*.cpp"
    "${CMAKE_SOURCE_DIR}/lib/static/*.cpp"
    "${CMAKE_SOURCE_DIR}/lib/static/*.h"
    "${CMAKE_SOURCE_DIR}/lib/shared/shell_fun/*.cpp"
    "${CMAKE_SOURCE_DIR}/lib/shared/shell_fun/*.h"
    "${CMAKE_SOURCE_DIR}/lib/shared/lang_fun/*.cpp"
    "${CMAKE_SOURCE_DIR}/lib/shared/lang_fun/*.h"
    )

  add_custom_target(format-check
  COMMAND /home/yxw/share/check/clang-format
  ${SOURCES_TO_CHECK}
  --dry-run -style=file -i -fallback-style=none
 )

 add_custom_target(format-fix
 COMMAND /home/yxw/share/check/clang-format
 ${SOURCES_TO_CHECK}
 -style=file -i -fallback-style=none
)
else()
  message("[WARNING] Clang-format is not installed. Formatting targets are disabled.")
endif()
