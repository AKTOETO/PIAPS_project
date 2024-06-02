cmake_minimum_required(VERSION 3.22)
project(all_libs)

####################################
# OpenGL
####################################

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
message(STATUS "OPENGL_INCLUDE_DIR:= ${OPENGL_INCLUDE_DIR}/GL")
message(STATUS "OPENGL_LIBRARY:= ${OPENGL_LIBRARY}")
message(STATUS "OPENGL_LIBRARIES:= ${OPENGL_LIBRARIES}")

####################################
# SDL
####################################

set(SDL_SHARED FALSE CACHE BOOL "Build a SDL shared library")
set(SDL_STATIC TRUE CACHE BOOL "Build a SDL static library")

add_subdirectory(SDL3 EXCLUDE_FROM_ALL)
set(SDL_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/SDL3/include CACHE INTERNAL "sdl include")

####################################
# PQXX
####################################
set(SKIP_BUILD_TEST True)
add_subdirectory(PQXX build-pqxx)
set(PQXX_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/PQXX/include CACHE INTERNAL "postgresql include")

####################################
# JSON
####################################
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(NLOHMANN)
set(JSON_INCLUDE ${CMAKE_SOURCE_DIR}/NLOHMANN/include/ CACHE INTERNAL "nlohman json include")

####################################
# IMGUI
####################################

add_subdirectory(IMGUI)

####################################
# setting up all variables
####################################

set(ALL_LIBS
    ${OPENGL_LIBRARIES}
    SDL3::SDL3
    pqxx
    nlohmann_json::nlohmann_json
    imgui
    CACHE INTERNAL "all libraries to link"
)

set(ALL_INCLUDES 
    ${OPENGL_INCLUDE_DIR}/GL/
    ${SDL_INCLUDE}
    ${PQXX_INCLUDE}
    ${JSON_INCLUDE}
    ${IMGUI_INCLUDE_FOLDER}

    CACHE INTERNAL "all include paths"
)
