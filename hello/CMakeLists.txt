# This is a comment

# Stop procesing, or automatically invoke cmake_policy(VERSION x.y.z)
cmake_minimum_required(VERSION 2.6)

# Set name and languages for entire project, & variables <projectName>_BINARY_DIR
# and <projectName>_SOURCE_DIR. Default language is CXX and C
project(HELLO CXX)

# Recurse into subdirectories source_dir [binary_dir] where output goes into
# binary_dir
#add_subdirectory(Hello)
#add_subdirectory(Demo)

# Global flags to pass to the compiler for everything here and below
add_definitions(-c -Wall -Werror -std=c++11 -pthread -march=native)

# Enable testing for current directory and below
enable_testing()

# Add given directory to compiler to search for include files
#include_directories($(HELLO_SOURCE_DIR))

# Paths for linker to search for libraries
#link_directories($(HELLO_BINARY_DIR))

# Add executable target <name> to build from source files listed
add_executable(hello hello.c++)
add_executable(hello_test hello_test.c++)

# Add a dependency between top-level targets
add_dependencies(hello_test hello)

# Add compile options to a target.
# target, scope, items
target_compile_options(hello_test PUBLIC -ggdb)

# Add compile definitions (i.e. -Dfoo ) to a target.
# target, scope, items
target_compile_definitions(hello_test PUBLIC -DDEBUG)

# Append to INCLUDE_DIRECTORIES property for specific target
set_property(TARGET hello_test
             APPEND_STRING
             PROPERTY INCLUDE_DIRECTORIES /usr/include/pstreams)

# Add test to the project with name, exename, arg1...argN
# name can specify an executable target
add_test(hello_test hello_test)
