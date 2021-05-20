cmake_minimum_required (VERSION 3.6)
project(dux_base_project)

add_compile_options(-std=c++17 -Werror)

add_subdirectory(../ dux_base_lib_build_dir)

if (APPLE)
# I didn't manage to have libc++ with thread safety annotations on linux, so
# only enabling them on macOS.
add_compile_options("-D _LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS")
add_compile_options("-Wthread-safety")
endif()

add_executable(
  dux_base_test
  background_task_runner_test.cpp
  background_task_runner_test.h
  copyable_lambda_test.cpp
  copyable_lambda_test.h
  interpolate_test.cpp
  interpolate_test.h
  observer_list_test.cpp
  observer_list_test.h
  string_utils_test.cpp
  string_utils_test.h
  test.cpp
  value_test.cpp
  value_test.h
)

target_link_libraries(dux_base_test PRIVATE dux_base pthread)