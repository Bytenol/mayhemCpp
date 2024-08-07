cmake_minimum_required(VERSION 3.9)

project(Mayhem_example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/example)

include_directories(deps/glad/)
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE CPP_SOURCE_FILES "src/*.cpp")

foreach(SRC IN LISTS CPP_SOURCE_FILES)
    string(REGEX MATCH "[A-Z|a-z|0-9]+.cpp" FILE_NAME ${SRC})
    string(REPLACE ".cpp" "" F ${FILE_NAME})
    add_executable(${F} "src/${FILE_NAME}")
    target_link_libraries(${F} mayhem)
endforeach()
