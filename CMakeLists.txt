cmake_minimum_required(VERSION 3.25)
project(Mayhem_CPP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)


find_package(OpenGL REQUIRED)

add_subdirectory(deps/glad)
add_subdirectory(deps/glfw-3.4)

include_directories(include)
include_directories(deps/glad/include)
include_directories(deps/glfw-3.4/include)

file(GLOB HPP_HEADER_FILES "include/mayhem/*.hpp")
file(GLOB CPP_SOURCE_FILES "src/*.cpp")

add_library(mayhem ${HPP_HEADER_FILES} ${CPP_SOURCE_FILES})
add_library(mayhem::cpp ALIAS mayhem)

target_link_libraries(mayhem glad glfw)

add_subdirectory(example)