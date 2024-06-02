cmake_minimum_required(VERSION 3.22)
project(imgui_lib)

set(IMGUI_INCLUDE_FOLDER
    ${CMAKE_CURRENT_SOURCE_DIR}

    CACHE INTERNAL "imgui include folder"
)

set(IMGUI_SOURCES
    # main IMGUI files
    imconfig.h
    imgui.cpp
    imgui.h
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_internal.h
    imgui_widgets.cpp
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
    imgui_tables.cpp

    # SDL + OPENGL 
    backends/imgui_impl_sdl3.cpp
    backends/imgui_impl_sdlrenderer3.cpp
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_opengl3.h

    CACHE INTERNAL "imgui sources"
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL_INCLUDE}
    #${CMAKE_SOURCE_DIR}/deps/SDL3/include/
)