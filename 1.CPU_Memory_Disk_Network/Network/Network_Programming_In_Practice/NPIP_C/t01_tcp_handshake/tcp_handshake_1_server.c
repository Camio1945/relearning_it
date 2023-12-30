#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

static int port = 9123;

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
    accept(listen_fd, (struct sockaddr *) &client_addr, &client_len); // 【重点4】
    fprintf(stdout, "Accepted from client\n");
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
