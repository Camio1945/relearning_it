#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdarg.h>        /* ANSI C header file */
#include <sys/socket.h>    /* basic socket definitions */
#include <netinet/in.h>    /* sockaddr_in{} and other Internet definitions */
#include <fcntl.h>

#define MAX_LINE 1024
#define SERV_PORT 9123
#define LISTENQ 1024
#define FD_INIT_SIZE 128

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


char rot13_char(char c) {
  if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
    return c + 13;
  else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
    return c - 13;
  else
    return c;
}

//数据缓冲区
struct Buffer {
  int connect_fd;  //连接字
  char buffer[MAX_LINE];  //实际缓冲
  size_t writeIndex;      //缓冲写入位置
  size_t readIndex;       //缓冲读取位置
  int readable;           //是否可以读
};


//分配一个Buffer对象，初始化writeIdnex和readIndex等
struct Buffer *alloc_Buffer() {
  struct Buffer *buffer = malloc(sizeof(struct Buffer));
  if (!buffer)
    return NULL;
  buffer->connect_fd = 0;
  buffer->writeIndex = buffer->readIndex = buffer->readable = 0;
  return buffer;
}

//释放Buffer对象
void free_Buffer(struct Buffer *buffer) {
  free(buffer);
}

//这里从fd套接字读取数据，数据先读取到本地buf数组中，再逐个拷贝到buffer对象缓冲中
int onSocketRead(int fd, struct Buffer *buffer) {
  char buf[1024];
  int i;
  ssize_t result;
  while (1) {
    result = recv(fd, buf, sizeof(buf), 0);
    if (result <= 0)
      break;

    //按char对每个字节进行拷贝，每个字节都会先调用rot13_char来完成编码，之后拷贝到buffer对象的缓冲中，
    //其中writeIndex标志了缓冲中写的位置
    for (i = 0; i < result; ++i) {
      if (buffer->writeIndex < sizeof(buffer->buffer))
        buffer->buffer[buffer->writeIndex++] = rot13_char(buf[i]);
      //如果读取了回车符，则认为client端发送结束，此时可以把编码后的数据回送给客户端
      if (buf[i] == '\n') {
        buffer->readable = 1;  //缓冲区可以读
      }
    }
  }

  if (result == 0) {
    return 1;
  } else if (result < 0) {
    if (errno == EAGAIN)
      return 0;
    return -1;
  }

  return 0;
}

//从buffer对象的readIndex开始读，一直读到writeIndex的位置，这段区间是有效数据
int onSocketWrite(int fd, struct Buffer *buffer) {
  while (buffer->readIndex < buffer->writeIndex) {
    ssize_t result = send(fd, buffer->buffer + buffer->readIndex, buffer->writeIndex - buffer->readIndex, 0);
    if (result < 0) {
      if (errno == EAGAIN)
        return 0;
      return -1;
    }

    buffer->readIndex += result;
  }

  //readindex已经追上writeIndex，说明有效发送区间已经全部读完，将readIndex和writeIndex设置为0，复用这段缓冲
  if (buffer->readIndex == buffer->writeIndex)
    buffer->readIndex = buffer->writeIndex = 0;

  //缓冲数据已经全部读完，不需要再读
  buffer->readable = 0;

  return 0;
}


void make_nonblocking(int fd) {
  fcntl(fd, F_SETFL, O_NONBLOCK); // 【重点】
}


int tcp_nonblocking_server_listen(int port) {
  int listenfd;
  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  make_nonblocking(listenfd);

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

  return listenfd;
}


int main(int argc, char **argv) {
  int listen_fd;
  int i, maxfd;

  struct Buffer *buffer[FD_INIT_SIZE];
  for (i = 0; i < FD_INIT_SIZE; ++i) {
    buffer[i] = alloc_Buffer();
  }

  listen_fd = tcp_nonblocking_server_listen(SERV_PORT);

  fd_set readset, writeset, exset;
  FD_ZERO(&readset);
  FD_ZERO(&writeset);
  FD_ZERO(&exset);

  while (1) {
    maxfd = listen_fd;

    FD_ZERO(&readset);
    FD_ZERO(&writeset);
    FD_ZERO(&exset);

    // listener加入readset
    FD_SET(listen_fd, &readset);

    for (i = 0; i < FD_INIT_SIZE; ++i) {
      if (buffer[i]->connect_fd > 0) {
        if (buffer[i]->connect_fd > maxfd)
          maxfd = buffer[i]->connect_fd;
        FD_SET(buffer[i]->connect_fd, &readset);
        if (buffer[i]->readable) {
          FD_SET(buffer[i]->connect_fd, &writeset);
        }
      }
    }

    if (select(maxfd + 1, &readset, &writeset, &exset, NULL) < 0) {
      error(1, errno, "select error");
    }

    if (FD_ISSET(listen_fd, &readset)) {
      printf("listening socket readable\n");
      sleep(5);
      struct sockaddr_storage ss;
      socklen_t slen = sizeof(ss);
      int fd = accept(listen_fd, (struct sockaddr *) &ss, &slen);
      if (fd < 0) {
        error(1, errno, "accept failed");
      } else if (fd > FD_INIT_SIZE) {
        error(1, 0, "too many connections");
        close(fd);
      } else {
        make_nonblocking(fd);
        if (buffer[fd]->connect_fd == 0) {
          buffer[fd]->connect_fd = fd;
        } else {
          error(1, 0, "too many connections");
        }
      }
    }

    for (i = 0; i < maxfd + 1; ++i) {
      int r = 0;
      if (i == listen_fd)
        continue;

      if (FD_ISSET(i, &readset)) {
        r = onSocketRead(i, buffer[i]);
      }
      if (r == 0 && FD_ISSET(i, &writeset)) {
        r = onSocketWrite(i, buffer[i]);
      }
      if (r) {
        buffer[i]->connect_fd = 0;
        close(i);
      }
    }
  }
}
