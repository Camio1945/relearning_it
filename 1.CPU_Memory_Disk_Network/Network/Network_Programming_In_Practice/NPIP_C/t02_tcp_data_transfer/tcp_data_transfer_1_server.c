#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static int port = 9123;

size_t readn(int fd, void *buffer, size_t size) {
  char *buffer_pointer = buffer;
  size_t length = size;
  while (length > 0) {
    ssize_t result = read(fd, buffer_pointer, length);
    if (result < 0) {
      if (errno == EINTR) {
        continue; // Consider the non-blocking case, read() needs to be called again
      } else {
        return -1;
      }
    } else if (result == 0) {
      break;
    } else {
      length -= result;
      buffer_pointer += result;
    }
  }
  return size - length; // return the actual read bytes size
}

void read_data(int socket_fd) {
  size_t n;
  char buf[1024];
  int time = 0;
  while (1) {
    fprintf(stdout, "block in read\n");
    n = readn(socket_fd, buf, 1024);
    if (n <= 0) {
      return;
    }
    time++;
    fprintf(stdout, "1K read for the %dth time\n", time);
  }
}

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

/**
 * Bind address to socket
 * @param socket_fd socket file descriptor
 * @param sock_addr socket address
 */
void bind_port(int socket_fd, struct sockaddr_in sock_addr) {
  // Give the socket FD the local address ADDR (which is LEN bytes long).
  int bind_res = bind(socket_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)); // 【重点2】
  if (bind_res < 0) {
    fprintf(stderr, "Bind failed on port : %d", port);
    exit(EXIT_FAILURE);
  }
}

/**
 * Get socket address
 * @return
 */
struct sockaddr_in get_server_addr() {
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(port);
  return server_addr;
}

/**
 * Listen for collection
 * @param listen_fd
 */
void listen_for_connection(int listen_fd) {
  int listen_res = listen(listen_fd, 1024); // 【重点3】
  if (listen_res < 0) {
    fprintf(stderr, "Listening for connection failed : %d", listen_res);
    exit(EXIT_FAILURE);
  }
}

/**
 * Accept connection forever
 * @param listen_fd
 */
void accept_connection_forever(int listen_fd) {
  while (1) {
    fprintf(stdout, "Server listening on port %d and waiting for client to connect...\n", port);
    struct sockaddr_in client_addr;
    socklen_t client_len;
    int connect_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len); // 【重点4】
    fprintf(stdout, "Accepted from client\n");
    read_data(connect_fd);
    close(connect_fd);
  }
}

int main() {
  int listen_fd = ipv4_tcp_socket();
  struct sockaddr_in server_addr = get_server_addr();
  bind_port(listen_fd, server_addr);
  listen_for_connection(listen_fd);
  accept_connection_forever(listen_fd);
  return 0;
}
