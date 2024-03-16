cmake_minimum_required(VERSION 3.9)
project(OpenGL_Cookbook)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(BOOK_LIBRARY_PATH "lib/book/src")

find_package(OpenGL REQUIRED)

add_subdirectory(lib/glfw-3.3.8)
add_subdirectory(lib/glad)

include_directories(lib/glm-0.9.9.8/glm/)
include_directories(lib/glfw-3.3.8/include)
include_directories(lib/glad/include)
include_directories(lib/book/)

add_executable(chp1_window "src/chapter 1/window.cpp")
add_executable(chp1_triangle "src/chapter 1/triangle.cpp")
add_executable(chp1_ripple_mesh "src/chapter 1/ripple_mesh.cpp")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


target_link_libraries(chp1_window glad glfw)
target_link_libraries(chp1_triangle glad glfw)
target_link_libraries(chp1_ripple_mesh glad glfw)