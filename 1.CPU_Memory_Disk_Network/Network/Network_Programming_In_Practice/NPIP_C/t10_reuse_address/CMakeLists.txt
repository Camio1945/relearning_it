cmake_minimum_required(VERSION 3.22)
project(t10_reuse_address C)

set(CMAKE_C_STANDARD 11)

add_executable(t10_reuse_address1_server t10_reuse_address1_server.c)
add_executable(t10_reuse_address2_client t10_reuse_address2_client.c)
