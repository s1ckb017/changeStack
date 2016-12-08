cmake_minimum_required(VERSION 2.8)
project(ChgStack)
set (ChgStack_VERSION_MAJOR 1)
set (ChgStack_VERSION_MINOR 0)

set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/../bin)
include_directories("${PROJECT_SOURCE_DIR}/../include")
aux_source_directory( ${PROJECT_SOURCE_DIR}/../src CTR_SOURCES)

set (CMAKE_C_COMPILER "gcc")
set (CMAKE_C_FLAGS "-Wall")
add_executable("chgstack" ${CTR_SOURCES})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
