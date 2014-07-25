##### CMake global

# Stop procesing, or automatically invoke cmake_policy(VERSION x.y.z)
cmake_minimum_required(VERSION 2.6)

# Set name and languages for entire project, & variables <projectName>_BINARY_DIR
# and <projectName>_SOURCE_DIR. Default language is CXX and C
project(TIMEDEVENT CXX)

# Include global include path for this project
set_property(DIRECTORY
             APPEND_STRING
             PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}")

# Show what make is actually doing
# set(CMAKE_VERBOSE_MAKEFILE true)

# Enable testing for current directory and below
enable_testing()

# Global flags and defines passed to the compiler for everything here and below
add_compile_options(-c -Wall -Werror -std=c++11 -pthread -march=native -ggdb)

# Source code subdirectory and it's cooresponding binary directory
add_subdirectory("test")

# Add a <library> to the project using the specified <source files>
# STATIC libraries are archives of object files
# SHARED  libraries  are linked dynamically
# MODULE libraries are plugins
add_library(TimedEvent OBJECT TimedEvent.cpp)
