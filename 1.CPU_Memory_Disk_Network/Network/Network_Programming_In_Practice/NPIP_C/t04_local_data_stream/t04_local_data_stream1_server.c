#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/un.h>

int main() {
  int listen_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
  assert(listen_fd > 0);
  char *local_path = "/tmp/t04_local_data_stream.sock";
  unlink(local_path);

  struct sockaddr_un server_addr, client_addr;
  server_addr.sun_family = AF_LOCAL;
  strcpy(server_addr.sun_path, local_path);

  int bind_res = bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
  assert(bind_res == 0);
  listen(listen_fd, 1024);
  printf("\nServer is listening on %s ...\n", local_path);

  socklen_t client_len = sizeof(client_addr);
  int conn_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);

  char message[1024];
  while (1) {
    long n = read(conn_fd, message, 1024);
    message[n] = 0;
    printf("Received from client: %s\n", message);

    char send_line[1024];
    sprintf(send_line, "Hi, %s", message);
    write(conn_fd, send_line, strlen(send_line));
  }
}
