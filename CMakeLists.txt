cmake_minimum_required(VERSION 3.22)
project(catalog)

# C++ standart
set(CMAKE_CXX_STANDART 20)
set(CMAKE_CXX_STANDART_REQUIRED True)

# installing binary folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# source files
file(GLOB_RECURSE src CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

set(imgui_src ${CMAKE_SOURCE_DIR}/deps/IMGUI)

set(src ${src}
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

# create executable
add_executable(${CMAKE_PROJECT_NAME} ${src})

####################################
#           LINK LIBS
####################################

# All libs to be linked
set(LIBS)

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

# including directories
target_include_directories(${CMAKE_PROJECT_NAME} 
    # opengl
    PUBLIC ${OPENGL_INCLUDE_DIR}/GL/

    # Make SDL2 available to Imgui
    PUBLIC ${CMAKE_SOURCE_DIR}/deps/SDL3/include/
    
    # imgui/backends/ contains the SDL implementation
    PUBLIC ${imgui_src}/
    PUBLIC ${imgui_src}/backends
)

####################################

# Link SDL, IMGUI, GLAD to Executable
target_link_libraries(${CMAKE_PROJECT_NAME} ${LIBS})

####################################