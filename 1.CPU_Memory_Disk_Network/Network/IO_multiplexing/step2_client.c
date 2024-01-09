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

int main(int argc, char **argv) {
  setbuf(stdout, NULL);
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERV_PORT);
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

  socklen_t server_len = sizeof(server_addr);
  int connect_rt = connect(socket_fd, (struct sockaddr *) &server_addr, server_len);
  if (connect_rt < 0) {
    error(1, errno, "connect failed ");
  }
  printf("\nInput the message you want sent to the server:\n");

  char send_line[MAXLINE], recv_line[MAXLINE + 1];
  for (;;) {
    if (fgets(send_line, MAXLINE, stdin) != NULL) {
      int i = strlen(send_line);
      if (send_line[i - 1] == '\n') {
        send_line[i - 1] = 0;
      }
      write(socket_fd, send_line, strlen(send_line));
      printf("Send : %s\n", send_line);
    }
    int n = read(socket_fd, recv_line, MAXLINE);
    if (n < 0) {
      error(1, errno, "read error");
    } else if (n == 0) {
      error(1, 0, "server terminated \n");
    }
    recv_line[n] = 0;
    printf("Received from server: %s\n", recv_line);
  }
}
