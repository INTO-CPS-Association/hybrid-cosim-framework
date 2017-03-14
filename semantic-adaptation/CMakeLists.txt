cmake_minimum_required (VERSION 2.8.5)
project (hcf C CXX)

include(CheckCXXCompilerFlag)

set(CMAKE_VERBOSE_MAKEFILE on)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFMI_COSIMULATION -O0 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFMI_COSIMULATION -O0 -g")



#get_filename_component(FMI_INCLUDE_DIR "../third_party/fmi/include" ABSOLUTE)

#include_directories(${FMI_INCLUDE_DIR} src)
include_directories(src)

add_definitions(-DFMI_COSIMULATION)

file(GLOB C_FILES src/*.c)
file(GLOB CPP_FILES src/*.cpp)

add_library(${PROJECT_NAME} ${C_FILES} ${CPP_FILES})

target_link_libraries(${PROJECT_NAME} fmu)
target_include_directories(${PROJECT_NAME} PUBLIC src)

set_target_properties(${PROJECT_NAME}
                      PROPERTIES POSITION_INDEPENDENT_CODE ON)



