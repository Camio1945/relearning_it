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

static int count = 0;

static void sig_int(int signo) {
  printf("\nreceived %d datagrams\n", count);
  exit(0);
}

int main(int argc, char **argv) {
  setbuf(stdout, NULL);
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(SERV_PORT);

  int rt1 = bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
  if (rt1 < 0) {
    error(1, errno, "bind failed ");
  }

  int rt2 = listen(listenfd, LISTENQ);
  if (rt2 < 0) {
    error(1, errno, "listen failed ");
  }
  printf("\nServer is listening on port %d\n", SERV_PORT);

  signal(SIGINT, sig_int);
  signal(SIGPIPE, SIG_DFL);

  int connfd;
  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);

  if ((connfd = accept(listenfd, (struct sockaddr *) &client_addr, &client_len)) < 0) {
    error(1, errno, "bind failed ");
  }

  char message[MAXLINE];
  count = 0;

  for (;;) {
    long n = read(connfd, message, MAXLINE);
    if (n < 0) {
      error(1, errno, "error read");
    } else if (n == 0) {
      error(1, 0, "client closed \n");
    }
    message[n] = 0;
    printf("received %d bytes: %s\n", n, message);
    count++;

    char send_line[MAXLINE];
    sprintf(send_line, "Hi, %s", message);

    printf("The server will sleep for 5 seconds to send a response to the client.\n");
    sleep(5);

    long write_nc = send(connfd, send_line, strlen(send_line), 0);
    printf("send bytes: %zu \n", write_nc);
    if (write_nc < 0) {
      error(1, errno, "error write");
    }
  }

}
