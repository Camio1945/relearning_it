#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdarg.h>        /* ANSI C header file */
#include <sys/socket.h>    /* basic socket definitions */
#include <netinet/in.h>    /* sockaddr_in{} and other Internet definitions */

#define MAXLINE 4096
#define SERV_PORT 9123
#define LISTENQ 1024

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

int tcp_server(int port) {
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

  int connfd;
  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);

  if ((connfd = accept(listenfd, (struct sockaddr *) &client_addr, &client_len)) < 0) {
    error(1, errno, "bind failed ");
  }

  return connfd;
}

void print_test_step() {
  setbuf(stdout, NULL);
  printf("\n");
  printf("1. Start server\n");
  printf("2. Start client and input message like 'good'\n");
  printf("3. Stop server as soon as possible\n");
  printf("The client should print a 'peer connection closed' message, this is because when we close the server, the operating system will send a FIN packet to client.\n");
}

int main(int argc, char **argv) {
  int connfd;
  char buf[1024];

  print_test_step();
  connfd = tcp_server(SERV_PORT);

  for (;;) {
    int n = read(connfd, buf, 1024);
    if (n < 0) {
      error(1, errno, "error read");
    } else if (n == 0) {
      error(1, 0, "client closed \n");
    }

    sleep(5);

    int write_nc = send(connfd, buf, n, 0);
    printf("send bytes: %zu \n", write_nc);
    if (write_nc < 0) {
      error(1, errno, "error write");
    }
  }
}
