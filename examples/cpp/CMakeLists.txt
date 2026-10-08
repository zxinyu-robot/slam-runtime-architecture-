add_executable(test_json test_json.cpp)
add_executable(test_logging test_logging.cpp)
add_executable(test_spatial_runtime test_spatial_runtime.cpp)

target_include_directories(test_json PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(test_logging PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(test_spatial_runtime PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(test_logging PRIVATE slam_tools)
target_link_libraries(test_spatial_runtime PRIVATE slam_runtime)

if(BUILD_TESTING)
  add_test(NAME spatial_runtime COMMAND test_spatial_runtime)
endif()
