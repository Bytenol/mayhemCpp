cmake_minimum_required(VERSION 3.25)

project(Bytenol_Showcase_LearnOpengl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/project/small)

add_executable(triangle triangle.cpp)
target_link_libraries(triangle mayhem)