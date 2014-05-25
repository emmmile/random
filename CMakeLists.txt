cmake_minimum_required(VERSION 2.8)
project(random)

include(ExternalProject)
add_definitions(-std=c++11)
set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O4 -mtune=native -ffast-math -funroll-loops" )
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS}")

ExternalProject_Add(timer
    PREFIX "timer"
    GIT_REPOSITORY https://github.com/emmmile/timer.git
    DOWNLOAD_DIR "."
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD 1
)


ExternalProject_Get_Property(timer SOURCE_DIR)

include_directories(${SOURCE_DIR})


add_executable(test test.cpp)

add_dependencies(test timer)
target_link_libraries(test)
