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
  // sAdr.sin_addr.s_addr = htonl(atoi(argv[1]));
  sAdr.sin_addr.s_addr = inet_addr(argv[1]);
  sAdr.sin_port = htons(atoi(argv[2]));

  if (connect(csock, (struct sockaddr *)&sAdr, sizeof(sAdr)) == -1) {
    error_handling("connect() error");
  } else {
    puts("Connected........");
  }

  while (1) {
    char buf[BUF_SIZE];
    fputs("input q/Q to exit\n", stdout);
    fgets(buf, BUF_SIZE, stdin);
    if ((0 == strcmp(buf, "q\n")) || (0 == strcmp(buf, "Q\n"))) {
      break;
    }

    int wlen = write(csock, buf, strlen(buf));

    char message[BUF_SIZE];
    int rlen = 0;
    while (rlen < wlen) {
      int len = read(csock, &message[rlen], BUF_SIZE);
      if (len == -1) {
        error_handling("read() error");
      }
      rlen += len;
    }
    printf("message from server : %s\n", message);
  }

  close(csock);
  return 0;
}
