#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <assert.h>

int main() {
  setbuf(stdout, NULL);
  int socket_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
  assert(socket_fd > 0);
  struct sockaddr_un server_addr;
  server_addr.sun_family = AF_LOCAL;
  strcpy(server_addr.sun_path, "/tmp/t04_local_data_stream.sock");

  int connect_res = connect(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
  assert(connect_res == 0);
  printf("\nPlease input the message you want send to the server:\n");

  char send_line[1024];
  char recv_line[1024];

  while (fgets(send_line, 1024, stdin) != NULL) {
    int nbytes = sizeof(send_line);
    if (write(socket_fd, send_line, nbytes) != nbytes) {
      printf("write error\n");
    }

    if (read(socket_fd, recv_line, 1024) == 0) {
      printf("server terminated prematurely\n");
    }

    fputs(recv_line, stdout);
  }
  return 0;
}
