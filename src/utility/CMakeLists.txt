add_library(slam_utility
  base.cpp
)

target_include_directories(slam_utility
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)
