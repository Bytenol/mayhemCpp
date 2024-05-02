cmake_minimum_required(VERSION 3.25)
project(Mayhem_CPP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)


option(BUILD_EXAMPLE "Check if to build example" ON)
option(BUILD_TEST "Check if to build tests" ON)


find_package(OpenGL REQUIRED)


add_subdirectory(deps/glad)
add_subdirectory(deps/glfw-3.3.8)

include_directories(deps/glad/include)
include_directories(deps/glfw-3.3.8/include)
include_directories(deps/googletest/include)
include_directories(deps/googlemock/include)


file(GLOB HPP_HEADER_FILES "include/mayhem/*.hpp")
file(GLOB CPP_SOURCE_FILES "src/*.cpp")


add_library(mayhem ${HPP_HEADER_FILES} ${CPP_SOURCE_FILES})
add_library(mayhem::cpp ALIAS mayhem)

target_link_libraries(mayhem glad glfw)


if(BUILD_EXAMPLE)
    message("Building Examples")
    add_subdirectory(example)
endif()


if(BUILD_TEST)
    message("Building Tests")
    add_subdirectory(deps/googletest)
    add_subdirectory(test)
endif()