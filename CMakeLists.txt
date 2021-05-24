set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
  dux_base
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
  src/murmurhash3.h
  src/murmurhash3.cpp
  src/scope_guard.cpp
  src/scope_guard.h
  src/stream.cpp
  src/stream.h
  src/string_utils.cpp
  src/string_utils.h
  src/task_runner.cpp
  src/task_runner.h
  
  
  src/value.cpp
  src/value.h
  src/value_from_to.cpp
  src/value_from_to.h
  src/value_serialization.cpp
  src/value_serialization.h
)

source_group(src/.*)

target_include_directories(dux_base PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")