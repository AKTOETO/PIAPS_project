cmake_minimum_required(VERSION 3.22)

# C++ standart
set(CMAKE_CXX_STANDART 20)
set(CMAKE_CXX_STANDART_REQUIRED True)

# compiler settings
set(CMAKE_C_COMPILER "/usr/bin/gcc-11")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-11")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -fsanitize=address -g")
#set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -fsanitize=address -g")

project(catalog)

# installing binary folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

####################################
# INCLUDE LIBS
####################################
add_subdirectory(deps)

####################################
# INCLUDE HDRS&SRCS
####################################

add_subdirectory(include)
add_subdirectory(src)

####################################
#           EXECUTABLES
####################################

# client
add_executable(client ${GENERAL_SOURCE} ${GENERAL_INCLUDE} ${CLIENT_INCLUDE} ${CLIENT_SOURCE})
target_compile_options(client PRIVATE -W -Wall -fsanitize=address -g)
target_link_options(client PRIVATE -fsanitize=address)

# server
add_executable(server ${GENERAL_SOURCE} ${GENERAL_INCLUDE} ${SERVER_INCLUDE} ${SERVER_SOURCE})
target_compile_options(server PRIVATE -W -Wall -fsanitize=address -g)
target_link_options(server PRIVATE -fsanitize=address)

####################################
#      INCLUDING DIRECTORIES
####################################

target_include_directories(client PUBLIC ${ALL_INCLUDES} include)
target_include_directories(server PUBLIC ${ALL_INCLUDES} include)

####################################
#      LINK ALL LIBS TO EXECS
####################################

target_link_libraries(client ${ALL_LIBS})
target_link_libraries(server ${ALL_LIBS})

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