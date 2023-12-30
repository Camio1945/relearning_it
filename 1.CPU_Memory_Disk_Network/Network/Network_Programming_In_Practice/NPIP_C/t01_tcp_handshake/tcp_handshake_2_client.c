#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

static char *server_ip = "127.0.0.1";
static int server_port = 9123;

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

int main() {
  int client_socket_fd = ipv4_tcp_socket();
  struct sockaddr_in server_addr = get_server_addr();
  connect_to_server(client_socket_fd, server_addr);
  return 0;
}

