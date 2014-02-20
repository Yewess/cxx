# Flags passed to the compiler for this dir and below
add_compile_options(-c -Wall -Werror -std=c++11 -pthread -march=native -ggdb)

# Add compile definitions (i.e. -Dfoo ) to current dir and below
add_definitions(-DDEBUG)

# Add this directory to include search path for files here and below
set_property(DIRECTORY
             APPEND_STRING
             PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

# Find all files in this directory ending in _test.cxx
file(GLOB testfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cxx")
string(REPLACE ".cxx" "" testnames "${testfiles}")

# Add executables, tests, and linking flags for everythig in testlist
foreach(testname IN LISTS testnames)
    message("Adding testname ${testname}")
    # Add executable target <name> to build from source files listed
    add_executable(${testname} "${testname}.cxx" $<TARGET_OBJECTS:TimedEvent>)
    # Add a test target for a test executable
    add_test(${testname} ${testname})
endforeach(testname)
