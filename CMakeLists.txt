cmake_minimum_required(VERSION 3.25)

project(Bytenol_Showcase)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(GIT_PUSH_READY OFF)


include_directories(lib/glad/include)
include_directories(lib/glfw/include)
include_directories(lib/glm)
include_directories(lib/mayhem/include)
include_directories(lib/misc/include)

add_subdirectory(lib/glad)
add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)
add_subdirectory(lib/mayhem)

add_subdirectory(project/small)

if(GIT_PUSH_READY)
    message("Cmake building files for github commit")
else()
    message("Cmake building file for offline commit")
    add_subdirectory(scratch)
    add_subdirectory(tutorial/learn_opengl)
endif()