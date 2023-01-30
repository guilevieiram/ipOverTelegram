cmake_minimum_required(VERSION 3.15)

project(ipOverTelegram)


if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

add_definitions(-Wall -Wextra -Wpedantic -g -lcurl)

include_directories(include)
file(GLOB SOURCES "src/*.c")
add_library(ipOverTelegram SHARED ${SOURCES})


add_executable(main main.c)
add_executable(client client.c)
add_executable(server server.c)

target_link_libraries(main ipOverTelegram)
target_link_libraries(client ipOverTelegram)
target_link_libraries(server ipOverTelegram)

target_link_libraries(main ${CURL_LIBRARIES})
target_link_libraries(client ${CURL_LIBRARIES})
target_link_libraries(server ${CURL_LIBRARIES})
