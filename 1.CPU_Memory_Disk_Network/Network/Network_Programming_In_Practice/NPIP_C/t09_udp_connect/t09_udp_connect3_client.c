#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>        /* ANSI C header file */
#include <sys/socket.h>    /* basic socket definitions */
#include <netinet/in.h>    /* sockaddr_in{} and other Internet definitions */
#include <arpa/inet.h>

#define MAXLINE 4096
#define SERV_PORT 9123

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

int main(int argc, char **argv) {
  int socket_fd;
  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERV_PORT);
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

  socklen_t server_len = sizeof(server_addr);

  if (connect(socket_fd, (struct sockaddr *) &server_addr, server_len)) {
    error(1, errno, "connect failed");
  }
  printf("\nStart server, then start client 1 (aka this t09_udp_connect3_client.c), then start up client 2 (aka this t09_udp_connect3_client.c), use both clients to send message to the server, the second client will be refused\n");

  char send_line[MAXLINE], recv_line[MAXLINE + 1];
  int n;

  while (fgets(send_line, MAXLINE, stdin) != NULL) {
    int i = strlen(send_line);
    if (send_line[i - 1] == '\n') {
      send_line[i - 1] = 0;
    }

    printf("now sending %s\n", send_line);
    size_t rt = send(socket_fd, send_line, strlen(send_line), 0);
    printf("send bytes: %zu \n", rt);

    recv_line[0] = 0;
    n = recv(socket_fd, recv_line, MAXLINE, 0);
    if (n < 0) {
      error(1, errno, "recv failed");
    }
    recv_line[n] = 0;
    fputs(recv_line, stdout);
    fputs("\n", stdout);
  }

  exit(0);
}
