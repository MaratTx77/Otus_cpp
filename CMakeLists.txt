cmake_minimum_required(VERSION 3.13)

enable_testing()

set(PATCH_VERSION "2" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(learn_cpp_otus VERSION ${PROJECT_VERSION})
include_directories(${PROJECT_BINARY_DIR})

find_package(Boost COMPONENTS system REQUIRED)
if(Boost_FOUND)
    message(STATUS "Found boost ${Boost_VERSION_STRING}")
endif()

# CMake dynamically generated file stored in main directory
configure_file(version.h.in version.h)
add_library(common_lib lib.cpp)
target_compile_options(common_lib PRIVATE -Wall -Wextra -pedantic -Werror)

# Plan to use google test for all lessons
add_subdirectory(googletest)

# Each separate lesson will be created with own CMake configuration inside XX-lesson_name subdirectory
add_subdirectory(01-helloworld)

set(CMAKE_CXX_STANDARD 14)
