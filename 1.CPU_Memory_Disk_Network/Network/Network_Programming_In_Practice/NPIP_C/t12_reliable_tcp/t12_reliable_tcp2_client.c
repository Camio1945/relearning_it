#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
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

# define MESSAGE_SIZE 102400000

int tcp_client(char *address, int port) {
  int socket_fd;
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  inet_pton(AF_INET, address, &server_addr.sin_addr);

  socklen_t server_len = sizeof(server_addr);
  int connect_rt = connect(socket_fd, (struct sockaddr *) &server_addr, server_len);
  if (connect_rt < 0) {
    error(1, errno, "connect failed ");
  }

  return socket_fd;
}

int main(int argc, char **argv) {
  int socket_fd = tcp_client("127.0.0.1", SERV_PORT);
  char buf[129];
  int len;
  int rc;

  while (fgets(buf, sizeof(buf), stdin) != NULL) {
    len = strlen(buf);
    rc = send(socket_fd, buf, len, 0);
    if (rc < 0)
      error(1, errno, "write failed");
    sleep(3);
    rc = read(socket_fd, buf, sizeof(buf));
    if (rc < 0)
      error(1, errno, "read failed");
    else if (rc == 0)
      error(1, 0, "peer connection closed\n");
    else
      fputs(buf, stdout);
  }
  exit(0);
}

