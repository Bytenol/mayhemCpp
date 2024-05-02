cmake_minimum_required(VERSION 3.25)
project(Mayhem_CPP_TEST)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/test)

include_directories(../deps/googletest/googletest/include)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()
include(GoogleTest)

add_executable(basic_test basic_test.cc)

target_link_libraries(basic_test GTest::gtest_main)
gtest_discover_tests(basic_test)
