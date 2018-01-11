#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100

void error_handling(const char *m) {
  fputs(m, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(0);
  }

  int server = socket(PF_INET, SOCK_STREAM, 0);
  if (server == -1) {
    error_handling("socket() error");
  }

  struct sockaddr_in serAddr;
  memset(&serAddr, 0, sizeof(serAddr));
  serAddr.sin_family = AF_INET;
  serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serAddr.sin_port = htons(atoi(argv[1]));

  if (bind(server, (struct sockaddr *)&serAddr, sizeof(serAddr)) == -1) {
    error_handling("bind() error");
  }
  if (listen(server, 5) == -1) {
    error_handling("listen() error");
  }

  fd_set reads;
  FD_ZERO(&reads);
  FD_SET(server, &reads);
  int fd_max = server;

  while (1) {
    fd_set backup = reads;
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;
    int fd_num = 0;
    if ((fd_num = select(fd_max+1, &backup, 0, 0, &timeout)) == -1) {
      break;
    }
    if (fd_num == 0) {
      continue;
    }

    for (int i = 0; i <  fd_max+1; i++) {
      if (FD_ISSET(i, &backup)) {
        if (i == server) {
          struct sockaddr_in cliAddr;
          socklen_t addrLen = sizeof(cliAddr);
          int client = accept(server, (struct sockaddr *)&cliAddr, &addrLen);
          if (client == -1) {
            error_handling("accept() error");
          }
          FD_SET(client, &reads);
          if (fd_max < client) {
            fd_max = client;
          }
          printf("connected client : %d\n", client);
        } else {
          char buffer[BUF_SIZE];
          int strLen = read(i, buffer, BUF_SIZE);
          if (strLen == 0) {
            FD_CLR(i, &reads);
            close(i);
            printf("closed client : %d\n", i);
          } else {
            write(i, buffer, strLen);
          }
        }
      }
    }
  }
  close(server);
  return 0;
}
