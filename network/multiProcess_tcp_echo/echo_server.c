#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <signal.h>

#define BUF_SIZE 1024

void read_childproc(int sig) {
  int status;
  pid_t id = waitpid(-1, &status, WNOHANG);
  printf("removed child process id : %d\n", id);
}

void error_handling(const char *m) {
  fputs(m, stderr);
  fputc('\n', stderr);
  exit(0);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  struct sigaction act;
  act.sa_handler = read_childproc;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(SIGCHLD, &act, 0);

  int server = socket(PF_INET, SOCK_STREAM, 0);
  if (server == -1) {
    error_handling("socket() error");
  }

  struct sockaddr_in serAddr;
  memset(&serAddr, 0, sizeof(serAddr));
  serAddr.sin_family = AF_INET;
  serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serAddr.sin_port = htons(atoi(argv[1]));

  if (bind(server, (struct sockaddr *)&serAddr, sizeof(serAddr))) {
    error_handling("bind() error");
  }

  if (listen(server, 5)) {
    error_handling("listen() error");
  }

  int fds[2];
  pipe(fds);
  pid_t pid = fork();
  // reserve msg into file
  if (pid == 0) {
    FILE *fp = fopen("echomsg.txt", "wt");
    char msgbuf[BUF_SIZE];
    for (int i = 0; i < 5; i++) {
      int len = read(fds[0], msgbuf, BUF_SIZE);
      fwrite((void *)msgbuf, 1, len, fp);
    }
    fclose(fp);
    return 0;
  }

  while (1) {
    struct sockaddr_in cliAddr;
    socklen_t addrLen = sizeof(cliAddr);
    int client = accept(server, (struct sockaddr *)&cliAddr, &addrLen);
    if (client == -1) {
      continue;
    } else {
      printf("connect client : \n");
    }
    pid_t pid = fork();
    if (pid == -1) {
      printf("fork() failed\n");
      close(client);
      continue;
    } else if (pid == 0) {
      close(server);
      char buffer[BUF_SIZE];
      while(int strLen = read(client, buffer, BUF_SIZE)) {
        write(client, buffer, strLen);
        write(fds[1], buffer, strLen);
      }
      close(client);
      puts("client disconnect...\n");
      return 0;
    } else {
      close(client);
    }
  }
  close(server);
  return 0;
}
