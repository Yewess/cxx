##### CMake global

# Stop procesing, or automatically invoke cmake_policy(VERSION x.y.z)
cmake_minimum_required(VERSION 2.6)

# Set name and languages for entire project, & variables <projectName>_BINARY_DIR
# and <projectName>_SOURCE_DIR. Default language is CXX and C
project(MAXDS18X20 CXX)

# Add this directory to include search path for files here and below
set_property(DIRECTORY
             APPEND_STRING
             PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}")

# Add include directory to include search path for files here and below
set_property(DIRECTORY
             APPEND_STRING
             PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Show what make is actually doing
# set(CMAKE_VERBOSE_MAKEFILE true)

# Enable testing for current directory and below
enable_testing()

# Source code subdirectory and it's cooresponding binary directory
add_subdirectory("include")
add_subdirectory("test")

# Global flags and defines passed to the compiler for everything here and below
add_compile_options(-c -Wall -Werror -std=c++11 -pthread -march=native -ggdb)
add_compile_options(-Wfatal-errors)

# Add a <library> to the project using the specified <source files>
# STATIC libraries are archives of object files
# SHARED  libraries  are linked dynamically
# MODULE libraries are plugins
# OBJECT only produce object code output
add_library(MaxDS18x20 OBJECT MaxDS18x20.cpp)
