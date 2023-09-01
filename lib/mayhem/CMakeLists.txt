cmake_minimum_required(VERSION 3.25)

project(mayhem)
set(CMAKE_CXX_STANDARD 20)

add_library(mayhem STATIC src/mayhem.cpp)

target_link_libraries(mayhem glad glfw glm)