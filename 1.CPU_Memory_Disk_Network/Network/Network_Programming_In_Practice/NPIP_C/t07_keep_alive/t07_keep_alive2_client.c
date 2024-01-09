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

static void sig_int(int signo) {
  printf("\nreceived %d datagrams\n", count);
  exit(0);
}

typedef struct {
  u_int32_t type;
  char data[1024];
} messageObject;

#define MSG_PING          1
#define MSG_PONG          2
#define MSG_TYPE1        11
#define MSG_TYPE2        21
#define    MAXLINE     4096
#define    KEEP_ALIVE_TIME  10
#define    KEEP_ALIVE_INTERVAL  3
#define    KEEP_ALIVE_PROBETIMES  3


int main(int argc, char **argv) {
  int socket_fd;
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

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
  printf("\nConnected success, will send heartbeat in %d seconds\n", KEEP_ALIVE_TIME);

  char recv_line[MAXLINE + 1];
  int n;

  fd_set readmask;
  fd_set allreads;


  struct timeval tv;
  int heartbeats = 0;

  tv.tv_sec = KEEP_ALIVE_TIME;
  tv.tv_usec = 0;

  messageObject messageObject;

  FD_ZERO(&allreads);
  FD_SET(0, &allreads);
  FD_SET(socket_fd, &allreads);
  for (;;) {
    readmask = allreads;
    int rc = select(socket_fd + 1, &readmask, NULL, NULL, &tv);
    if (rc < 0) {
      error(1, errno, "select failed");
    }
    if (rc == 0) {
      if (++heartbeats > KEEP_ALIVE_PROBETIMES) {
        error(1, 0, "connection dead\n");
      }
      printf("sending heartbeat #%d\n", heartbeats);
      messageObject.type = htonl(MSG_PING);
      rc = send(socket_fd, (char *) &messageObject, sizeof(messageObject), 0);
      if (rc < 0) {
        error(1, errno, "send failure");
      }
      tv.tv_sec = KEEP_ALIVE_INTERVAL;
      continue;
    }
    if (FD_ISSET(socket_fd, &readmask)) {
      n = read(socket_fd, recv_line, MAXLINE);
      if (n < 0) {
        error(1, errno, "read error");
      } else if (n == 0) {
        error(1, 0, "server terminated \n");
      }
      printf("received heartbeat, make heartbeats to 0 \n");
      heartbeats = 0;
      tv.tv_sec = KEEP_ALIVE_TIME;
    }
  }
}
