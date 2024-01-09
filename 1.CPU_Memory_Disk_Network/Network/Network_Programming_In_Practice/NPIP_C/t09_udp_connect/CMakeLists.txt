cmake_minimum_required(VERSION 3.22)
project(t09_udp_connect C)

set(CMAKE_C_STANDARD 11)

add_executable(t09_udp_connect1_client t09_udp_connect1_client.c)
add_executable(t09_udp_connect2_server t09_udp_connect2_server.c)
add_executable(t09_udp_connect3_client t09_udp_connect3_client.c)
