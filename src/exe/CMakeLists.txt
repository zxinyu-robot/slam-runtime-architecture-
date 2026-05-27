add_executable(slam_app
  main.cpp
  gui.cpp
)

target_include_directories(slam_app
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(slam_app
  PRIVATE
    slam_utility
    slam_tools
)
