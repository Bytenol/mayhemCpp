cmake_minimum_required(VERSION 3.25)

project(Bytenol_Showcase_LearnOpengl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/tutorial/learn_opengl)


set(SRC_FILE 
triangle
textures)


foreach(f ${SRC_FILE})
    add_executable(${f} ${f}.cpp)
    target_link_libraries(${f} mayhem)
endforeach()