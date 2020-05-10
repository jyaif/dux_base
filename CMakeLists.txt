cmake_minimum_required (VERSION 2.6)
project(dux_base)

# for tests:
include_directories("src")

set(CMAKE_CXX_FLAGS "-std=c++17 -Werror")

if (APPLE)
# I didn't manage to have libc++ with thread safety annotations on linux, so
# only enabling them on macOS.
add_compile_options("-D _LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS")
add_compile_options("-Wthread-safety")
endif

add_executable(
    dux_base_test_bin
    src/background_task_runner.cpp
    src/background_task_runner.h
    src/base_type_wrapper.h
    src/crc32.cpp
    src/crc32.h
    src/fps_counter.cpp
    src/fps_counter.h
    src/interpolate.cpp
    src/interpolate.h
    src/macros.h
    src/scope_guard.cpp
    src/scope_guard.h
    src/stream.cpp
    src/stream.h
    src/task_runner.cpp
    src/task_runner.h
    src/timer.cpp
    src/timer.h
    src/value.cpp
    src/value.h
    src/value_serialization.cpp
    src/value_serialization.h
    test/background_task_runner_test.cpp
    test/background_task_runner_test.h
    test/copyable_lambda_test.cpp
    test/copyable_lambda_test.h
    test/observer_list_test.cpp
    test/observer_list_test.h
    test/test.cpp
    test/value_test.cpp
    test/value_test.h
)

target_link_libraries(dux_base_test_bin pthread)
