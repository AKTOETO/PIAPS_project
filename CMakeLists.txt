cmake_minimum_required(VERSION 3.22)
project(catalog)

# C++ standart
set(CMAKE_CXX_STANDART 20)
set(CMAKE_CXX_STANDART_REQUIRED True)

# get files
file(GLOB_RECURSE src CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

add_executable(${CMAKE_PROJECT_NAME} ${src})

