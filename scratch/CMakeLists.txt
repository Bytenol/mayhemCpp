cmake_minimum_required(VERSION 3.25)

project(Bytenol_Showcase_Scratch)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

add_executable(scratch2 scratch2.cpp)