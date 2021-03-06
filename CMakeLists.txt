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
project (hcf C CXX)

include(third_party/clang-format.cmake)

enable_testing()

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

#add_definitions(-DSA_DEBUG) #Enable debugging

if (WIN32)
	#windows, becuase windows just cannot figure out to do it correct. 
	# must be a bug in msys mingw gcc 6.3	
	# it doesnt read the rsp files.
	set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 0)
	set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES 0)
	
	set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_LIBRARIES 0)
	set(CMAKE_C_USE_RESPONSE_FILE_FOR_LIBRARIES 0)
endif() 

add_subdirectory(third_party/googletest/googletest)
add_subdirectory (semantic-adaptation)
add_subdirectory (semantic_adaptation_samples/SensorMultiRate)
add_subdirectory (semantic_adaptation_samples/PlantMultiRate)
add_subdirectory (semantic_adaptation_samples/CtrlMultiRate)
add_subdirectory (semantic_adaptation_samples/SensorPlantMultiRate)
add_subdirectory (semantic_adaptation_samples/CtrlProactive)
