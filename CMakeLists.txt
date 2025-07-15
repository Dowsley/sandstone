cmake_minimum_required(VERSION 3.31)
project(sandstone VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib/raylib)

add_executable(sandstone src/main.cpp
        src/core/simulation.cpp
        src/core/simulation.h
        src/types/Point.cpp
        src/types/Point.h)
target_link_libraries(sandstone PRIVATE raylib)

if(UNIX AND NOT APPLE)
    target_link_libraries(sandstone PRIVATE m pthread dl rt X11)
endif()
