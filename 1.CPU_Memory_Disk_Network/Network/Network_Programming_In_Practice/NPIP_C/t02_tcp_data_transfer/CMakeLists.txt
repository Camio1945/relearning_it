cmake_minimum_required(VERSION 3.22)
project(t01_tcp_data_transfer C)

set(CMAKE_C_STANDARD 11)

add_executable(tcp_data_transfer_1_server tcp_data_transfer_1_server.c)

add_executable(tcp_data_transfer_2_client tcp_data_transfer_2_client.c)
