add_library(slam_tools
  logging.cpp
  lvo.cpp
)

target_include_directories(slam_tools
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(slam_tools
  PUBLIC
    slam_utility
)
