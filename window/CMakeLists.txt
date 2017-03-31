#
# #%~
# The Overture Abstract Syntax Tree
# %%
# Copyright (C) 2017 - 2014 Aarhus University
# %%
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public
# License along with this program.  If not, see
# <http://www.gnu.org/licenses/gpl-3.0.html>.
# #~%
#
cmake_minimum_required (VERSION 2.8.5)
project (window C CXX)

include(CheckCXXCompilerFlag)

#set(CMAKE_VERBOSE_MAKEFILE on)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

add_definitions(-DFMI_COSIMULATION)

include_directories(src)


file(GLOB CPP_FILES src/*.cpp)

file(GLOB CPP_MAIN_FILES src/main*.cpp)

list(REMOVE_ITEM CPP_FILES ${CPP_MAIN_FILES})


add_executable(${PROJECT_NAME}_main src/main.cpp ${CPP_FILES})
target_link_libraries(${PROJECT_NAME}_main hcf)

file(GLOB CPP_TEST_FILES tests/*.cpp)
add_executable(${PROJECT_NAME}_test ${CPP_FILES} ${CPP_TEST_FILES})
target_link_libraries(${PROJECT_NAME}_test hcf)
target_link_libraries(${PROJECT_NAME}_test gtest_main)
add_test(AllTestsIn-${PROJECT_NAME} ${PROJECT_NAME}_test --gtest_output=xml:gtestresults.xml)

foreach(LETTER ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})
	
if(EXISTS "${LETTER}/libstdc++.a")
#   message (STATUS "${LETTER}")
	set(STDCPP_LIBRARY "${LETTER}/libstdc++.a")
else()
#  message (STATUS "${LETTER} ---- not these")
endif()
endforeach()

message(STATUS "Static linking with libstdc++.a in ${STDCPP_LIBRARY}")
#target_link_libraries(${PROJECT_NAME} ${STDCPP_LIBRARY})

if (WIN32)
	#windows	
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
endif() 



