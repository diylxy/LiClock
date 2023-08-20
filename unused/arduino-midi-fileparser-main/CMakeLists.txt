cmake_minimum_required(VERSION 3.0)

# set the project name
project(midi-file)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

add_library(midi-file INTERFACE)

# make include directory available to calling projects 
target_include_directories (midi-file INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# installation of all header files
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/" # source directory
    DESTINATION "include/midi-file" # target directory
    FILES_MATCHING # install only matched files
    PATTERN "*.h" # select header files
)