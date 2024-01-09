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
  if (err) {
    fprintf(stderr, ": %s (%d)\n", strerror(err), err);
  }
  if (status) {
    exit(status);
  }
}


int main() {
  setbuf(stdout, NULL);
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(SERV_PORT);
  int on = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  int bind_res = bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
  if (bind_res < 0) {
    error(1, errno, "bind failed ");
  }
  int listen_res = listen(listen_fd, LISTENQ);
  if (listen_res < 0) {
    error(1, errno, "listen failed ");
  }
  printf("\nServer is listening on port %d\n", SERV_PORT);
  int connect_fd;
  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);
  if ((connect_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len)) < 0) {
    error(1, errno, "bind failed ");
  }
  char message[MAXLINE] = "Hello, client!";
  ssize_t write_res = write(connect_fd, message, strlen(message));
  if (write_res < 0) {
    error(1, errno, "write failed ");
  }
  printf("Send : %s\n", message);
  for (;;) {
    long n = read(connect_fd, message, MAXLINE);
    if (n < 0) {
      error(1, errno, "error read");
    } else if (n == 0) {
      error(1, 0, "client closed \n");
    }
    message[n] = 0;
    printf("Received from client: %s\n", message);

    char send_line[MAXLINE];
    sprintf(send_line, "Hi, %s", message);

    long write_nc = write(connect_fd, send_line, strlen(send_line));
    printf("Send : %s\n", send_line);
    if (write_nc < 0) {
      error(1, errno, "error write");
    }
  }
}
