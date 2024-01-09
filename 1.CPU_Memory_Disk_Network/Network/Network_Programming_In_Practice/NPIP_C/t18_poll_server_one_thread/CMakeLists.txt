cmake_minimum_required(VERSION 3.22)
project(t18_poll_server_one_thread C)

set(CMAKE_C_STANDARD 11)

add_library(lib2
        lib/log.c
        lib/sock_ntop.c
        lib/read.c
        lib/tcp_server.c
        lib/tcp_client.c
        lib/event_loop.c
        lib/channel.c
        lib/acceptor.c
        lib/channel_map.c
        lib/poll_dispatcher.c
        lib/thread_pool.c
        lib/event_loop_thread.c
        lib/utils.c
        lib/epoll_dispatcher.c
        lib/buffer.c
        lib/tcp_connection.c
        lib/http_server.c
        lib/http_request.c
        lib/http_response.c
        )
add_executable(t18_poll_server_one_thread t18_poll_server_one_thread.c)
target_link_libraries(t18_poll_server_one_thread lib2)
