# All tests are expected to have a .cc extension
# otherwise, they will not appear in the build
cmake_minimum_required(VERSION 3.25)
project(Mayhem_CPP_TEST)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/test)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/deps/googletest/googletest/include)
include_directories(${CMAKE_SOURCE_DIR}/deps/googletest/googlemock/include)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()
include(GoogleTest)

FILE(GLOB TEST_FILES src/*.cc)

foreach (SRC IN LISTS TEST_FILES)
    string(REGEX MATCH "[A-Z|a-z|0-9]+.cc" FILE_NAME ${SRC})
    string(REPLACE ".cc" "" FILE ${FILE_NAME})
    add_executable(${FILE} "src/${FILE_NAME}")
    target_link_libraries(${FILE} mayhem::cpp GTest::gtest_main)
    gtest_discover_tests(${FILE})
endforeach ()