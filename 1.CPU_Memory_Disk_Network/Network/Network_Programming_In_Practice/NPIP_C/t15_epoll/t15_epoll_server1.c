#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdarg.h>        /* ANSI C header file */
#include <sys/socket.h>    /* basic socket definitions */
#include <netinet/in.h>    /* sockaddr_in{} and other Internet definitions */
#include <fcntl.h>
#include  <sys/epoll.h>

#define MAX_LINE 1024
#define SERV_PORT 9123
#define LISTENQ 1024
#define FD_INIT_SIZE 128

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

#define MAXEVENTS 128

char rot13_char(char c) {
  if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
    return c + 13;
  else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
    return c - 13;
  else
    return c;
}


void make_nonblocking(int fd) {
  fcntl(fd, F_SETFL, O_NONBLOCK);
}

int tcp_nonblocking_server_listen(int port) {
  int listenfd;
  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  make_nonblocking(listenfd);

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
  int listen_fd, socket_fd;
  int n, i;
  int efd;
  struct epoll_event event;
  struct epoll_event *events;

  listen_fd = tcp_nonblocking_server_listen(SERV_PORT);

  efd = epoll_create1(0);
  if (efd == -1) {
    error(1, errno, "epoll create failed");
  }

  event.data.fd = listen_fd;
  event.events = EPOLLIN | EPOLLET;
  if (epoll_ctl(efd, EPOLL_CTL_ADD, listen_fd, &event) == -1) {
    error(1, errno, "epoll_ctl add listen fd failed");
  }

  /* Buffer where events are returned */
  events = calloc(MAXEVENTS, sizeof(event));

  while (1) {
    n = epoll_wait(efd, events, MAXEVENTS, -1);
    printf("epoll_wait wakeup\n");
    for (i = 0; i < n; i++) {
      if ((events[i].events & EPOLLERR) ||
          (events[i].events & EPOLLHUP) ||
          (!(events[i].events & EPOLLIN))) {
        fprintf(stderr, "epoll error\n");
        close(events[i].data.fd);
        continue;
      } else if (listen_fd == events[i].data.fd) {
        struct sockaddr_storage ss;
        socklen_t slen = sizeof(ss);
        int fd = accept(listen_fd, (struct sockaddr *) &ss, &slen);
        if (fd < 0) {
          error(1, errno, "accept failed");
        } else {
          make_nonblocking(fd);
          event.data.fd = fd;
          event.events = EPOLLIN | EPOLLET; //edge-triggered
          if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &event) == -1) {
            error(1, errno, "epoll_ctl add connection fd failed");
          }
        }
        continue;
      } else {
        socket_fd = events[i].data.fd;
        printf("get event on socket fd == %d \n", socket_fd);
        while (1) {
          char buf[512];
          if ((n = read(socket_fd, buf, sizeof(buf))) < 0) {
            if (errno != EAGAIN) {
              error(1, errno, "read error");
              close(socket_fd);
            }
            break;
          } else if (n == 0) {
            close(socket_fd);
            break;
          } else {
            for (i = 0; i < n; ++i) {
              buf[i] = rot13_char(buf[i]);
            }
            if (write(socket_fd, buf, n) < 0) {
              error(1, errno, "write error");
            }
          }
        }
      }
    }
  }
}


