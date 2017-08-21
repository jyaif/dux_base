cmake_minimum_required (VERSION 2.6)
project(dux_base)

# for tests:
include_directories("src")

set(CMAKE_CXX_FLAGS "-std=c++14 -Werror")

add_executable(
	dux_base_test_bin
	test/test.cpp
	src/scoped_lambda.cpp
	src/stream.cpp
	src/timer.cpp
)

target_link_libraries(dux_base_test_bin pthread)