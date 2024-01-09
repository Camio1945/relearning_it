#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define    MAXLINE        4096
#define    PORT           9123

struct sockaddr_in get_server_addr() {
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, "localhost", &server_addr.sin_addr);
  return server_addr;
}

int main() {
  int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in server_addr = get_server_addr();
  socklen_t server_len = sizeof(server_addr);
  struct sockaddr *reply_addr = malloc(server_len);
  char send_line[MAXLINE], recv_line[MAXLINE + 1];
  printf("Please input the message you want to send to the server:\n");
  while (fgets(send_line, MAXLINE, stdin) != NULL) {
    int i = strlen(send_line);
    if (send_line[i - 1] == '\n') {
      send_line[i - 1] = 0;
    }
    printf("now sending %s\n", send_line);
    size_t rt = sendto(socket_fd, send_line, strlen(send_line), 0, (struct sockaddr *) &server_addr, server_len);
    printf("send bytes: %zu\n", rt);

    int n = recvfrom(socket_fd, recv_line, MAXLINE, 0, reply_addr, &server_len);
    if (n < 0) {
      printf("receive failed\n");
      exit(EXIT_FAILURE);
    }
    fputs(recv_line, stdout);
    fputs("\n", stdout);
  }

  return 0;
}
