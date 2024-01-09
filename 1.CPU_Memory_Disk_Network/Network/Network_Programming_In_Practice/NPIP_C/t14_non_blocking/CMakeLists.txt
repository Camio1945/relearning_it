cmake_minimum_required(VERSION 3.22)
project(t14_non_blocking C)

set(CMAKE_C_STANDARD 11)

add_executable(t14_non_blocking1_server t14_non_blocking1_server.c)
add_executable(t14_non_blocking2_client t14_non_blocking2_client.c)
