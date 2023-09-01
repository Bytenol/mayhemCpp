cmake_minimum_required(VERSION 3.25)

project(Bytenol_Showcase)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)


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
add_subdirectory(project/learn_opengl)
add_subdirectory(scratch)

target_link_libraries(glad glfw)