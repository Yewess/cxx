# Add this directory to include search path for files here and below
set_property(DIRECTORY
             APPEND_STRING
             PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

# Global flags and defines passed to the compiler for everything here and below
add_compile_options(-c -Wall -Werror -std=c++11 -pthread -march=native -ggdb)
add_compile_options(-Wfatal-errors)

# Add a <library> to the project using the specified <source files>
# STATIC libraries are archives of object files
# SHARED  libraries  are linked dynamically
# MODULE libraries are plugins
# OBJECT only produce object code output
add_library(MaxDS18B20 OBJECT MaxDS18B20.cxx)
