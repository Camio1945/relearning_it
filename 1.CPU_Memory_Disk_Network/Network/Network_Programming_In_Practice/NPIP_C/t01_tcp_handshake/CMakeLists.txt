cmake_minimum_required(VERSION 3.22)
project(t01_tcp_handshake C)

set(CMAKE_C_STANDARD 11)

add_executable(tcp_handshake_1_server tcp_handshake_1_server.c)

add_executable(tcp_handshake_2_client tcp_handshake_2_client.c)
