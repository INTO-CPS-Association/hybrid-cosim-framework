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
project (CtrlMultiRate C CXX)

include(CheckCXXCompilerFlag)

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

include_directories(src-gen)

file(GLOB CPP_FILES src-gen/*.cpp)

file(GLOB CPP_MAIN_FILES src-gen/main*.cpp)

list(REMOVE_ITEM CPP_FILES ${CPP_MAIN_FILES})

## library ##

add_library(${PROJECT_NAME} SHARED ${CPP_FILES} ${CPP_FILES})
target_link_libraries(${PROJECT_NAME} hcf)
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)


## Main executable ##
add_executable(${PROJECT_NAME}_main ${CPP_MAIN_FILES} ${CPP_FILES})
target_link_libraries(${PROJECT_NAME}_main hcf)

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



# I need this: -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic

if (WIN32)
	#windows	
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++ -static-libgcc -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc  -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic")
endif() 




#if(NOT APPLE)
#https://developer.apple.com/legacy/library/documentation/Darwin/Reference/ManPages/man3/pthread.3.html 
#it is included in system
find_library(PTHREAD_LIBRARY NAMES libwinpthread-1.dll)# libpthread.a libpthread.dylib )
message( INFO ${PTHREAD_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${PTHREAD_LIBRARY})
#endif()



#add_custom_command(${PROJECT_NAME}_main POST_BUILD COMMAND "find . -name *.exe -exec echo {} \; -exec bash -c \"objdump -p {} | grep 'DLL Name:'\" \;")




