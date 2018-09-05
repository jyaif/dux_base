cmake_minimum_required (VERSION 2.6)
project(dux_base)

# for tests:
include_directories("src")

set(CMAKE_CXX_FLAGS "-std=c++14 -Werror")

add_executable(
	dux_base_test_bin
	test/test.cpp
	test/value_test.cpp
	src/fps_counter.cpp
	src/interpolate.cpp
	src/scoped_lambda.cpp
	src/stream.cpp
	src/task_runner.cpp
	src/timer.cpp
	src/value.cpp
	src/value_serialization.cpp
)

target_link_libraries(dux_base_test_bin pthread)