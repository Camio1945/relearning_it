cmake_minimum_required(VERSION 3.22)
project(IO_multiplexing C)

set(CMAKE_C_STANDARD 11)

add_executable(step1_server step1_server.c)
add_executable(step2_client step2_client.c)
add_executable(step3_client step3_client.c)
