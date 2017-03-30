cmake_minimum_required (VERSION 2.8.5)
project (windowsa-fullgen C CXX)

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

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFMI_COSIMULATION -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFMI_COSIMULATION -g")



include_directories(src)


file(GLOB CPP_FILES src/*.cpp)

file(GLOB CPP_MAIN_FILES src/main*.cpp)

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
find_library(PTHREAD_LIBRARY NAMES libwinpthread.dll.a)# libpthread.a libpthread.dylib )
#message( INFO ${PTHREAD_LIBRARY})
#target_link_libraries(${PROJECT_NAME}_main ${PTHREAD_LIBRARY} /C/msys64/mingw64/x86_64-w64-mingw32/lib/libwinpthread.a)
#endif()



#add_custom_command(${PROJECT_NAME}_main POST_BUILD COMMAND "find . -name *.exe -exec echo {} \; -exec bash -c \"objdump -p {} | grep 'DLL Name:'\" \;")




