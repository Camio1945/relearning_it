cmake_minimum_required(VERSION 3.22)
project(t03_udp_data_transfer C)

set(CMAKE_C_STANDARD 11)

add_executable(udp_data_transfer_1_server udp_data_transfer_1_server.c)
add_executable(udp_data_transfer_2_client udp_data_transfer_2_client.c)
