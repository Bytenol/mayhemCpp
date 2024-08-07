cmake_minimum_required(VERSION 3.9)

project(glad)

include_directories(include)

add_library(glad STATIC src/glad.c)