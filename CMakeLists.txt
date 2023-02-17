cmake_minimum_required(VERSION 2.8)

project(ipOverTelegram)

FIND_PACKAGE (Threads REQUIRED)
set(CMAKE_THREAD_LIBS_INIT "-lpthread")

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
 
add_definitions(-Wall -Wextra -Wpedantic -g -lcurl -lpthread)

include_directories(include)
file(GLOB SOURCES "src/*.c")
add_library(ipOverTelegram SHARED ${SOURCES})


add_executable(client client.c)
add_executable(server server.c)

target_link_libraries(client ipOverTelegram)
target_link_libraries(server ipOverTelegram)

target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(client ${CURL_LIBRARIES})
target_link_libraries(server ${CURL_LIBRARIES})
