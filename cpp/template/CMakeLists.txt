cmake_minimum_required (VERSION 3.16)

project(template)

add_executable(
    basic_template
    basic_template.cpp
)

add_executable(
    var_template
    var_template.cpp
)
# target_compile_features(var_template PRIVATE cxx_std_11)
set_property(TARGET var_template PROPERTY CXX_STANDARD 11)

add_executable(
    var_template_v2
    var_template_v2.cpp
)
# target_compile_features(var_template_v2 PRIVATE cxx_std_11)
set_property(TARGET var_template_v2 PROPERTY CXX_STANDARD 11)

add_executable(
    var_template_c17
    var_template_c17.cpp
)
# target_compile_features(var_template_c17 PRIVATE cxx_std_11)
set_property(TARGET var_template_c17 PROPERTY CXX_STANDARD 17)

add_executable(
    util_template
    util_template.cpp
)

add_executable(
    hyber_template
    hyber_template.cpp
)
install(TARGETS hyber_template util_template basic_template var_template var_template_v2 var_template_c17 RUNTIME DESTINATION bin/cpp/template)
