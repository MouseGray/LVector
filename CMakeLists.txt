cmake_minimum_required(VERSION 3.14)

project(LVector LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(LVector STATIC
  include/lvector_declaration.h
  include/lvector.h
  include/lvector_generator.h

  src/lvector.cpp
  src/lvector_generator.cpp
)

target_compile_definitions(LVector PRIVATE LVECTOR_LIBRARY)
