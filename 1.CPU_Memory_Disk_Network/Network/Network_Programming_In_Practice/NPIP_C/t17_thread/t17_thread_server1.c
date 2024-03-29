#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdarg.h>        /* ANSI C header file */
#include <sys/socket.h>    /* basic socket definitions */
#include <netinet/in.h>    /* sockaddr_in{} and other Internet definitions */
#include <sys/wait.h>
#include <pthread.h>

#define MAX_LINE 16384
#define SERV_PORT 9123
#define    LISTENQ        1024

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

char rot13_char(char c) {
  if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
    return c + 13;
  else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
    return c - 13;
  else
    return c;
}

void loop_echo(int fd) {
  char outbuf[MAX_LINE + 1];
  size_t outbuf_used = 0;
  ssize_t result;
  while (1) {
    char ch;
    result = recv(fd, &ch, 1, 0);

    //断开连接或者出错
    if (result == 0) {
      break;
    } else if (result == -1) {
      error(1, errno, "read error");
      break;
    }

    if (outbuf_used < sizeof(outbuf)) {
      outbuf[outbuf_used++] = rot13_char(ch);
    }

    if (ch == '\n') {
      send(fd, outbuf, outbuf_used, 0);
      outbuf_used = 0;
      continue;
    }
  }
}

void thread_run(void *arg) {
  pthread_detach(pthread_self());
  int fd = (int) arg;
  loop_echo(fd);
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


int main(int c, char **v) {
  int listener_fd = tcp_server_listen(SERV_PORT);
  pthread_t tid;

  while (1) {
    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);
    int fd = accept(listener_fd, (struct sockaddr *) &ss, &slen);
    if (fd < 0) {
      error(1, errno, "accept failed");
    } else {
      pthread_create(&tid, NULL, &thread_run, (void *) fd);
    }
  }

  return 0;
}
