cmake_minimum_required(VERSION 3.22)
project(t08_batch_write C)

set(CMAKE_C_STANDARD 11)

add_executable(t08_batch_write1_server t08_batch_write1_server.c)
add_executable(t08_batch_write2_client t08_batch_write2_client.c)
