cmake_minimum_required(VERSION 3.22)
project(t15_epoll C)

set(CMAKE_C_STANDARD 11)

add_executable(t15_epoll_server1 t15_epoll_server1.c)
add_executable(t15_epoll_server2 t15_epoll_server2.c)
add_executable(t15_epoll_server3 t15_epoll_server3.c)
