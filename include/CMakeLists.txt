cmake_minimum_required(VERSION 3.22)
project(include_files)

message(STATUS "Setting include variables")

# General Files
set(GENERAL_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/socket.h
    ${CMAKE_CURRENT_SOURCE_DIR}/events.h
    ${CMAKE_CURRENT_SOURCE_DIR}/requestmanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/requestsocket.h
    ${CMAKE_CURRENT_SOURCE_DIR}/error.h
    ${CMAKE_CURRENT_SOURCE_DIR}/includes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/background.h

    CACHE INTERNAL "general includes"
)


# SERVER
set(SERVER_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/server.h
    ${CMAKE_CURRENT_SOURCE_DIR}/serversocket.h

    CACHE INTERNAL "server includes"
)


# CLIENT
set(CLIENT_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/client.h
    ${CMAKE_CURRENT_SOURCE_DIR}/clientsocket.h

    CACHE INTERNAL "client includes"
)
