cmake_minimum_required(VERSION 3.25)

project(Bytenol_Showcase_Small)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/project/small)

add_executable(matrixRain matrixRain.cpp)
target_link_libraries(matrixRain mayhem)

add_executable(rose rose.cpp)
target_link_libraries(rose mayhem)