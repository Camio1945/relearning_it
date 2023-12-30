#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

static char *server_ip = "127.0.0.1";
static int server_port = 9123;
#define MESSAGE_SIZE 102400

/**
 * Create a IPV4 TCP socket
 * @return the file descriptor of the created socket
 */
int ipv4_tcp_socket() {
  // PF_INET: ipv4 ; SOCK_STREAM: byte stream (TCP) ; 0: automatic protocol
  int socket_fd = socket(PF_INET, SOCK_STREAM, 0); // 【重点1】
  if (socket_fd < 0) {
    perror("Init socket failed");
    exit(EXIT_FAILURE);
  }
  return socket_fd;
}

struct sockaddr_in get_server_addr() {
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(server_port);
  inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
  return server_addr;
}

/**
 * Connect to server
 * @param client_socket_fd Client socket file descriptor
 * @param server_addr
 */
void connect_to_server(int client_socket_fd, struct sockaddr_in server_addr) {
  int connect_res = connect(client_socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)); // 【重点2】
  if (connect_res < 0) {
    perror("Connect to server failed");
    exit(EXIT_FAILURE);
  } else {
    fprintf(stdout, "Connected to server.\n");
  }
}

/**
 * print a diagnostic and optionally exit
 * @param status exit program if status > 0
 * @param err error number
 * @param fmt string format
 * @param ... string format args
 */
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

void send_data(int socket_fd) {
  char *query = malloc(MESSAGE_SIZE + 1);
  for (int i = 0; i < MESSAGE_SIZE; ++i) {
    query[i] = 'a';
  }
  query[MESSAGE_SIZE] = '\0';

  const char *cp = query;
  size_t remaining = strlen(query);
  while (remaining) {
    int n_written = send(socket_fd, cp, remaining, 0);
    fprintf(stdout, "Send into buffer %ld\n", n_written);
    if (n_written <= 0) {
      error(1, errno, "send failed");
      return;
    }
    remaining -= n_written;
    cp += n_written;
  }
}

int main() {
  int client_socket_fd = ipv4_tcp_socket();
  struct sockaddr_in server_addr = get_server_addr();
  connect_to_server(client_socket_fd, server_addr);
  send_data(client_socket_fd);
  return 0;
}

