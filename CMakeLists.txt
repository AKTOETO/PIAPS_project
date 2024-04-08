cmake_minimum_required(VERSION 3.22)
project(catalog)

# C++ standart
set(CMAKE_CXX_STANDART 20)
set(CMAKE_CXX_STANDART_REQUIRED True)

# installing binary folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# include dependencies
add_subdirectory(deps/SDL EXCLUDE_FROM_ALL)
add_subdirectory(deps/GLAD EXCLUDE_FROM_ALL)

# get files
file(GLOB_RECURSE src CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

set(imgui_src ${CMAKE_SOURCE_DIR}/deps/IMGUI)

set(src ${src}
    #deps/GLAD/include/glad/glad.h
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
    ${imgui_src}/backends/imgui_impl_sdl2.cpp
    ${imgui_src}/backends/imgui_impl_sdlrenderer2.cpp
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
#include_directories(src ${src} ${OPENGL_INCLUDE_DIRS})

set(LIBS ${LIBS} ${OPENGL_LIBRARIES})

####################################
# GLAD
####################################

set(LIBS ${LIBS} GLAD)

####################################
# SDL
####################################
# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    #target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2main)
    set(LIBS ${LIBS} SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
#target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2)
set(LIBS ${LIBS} SDL2::SDL2)

####################################

# including directories
target_include_directories(${CMAKE_PROJECT_NAME} 
    # opengl
    PUBLIC ${OPENGL_INCLUDE_DIR}/GL/

    # Make GLAD includes avaliable
    PUBLIC ${CMAKE_SOURCE_DIR}/deps/GLAD/include/

    # Make SDL2 available to Imgui
    PUBLIC ${CMAKE_SOURCE_DIR}/deps/SDL/include/
    
    # imgui/backends/ contains the SDL implementation
    PUBLIC ${imgui_src}/
    PUBLIC ${imgui_src}/backends
)

# Configure Imgui to use OpengGL through SDL2
# target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<SDL_opengl.h>  GL_GLEXT_PROTOTYPES=1)

####################################

# Link SDL, IMGUI, GLAD to Executable
target_link_libraries(${CMAKE_PROJECT_NAME} ${LIBS})

####################################