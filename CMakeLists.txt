cmake_minimum_required(VERSION 3.13)

enable_testing()

set(PATCH_VERSION "2" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(learn_cpp_otus VERSION ${PROJECT_VERSION})
include_directories(${PROJECT_BINARY_DIR})

# Plan to use google test for all lessons
add_subdirectory(googletest)

# Each separate lesson will be created with own CMake configuration inside XX-lesson_name subdirectory
add_subdirectory(04-print_ip)

set(CMAKE_CXX_STANDARD 20)
