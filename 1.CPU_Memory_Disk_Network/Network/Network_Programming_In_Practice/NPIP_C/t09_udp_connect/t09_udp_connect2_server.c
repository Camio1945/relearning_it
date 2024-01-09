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

static int count;

static void recvfrom_int(int signo) {
  printf("\nreceived %d datagrams\n", count);
  exit(0);
}

int main(int argc, char **argv) {
  int socket_fd;
  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(SERV_PORT);

  bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));

  socklen_t client_len;
  char message[MAXLINE];
  message[0] = 0;
  count = 0;

  signal(SIGINT, recvfrom_int);

  struct sockaddr_in client_addr;
  client_len = sizeof(client_addr);

  int n = recvfrom(socket_fd, message, MAXLINE, 0, (struct sockaddr *) &client_addr, &client_len);
  if (n < 0) {
    error(1, errno, "recvfrom failed");
  }
  message[n] = 0;
  printf("received %d bytes: %s\n", n, message);

  if (connect(socket_fd, (struct sockaddr *) &client_addr, client_len)) {
    error(1, errno, "connect failed");
  }
  printf("Server is up...\n");

  while (strncmp(message, "goodbye", 7) != 0) {
    char send_line[MAXLINE];
    sprintf(send_line, "Hi, %s", message);

    size_t rt = send(socket_fd, send_line, strlen(send_line), 0);
    if (rt < 0) {
      error(1, errno, "send failed ");
    }
    printf("send bytes: %zu \n", rt);

    size_t rc = recv(socket_fd, message, MAXLINE, 0);
    if (rc < 0) {
      error(1, errno, "recv failed");
    }

    count++;
  }

  exit(0);
}
