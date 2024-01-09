cmake_minimum_required(VERSION 3.22)
project(t11_tcp_stream C)

set(CMAKE_C_STANDARD 11)

add_executable(t11_tcp_stream1_server t11_tcp_stream1_server.c)
add_executable(t11_tcp_stream2_client t11_tcp_stream2_client.c)
