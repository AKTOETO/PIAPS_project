cmake_minimum_required(VERSION 3.22)

# C++ standart
set(CMAKE_CXX_STANDART 20)
set(CMAKE_CXX_STANDART_REQUIRED True)

# compiler settings
set(CMAKE_C_COMPILER "/usr/bin/gcc-11")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-11")

project(catalog)

# installing binary folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

####################################
#      INCLUDE AND SORCE FILES
#       FOR CLIENT AND SERVER
####################################

# client sources
set(CLIENT_SOURCES
    src/client.cpp
)

# client includes
set(CLIENT_INCLUDE
    include/client.h
)

# server sources
set(SERVER_SOURCES
    src/server.cpp
    src/servermain.cpp
)

# server includes
set(SERVER_INCLUDE
    include/server.h
)

####################################

# source files
# file(GLOB_RECURSE src CONFIGURE_DEPENDS ${INCLUDE}/*.h ${SOURCES}/*.cpp)

####################################
#        SETTING UP IMGUI
####################################

# imgui folder
set(imgui_src ${CMAKE_SOURCE_DIR}/deps/IMGUI)

set(IMGUI_SOURCES
    # main IMGUI files
    ${imgui_src}/imconfig.h
    ${imgui_src}/imgui.cpp
    ${imgui_src}/imgui.h
    ${imgui_src}/imgui_demo.cpp
    ${imgui_src}/imgui_draw.cpp
    ${imgui_src}/imgui_internal.h
    ${imgui_src}/imgui_widgets.cpp
    ${imgui_src}/imstb_rectpack.h
    ${imgui_src}/imstb_textedit.h
    ${imgui_src}/imstb_truetype.h
    ${imgui_src}/imgui_tables.cpp

    # SDL + OPENGL 
    ${imgui_src}/backends/imgui_impl_sdl3.cpp
    ${imgui_src}/backends/imgui_impl_sdlrenderer3.cpp
    ${imgui_src}/backends/imgui_impl_opengl3.cpp
    ${imgui_src}/backends/imgui_impl_opengl3.h
)

####################################
#      GENERAL INCLUDE FOLDERS
####################################

# include folder
set(GEN_INCLUDE_FOLDERS 
    ${CMAKE_SOURCE_DIR}/include/
    # opengl
    ${OPENGL_INCLUDE_DIR}/GL/

    # Make SDL3 available to Imgui
    ${CMAKE_SOURCE_DIR}/deps/SDL3/include/

    # imgui/backends/ contains the SDL implementation
    ${imgui_src}/
    ${imgui_src}/backends

    # postgresql
    ${CMAKE_SOURCE_DIR}/deps/PQXX/include/
)

####################################
#           EXECUTABLES
####################################

# client
add_executable(client ${IMGUI_SOURCES} ${CLIENT_INCLUDE} ${CLIENT_SOURCES})

# server
add_executable(server ${IMGUI_SOURCES} ${SERVER_INCLUDE} ${SERVER_SOURCES})

####################################
#           LINK LIBS
####################################

# All libs to be linked
set(LIBS)

####################################
# error
####################################

add_library(error STATIC src/error.cpp include/error.h)
target_include_directories(error PUBLIC include/)
set(LIBS ${LIBS} error)

####################################
# socket
####################################

add_library(socket STATIC src/socket.cpp include/socket.h)
target_include_directories(socket PUBLIC include/)
set(LIBS ${LIBS} socket)

####################################
# OpenGL
####################################

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
message(STATUS "OPENGL_INCLUDE_DIR:= ${OPENGL_INCLUDE_DIR}/GL")
message(STATUS "OPENGL_LIBRARY:= ${OPENGL_LIBRARY}")
message(STATUS "OPENGL_LIBRARIES:= ${OPENGL_LIBRARIES}")

set(LIBS ${LIBS} ${OPENGL_LIBRARIES})

####################################
# SDL
####################################

set(SDL_SHARED FALSE CACHE BOOL "Build a SDL shared library")
set(SDL_STATIC TRUE CACHE BOOL "Build a SDL static library")

add_subdirectory(deps/SDL3 EXCLUDE_FROM_ALL)

set(LIBS ${LIBS} SDL3::SDL3)

####################################
# PQXX
####################################
set(SKIP_BUILD_TEST True)
add_subdirectory(deps/PQXX build-pqxx)
set(LIBS ${LIBS} pqxx)

####################################
#      INCLUDING DIRECTORIES
####################################

target_include_directories(client PUBLIC ${GEN_INCLUDE_FOLDERS})
target_include_directories(server PUBLIC ${GEN_INCLUDE_FOLDERS})

####################################
#      LINK ALL LIBS TO EXECS
####################################

target_link_libraries(client ${LIBS})
target_link_libraries(server ${LIBS})

####################################
#    COPYING ASSET FOLDER TO BIN
####################################

# copy assets folder
add_custom_target(
    copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "${CMAKE_BINARY_DIR}/bin/assets"
    COMMENT "Copy assets from ${PROJECT_SOURCE_DIR}/assets to ${CMAKE_BINARY_DIR}/bin"
)
add_dependencies(client copy_assets)
add_dependencies(server copy_assets)

# If on Release, disable console window and enable optimizations
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if (MSVC)
        set_target_properties(LightWeightUI PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
        # Enable optimizations
        target_compile_options(LightWeightUI PRIVATE /O2)
    endif()
endif()