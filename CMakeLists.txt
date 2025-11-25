cmake_minimum_required(VERSION 3.10)
project(RailwayApp)
set(CMAKE_CXX_STANDARD 17)

# Source files
file(GLOB SOURCES
    src/*.cpp
)

# Add include directory for headers
include_directories(include)

# Create executable
add_executable(RailwayApp ${SOURCES})
