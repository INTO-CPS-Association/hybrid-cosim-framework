cmake_minimum_required (VERSION 2.8.5)
project (hcf C CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

add_subdirectory (third_party/stability-test)
add_subdirectory (semantic-adaptation)
add_subdirectory (window-sa)

