#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>        /* ANSI C header file */
#include <sys/socket.h>    /* basic socket definitions */
#include <sys/uio.h>
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

static int count = 0;

static void sig_int(int signo) {
  printf("\nreceived %d datagrams\n", count);
  exit(0);
}

int main(int argc, char **argv) {
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
  printf("\nConnect success, You can input 'world' and 'network' to try out\n");

  char buf[128];
  struct iovec iov[2];

  char *send_one = "hello,";
  iov[0].iov_base = send_one;
  iov[0].iov_len = strlen(send_one);
  iov[1].iov_base = buf;
  while (fgets(buf, sizeof(buf), stdin) != NULL) {
    iov[1].iov_len = strlen(buf);
    // 接下来我们可以看到服务器端接收到了 iovec 组成的新的字符串。这里的原理其实就是在调用
    // writev 操作时，会自动把几个数组的输入合并成一个有序的字节流，然后发送给对端。
    if (writev(socket_fd, iov, 2) < 0) {
      error(1, errno, "writev failure");
    }
  }
  exit(0);
}
