#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

void error_handling(const char *m) {
  fputs(m, stderr);
  fputc('\n', stderr);
  exit(1);
}

void read_routing(int sock, char *buf) {
  while (1) {
    int strLen = read(sock, buf, BUF_SIZE);
    if (strLen == 0) {
      return;
    }
    buf[strLen] = 0;
    printf("Message from server : %s\n", buf);
  }
}

void write_routing(int sock, char *buf) {
  while (1) {
    fgets(buf, BUF_SIZE, stdin);
    if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
      shutdown(sock, SHUT_WR);
      return;
    }
    write(sock, buf, strlen(buf));
  }
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage : %s <IP> <PORT> \n", argv[0]);
    exit(1);
  }

  int csock = socket(PF_INET, SOCK_STREAM, 0);
  if (csock == -1) {
    error_handling("socket() error");
  }

  struct sockaddr_in sAdr;
  memset(&sAdr, 0, sizeof(sAdr));
  sAdr.sin_family = AF_INET;
  sAdr.sin_addr.s_addr = inet_addr(argv[1]);
  sAdr.sin_port = htons(atoi(argv[2]));

  if (connect(csock, (struct sockaddr *)&sAdr, sizeof(sAdr)) == -1) {
    error_handling("connect() error");
  } else {
    puts("Connected........");
  }

  pid_t pid = fork();
  char buf[BUF_SIZE];
  if (pid == 0) {
    write_routing(csock, buf);
  } else {
    read_routing(csock, buf);
  }

  close(csock);
  return 0;
}
