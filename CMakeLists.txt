cmake_minimum_required(VERSION 3.20)
project(slam LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SLAM_BUILD_EXAMPLES "Build C++ examples" ON)

add_subdirectory(src)
add_subdirectory(pyxxx)

if(SLAM_BUILD_EXAMPLES)
  add_subdirectory(examples/cpp)
endif()
