add_executable(test_json test_json.cpp)
add_executable(test_logging test_logging.cpp)

target_include_directories(test_json PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(test_logging PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(test_logging PRIVATE slam_tools)
