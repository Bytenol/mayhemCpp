cmake_minimum_required(VERSION 3.9)

project(Mayhem_example)

set(BASE_DIR ${CMAKE_SOURCE_DIR}/example/)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/example)

include_directories(include/glm-0.9.9.8/glm)
include_directories(include/standalone/include)

file(GLOB_RECURSE CPP_SOURCE_FILES "src/*.cpp")

foreach(SRC IN LISTS CPP_SOURCE_FILES)
    string(REGEX MATCH "[A-Z|a-z]+.cpp" FILE_NAME ${SRC})
    string(REPLACE ".cpp" "" F ${FILE_NAME})
    add_executable(${F} "src/${FILE_NAME}")
    target_link_libraries(${F} mayhem)
endforeach()
