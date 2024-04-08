# GLAD configuration

cmake_minimum_required(VERSION 3.22)
project(Glad)

# GLAD headers
set(GLAD_HDRS include/glad/glad.h)

add_library(GLAD STATIC include/glad/glad.h src/glad.c)
target_include_directories(GLAD PUBLIC include/)