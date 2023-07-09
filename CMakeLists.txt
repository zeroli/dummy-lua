# CMakeList.txt : CMake project for clox, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project(dummy-lua)

# Add source to this project's executable.
aux_source_directory(. SRCS)
add_executable (dummy-lua ${SRCS})

# TODO: Add tests and install targets if needed.
