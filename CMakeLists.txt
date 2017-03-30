cmake_minimum_required (VERSION 2.8.5)
project (hcf C CXX)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

if (WIN32)
	#windows, becuase windows just cannot figure out to do it correct. 
	# must be a bug in msys mingw gcc 6.3	
	# it doesnt read the rsp files.
	set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 0)
	set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES 0)
	
	set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_LIBRARIES 0)
	set(CMAKE_C_USE_RESPONSE_FILE_FOR_LIBRARIES 0)
endif() 

add_subdirectory (third_party/stability-test)
add_subdirectory (semantic-adaptation)
add_subdirectory (window-sa)
add_subdirectory (window)
add_subdirectory (windowsa-fullgen)

#add_subdirectory (window-sa-gen)

