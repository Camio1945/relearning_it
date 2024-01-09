#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdarg.h>        /* ANSI C header file */
#include <sys/socket.h>    /* basic socket definitions */
#include <netinet/in.h>    /* sockaddr_in{} and other Internet definitions */
#include <sys/wait.h>
#include <pthread.h>

#define MAX_LINE 16384
#define SERV_PORT 9123
#define    LISTENQ        1024
#define  THREAD_NUMBER      4
#define  BLOCK_QUEUE_SIZE   100

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

void loop_echo(int fd) {
  char outbuf[MAX_LINE + 1];
  size_t outbuf_used = 0;
  ssize_t result;
  while (1) {
    char ch;
    result = recv(fd, &ch, 1, 0);

    //断开连接或者出错
    if (result == 0) {
      break;
    } else if (result == -1) {
      error(1, errno, "read error");
      break;
    }

    if (outbuf_used < sizeof(outbuf)) {
      outbuf[outbuf_used++] = rot13_char(ch);
    }

    if (ch == '\n') {
      send(fd, outbuf, outbuf_used, 0);
      outbuf_used = 0;
      continue;
    }
  }
}

#define  THREAD_NUMBER      4
#define  BLOCK_QUEUE_SIZE   100

extern void loop_echo(int);

typedef struct {
  pthread_t thread_tid;        /* thread ID */
  long thread_count;    /* # connections handled */
} Thread;

Thread *thread_array;

typedef struct {
  int number;
  int *fd;
  int front;
  int rear;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} block_queue;


void block_queue_init(block_queue *blockQueue, int number) {
  blockQueue->number = number;
  blockQueue->fd = calloc(number, sizeof(int));
  blockQueue->front = blockQueue->rear = 0;
  pthread_mutex_init(&blockQueue->mutex, NULL);
  pthread_cond_init(&blockQueue->cond, NULL);
}

void block_queue_push(block_queue *blockQueue, int fd) {
  pthread_mutex_lock(&blockQueue->mutex);
  blockQueue->fd[blockQueue->rear] = fd;
  if (++blockQueue->rear == blockQueue->number) {
    blockQueue->rear = 0;
  }
  printf("push fd %d", fd);
  pthread_cond_signal(&blockQueue->cond);
  pthread_mutex_unlock(&blockQueue->mutex);
}


int block_queue_pop(block_queue *blockQueue) {
  pthread_mutex_lock(&blockQueue->mutex);
  while (blockQueue->front == blockQueue->rear)
    pthread_cond_wait(&blockQueue->cond, &blockQueue->mutex);
  int fd = blockQueue->fd[blockQueue->front];
  if (++blockQueue->front == blockQueue->number) {
    blockQueue->front = 0;
  }
  printf("pop fd %d", fd);
  pthread_mutex_unlock(&blockQueue->mutex);
  return fd;
}

void thread_run(void *arg) {
  pthread_t tid = pthread_self();
  pthread_detach(tid);

  block_queue *blockQueue = (block_queue *) arg;
  while (1) {
    int fd = block_queue_pop(blockQueue);
    printf("get fd in thread, fd==%d, tid == %d", fd, tid);
    loop_echo(fd);
  }
}

int tcp_server_listen(int port) {
  int listenfd;
  listenfd = socket(AF_INET, SOCK_STREAM, 0);

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


int main(int c, char **v) {
  int listener_fd = tcp_server_listen(SERV_PORT);

  block_queue blockQueue;
  block_queue_init(&blockQueue, BLOCK_QUEUE_SIZE);

  thread_array = calloc(THREAD_NUMBER, sizeof(Thread));
  int i;
  for (i = 0; i < THREAD_NUMBER; i++) {
    pthread_create(&(thread_array[i].thread_tid), NULL, &thread_run, (void *) &blockQueue);
  }

  while (1) {
    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);
    int fd = accept(listener_fd, (struct sockaddr *) &ss, &slen);
    if (fd < 0) {
      error(1, errno, "accept failed");
    } else {
      block_queue_push(&blockQueue, fd);
    }
  }

  return 0;
}
