cmake_minimum_required(VERSION 2.6)

PROJECT(topicAdaptation)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)
#SET( CMAKE_CXX_FLAGS " -Wno-deprecated " CACHE STRING "Flags used by the C++ compiler" FORCE )
SET (CMAKE_CXX_FLAGS " -fopenmp ")

#add_definitions(-DCMAKE_INSTALL_PREFIX=/lium/buster1/servan/Programmation/TERCpp)
add_definitions(-DIBPP_LINUX -DIBPP_GCC -DINSTANTIATE_TEMPLATES )
# include_directories(/lium/buster1/servan/Tools/include)
# link_directories(/lium/buster1/servan/Tools/lib)

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O2  -DINSTANTIATE_TEMPLATES )

#list all source files here
# add_subdirectory(src)
add_subdirectory(test)

#ADD_EXECUTABLE(tercpp main.cpp)

#install(TARGETS tercpp DESTINATION bin)

#need to link to some other libraries ? just add them here
#TARGET_LINK_LIBRARIES(tercpp png jpeg)
 
