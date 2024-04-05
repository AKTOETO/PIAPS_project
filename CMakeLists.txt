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

# get files
file(GLOB_RECURSE src CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

# create executable
add_executable(${CMAKE_PROJECT_NAME} ${src})

####################################
#           LINK LIBS
####################################

# All libs to be linked
set(LIBS )

####################################
# SDL
####################################
# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
#target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2)
set(LIBS ${LIBS} SDL2::SDL2)

####################################
# IMGUI
####################################

set(imgui_src ${CMAKE_SOURCE_DIR}/deps/IMGUI)

add_library(IMGUI STATIC
    # main IMGUI files
    ${imgui_src}/imgui.cpp
    ${imgui_src}/imgui_draw.cpp
    ${imgui_src}/imgui_tables.cpp
    ${imgui_src}/imgui_widgets.cpp
    
    # SDL + OPENGL 
    ${imgui_src}/backends/imgui_impl_sdl2.cpp
    #${imgui_src}/backends/imgui_impl_opengl3.cpp
)

# including directories to IMGUI
target_include_directories(IMGUI 
    # Make SDL2 available to Imgui
    PUBLIC ${CMAKE_SOURCE_DIR}/deps/SDL/include/
    
    # imgui/backends/ contains the SDL implementation
    PUBLIC ${imgui_src}/
    PUBLIC ${imgui_src}/backends
)

# Configure Imgui to use OpengGL through SDL2
#target_compile_definitions(IMGUI PUBLIC IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<SDL_opengl.h>  GL_GLEXT_PROTOTYPES=1)
set(LIBS ${LIBS} IMGUI)

####################################

# Link SDL, IMGUI to Executable
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIBS})

####################################