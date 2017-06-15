cmake_minimum_required (VERSION 2.6)
project (hashtable)

set (hashtable_VERSION_MAJOR 1)
set (hashtable_VERSION_MINOR 0)

set (CMAKE_BUILD_TYPE Debug)

include_directories ("${PROJECT_BINARY_DIR}/../include")

add_library(simpleHash STATIC src/hash.c)

add_executable (hashtable src/main.c)
target_link_libraries(hashtable simpleHash)
