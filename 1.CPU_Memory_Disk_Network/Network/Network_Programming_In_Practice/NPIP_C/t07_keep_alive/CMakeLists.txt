cmake_minimum_required(VERSION 3.22)
project(t07_keep_alive C)

set(CMAKE_C_STANDARD 11)

add_executable(t07_keep_alive_server t07_keep_alive1_server.c)
add_executable(t07_keep_alive_client t07_keep_alive2_client.c)
