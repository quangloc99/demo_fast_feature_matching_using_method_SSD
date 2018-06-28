cmake_minimum_required(VERSION 2.8)
project(DisplayImage)

find_package( OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_LIBS})

add_library(FastFeatureMatcher FastFeatureMatcher.cpp)
add_library(BallTree BallTree.cpp)

add_executable(main main.cpp)
target_link_libraries(FastFeatureMatcher BallTree)
target_link_libraries(main ${OpenCV_LIBS} FastFeatureMatcher)
