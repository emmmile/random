cmake_minimum_required(VERSION 2.8)
project(random)
add_definitions(-std=c++0x)
set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -mtune=native -ffast-math -funroll-loops" )
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS}")

#ExternalProject_Add(libtimer
#    PREFIX "timer"
#    GIT_REPOSITORY https://github.com/emmmile/timer.git
#    DOWNLOAD_DIR "."
#    UPDATE_COMMAND ""
#    #CONFIGURE_COMMAND ""
#    #BUILD_COMMAND ""
#    BUILD_IN_SOURCE 1
#    INSTALL_COMMAND ""
#    LOG_DOWNLOAD 1
#)

#ExternalProject_Get_Property(libtimer SOURCE_DIR)
#include_directories(${SOURCE_DIR})
#link_directories(${SOURCE_DIR})
#add_dependencies(test libtimer)

include_directories(${PROJECT_SOURCE_DIR}/timer)
add_subdirectory(${PROJECT_SOURCE_DIR}/timer)
link_directories(${PROJECT_SOURCE_DIR}/timer)

add_executable(performance performance.cpp)

target_link_libraries(performance timer)


#enable_testing()
#add_test(performance test)
