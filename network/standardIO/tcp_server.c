#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

void error_handling(const char *m) {
  fputs(m, stderr);
  fputc('\n', stderr);
  exit(1);
}

void change(char *s, int len) {
  for (int i = 0; i < len; i++) {
    s[i] = toupper(s[i]);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage : %s <PORT>\n", argv[0]);
    exit(1);
  }

  int ssock = socket(PF_INET, SOCK_STREAM, 0);
  if (ssock == -1) {
    error_handling("socket() error");
  }

  // set SO_REUSEADDR to avoid TIME_WAIT
  int option = true;
  socklen_t optlen = sizeof(option);
  setsockopt(ssock, SOL_SOCKET, SO_REUSEADDR, (void *)&option, optlen);

  struct sockaddr_in sAdr;
  memset(&sAdr, 0, sizeof(sAdr));
  sAdr.sin_family = AF_INET;
  sAdr.sin_addr.s_addr = htonl(INADDR_ANY);
  sAdr.sin_port = htons(atoi(argv[1]));

  if (bind(ssock, (struct sockaddr *)&sAdr, sizeof(sAdr)) == -1) {
    error_handling("bind() error");
  }

  if (listen(ssock, 5) == -1) {
    error_handling("listen() error");
  }

  struct sockaddr_in cAdr;
  socklen_t clnt_adr_size = sizeof(cAdr);

  for (int i = 0; i < 5; i++) {
    int csock = accept(ssock, (struct sockaddr *)&cAdr, &clnt_adr_size);
    if (csock == -1) {
      error_handling("accept() error");
    } else {
      printf("Connect Client %d \n", i+1);
    }

    FILE *readfp = fdopen(csock, "r");
    FILE *writefp = fdopen(csock, "w");
    char buf[BUF_SIZE];
    while (!feof(readfp)) {
      fgets(buf, BUF_SIZE, readfp);
      change(buf, strlen(buf));
      fputs(buf, writefp);
      fflush(writefp);
    }
    fclose(readfp);
    fclose(writefp);
  }

  close(ssock);
  return 0;
}
