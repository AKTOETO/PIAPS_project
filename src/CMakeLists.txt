cmake_minimum_required(VERSION 3.22)
project(src_files)

message(STATUS "Setting source variables")

# General Files
set(GENERAL_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/socket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/events.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/requestmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/requestsocket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/error.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/background.cpp

    CACHE INTERNAL "general sources"
)
message(GENERAL_SOURCE: ${GENERAL_SOURCE})

# SERVER
set(SERVER_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/serversocket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/servermain.cpp

    CACHE INTERNAL "server sources"
)
message(SERVER_SOURCE: ${SERVER_SOURCE})


# CLIENT
set(CLIENT_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/clientsocket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/clientmain.cpp

    CACHE INTERNAL "client sources"
)
message(CLIENT_SOURCE: ${CLIENT_SOURCE})
