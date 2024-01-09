#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdarg.h>        /* ANSI C header file */
#include <sys/socket.h>    /* basic socket definitions */
#include <netinet/in.h>    /* sockaddr_in{} and other Internet definitions */
#include <sys/poll.h>

#define MAXLINE 4096
#define SERV_PORT 9123
#define LISTENQ 1024
#define INIT_SIZE 128

/* error - print a diagnostic and optionally exit */
void error(int status, int err, char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  if (err)
    fprintf(stderr, ": %s (%d)\n", strerror(err), err);
  if (status)
    exit(status);
}

int tcp_server_listen(int port) {
  int listenfd;
  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(port);

  int on = 1;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  int rt1 = bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
  if (rt1 < 0) {
    error(1, errno, "bind failed ");
  }

  int rt2 = listen(listenfd, LISTENQ);
  if (rt2 < 0) {
    error(1, errno, "listen failed ");
  }

  signal(SIGPIPE, SIG_IGN);

  return listenfd;
}

int main(int argc, char **argv) {
  printf("You can use multiple telnets to connect to this server\n");
  int listen_fd, connected_fd;
  int ready_number;
  ssize_t n;
  char buf[MAXLINE];
  struct sockaddr_in client_addr;

  listen_fd = tcp_server_listen(SERV_PORT);

  //初始化pollfd数组，这个数组的第一个元素是listen_fd，其余的用来记录将要连接的connect_fd
  struct pollfd event_set[INIT_SIZE];
  event_set[0].fd = listen_fd;
  event_set[0].events = POLLRDNORM;

  // 用-1表示这个数组位置还没有被占用
  int i;
  for (i = 1; i < INIT_SIZE; i++) {
    event_set[i].fd = -1;
  }

  for (;;) {
    if ((ready_number = poll(event_set, INIT_SIZE, -1)) < 0) {
      error(1, errno, "poll failed ");
    }

    if (event_set[0].revents & POLLRDNORM) {
      socklen_t client_len = sizeof(client_addr);
      connected_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);

      //找到一个可以记录该连接套接字的位置
      for (i = 1; i < INIT_SIZE; i++) {
        if (event_set[i].fd < 0) {
          event_set[i].fd = connected_fd;
          event_set[i].events = POLLRDNORM;
          break;
        }
      }

      if (i == INIT_SIZE) {
        error(1, errno, "can not hold so many clients");
      }

      if (--ready_number <= 0)
        continue;
    }

    for (i = 1; i < INIT_SIZE; i++) {
      int socket_fd;
      if ((socket_fd = event_set[i].fd) < 0)
        continue;
      if (event_set[i].revents & (POLLRDNORM | POLLERR)) {
        if ((n = read(socket_fd, buf, MAXLINE)) > 0) {
          printf("%s", buf);
          if (write(socket_fd, buf, n) < 0) {
            error(1, errno, "write error");
          }
        } else if (n == 0 || errno == ECONNRESET) {
          close(socket_fd);
          event_set[i].fd = -1;
        } else {
          error(1, errno, "read error");
        }

        if (--ready_number <= 0)
          break;
      }
    }
  }
}

