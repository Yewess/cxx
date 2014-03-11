##### CMake global

# Stop procesing, or automatically invoke cmake_policy(VERSION x.y.z)
cmake_minimum_required(VERSION 2.6)

# Set name and languages for entire project, & variables <projectName>_BINARY_DIR
# and <projectName>_SOURCE_DIR. Default language is CXX and C
project(MOVINGAVG CXX)

# Include global include path for this project
set_property(DIRECTORY
             APPEND_STRING
             PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Show what make is actually doing
# set(CMAKE_VERBOSE_MAKEFILE true)

# Enable testing for current directory and below
enable_testing()

# Source code subdirectory and it's cooresponding binary directory
add_subdirectory("include")
add_subdirectory("src")
add_subdirectory("test")
