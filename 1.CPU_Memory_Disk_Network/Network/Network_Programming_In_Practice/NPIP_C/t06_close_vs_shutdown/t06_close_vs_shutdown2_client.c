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
  printf(
      "\nInput the message you want sent to the server, and when you are done, you can try 'close' or 'shutdown' to finish the connection:\n");

  char send_line[MAXLINE], recv_line[MAXLINE + 1];
  int n;

  fd_set readmask;
  fd_set allreads;

  FD_ZERO(&allreads);
  FD_SET(0, &allreads);
  FD_SET(socket_fd, &allreads);
  for (;;) {
    readmask = allreads; // 【重点】每次测试完之后，重新设置待测试的描述符集合
    // select 使得我们可以同时完成对连接套接字和标准输入两个 I/O 对象的处理。【重点】切记需要+1
    int rc = select(socket_fd + 1, &readmask, NULL, NULL, NULL);
    if (rc <= 0) {
      error(1, errno, "select failed");
    }
    if (FD_ISSET(socket_fd, &readmask)) {
      n = read(socket_fd, recv_line, MAXLINE);
      if (n < 0) {
        error(1, errno, "read error");
      } else if (n == 0) {
        error(1, 0, "server terminated \n");
      }
      recv_line[n] = 0;
      fputs(recv_line, stdout);
      fputs("\n", stdout);
    }
    if (FD_ISSET(0, &readmask)) {
      if (fgets(send_line, MAXLINE, stdin) != NULL) {
        if (strncmp(send_line, "shutdown", 8) == 0) {
          FD_CLR(0, &allreads);
          if (shutdown(socket_fd, 1)) {
            error(1, errno, "shutdown failed");
          }
        } else if (strncmp(send_line, "close", 5) == 0) {
          FD_CLR(0, &allreads);
          if (close(socket_fd)) {
            error(1, errno, "close failed");
          }
          sleep(6);
          exit(0);
        } else {
          int i = strlen(send_line);
          if (send_line[i - 1] == '\n') {
            send_line[i - 1] = 0;
          }

          printf("now sending %s\n", send_line);
          size_t rt = write(socket_fd, send_line, strlen(send_line));
          if (rt < 0) {
            error(1, errno, "write failed ");
          }
          printf("send bytes: %zu \n", rt);
        }

      }
    }

  }

}
