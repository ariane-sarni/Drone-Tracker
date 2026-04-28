cmake_minimum_required(VERSION 3.16)

project(Counter-UAS)

add_executable(Counter-UAS src/detection.cpp)

# 1. Point the compiler directly to the system header directory
target_include_directories(Counter-UAS PRIVATE /usr/include/libcamera)

# 2. Link against the camera library
target_link_libraries(Counter-UAS PRIVATE camera)