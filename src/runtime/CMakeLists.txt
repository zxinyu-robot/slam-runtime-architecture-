add_library(slam_runtime
  spatial_runtime.cpp
  state_store.cpp
)

target_include_directories(slam_runtime
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_compile_features(slam_runtime PUBLIC cxx_std_20)
