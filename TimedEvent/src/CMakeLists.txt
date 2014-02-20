# Add this directory to include search path for files here and below
set_property(DIRECTORY
             APPEND_STRING
             PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

# Global flags and defines passed to the compiler for everything here and below
add_compile_options(-c -Wall -Werror -std=c++11 -pthread -march=native -ggdb)

# Add a <library> to the project using the specified <source files>
# STATIC libraries are archives of object files
# SHARED  libraries  are linked dynamically
# MODULE libraries are plugins
add_library(TimedEvent OBJECT TimedEvent.cxx)
