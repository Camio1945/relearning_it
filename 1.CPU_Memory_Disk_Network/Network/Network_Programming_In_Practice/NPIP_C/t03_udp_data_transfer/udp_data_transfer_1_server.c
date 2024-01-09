#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define    MAXLINE        4096
#define    PORT           9123

static int count = 0;

static void recvfrom_int(int sig_no) {
  printf("\n received %d datagrams\n", count);
  exit(0);
}

struct sockaddr_in get_server_addr() {
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(PORT);
  return server_addr;
}

int main() {
  int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in server_addr = get_server_addr();
  bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
  signal(SIGINT, recvfrom_int);
  printf("Server is listening on port %d ...\n", PORT);

  char message[MAXLINE];

  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);

  while (1) {
    long n = recvfrom(socket_fd, message, MAXLINE, 0, (struct sockaddr *) &client_addr, &client_len);
    message[n] = 0;
    printf("received %ld bytes: %s\n", n, message);

    char send_line[MAXLINE];
    sprintf(send_line, "Hi, %s", message);
    sendto(socket_fd, send_line, strlen(send_line), 0, (struct sockaddr *) &client_addr, client_len);

    count++;
  }
}
