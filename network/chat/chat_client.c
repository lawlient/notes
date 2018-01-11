#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

void *send_msg(void *arg) {
  int sock = *((int *)arg);
  char name_msg[NAME_SIZE + BUF_SIZE];
  while (1) {
    fgets(msg, BUF_SIZE, stdin);
    if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
      close(sock);
      exit(0);
    }
    sprintf(name_msg, "%s %s", name, msg);
    write(sock, name_msg, strlen(name_msg));
  }
  return NULL;
}

void *recv_msg(void *arg) {
  int sock = *((int *)arg);
  char name_msg[NAME_SIZE + BUF_SIZE];
  while (1) {
    int strLen = read(sock, name_msg, NAME_SIZE + BUF_SIZE - 1);
    if (strLen == -1)
      return (void *)-1;
    name_msg[strLen] = 0;
    fputs(name_msg, stdout);
  }
  return NULL;
}

void error_handling (const char *m) {
  fputs(m, stderr);
  fputc('\n', stderr);
  exit(0);
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Usage : %s <IP> <port> <name>\n", argv[0]);
    exit(1);
  }

  sprintf(name, "[%s]", argv[3]);
  int client = socket(PF_INET, SOCK_STREAM, 0);
  if (client == -1) {
    error_handling("socket() error");
  }

  struct sockaddr_in serAddr;
  memset(&serAddr, 0, sizeof(serAddr));
  serAddr.sin_family = AF_INET;
  serAddr.sin_addr.s_addr = inet_addr(argv[1]);
  serAddr.sin_port = htons(atoi(argv[2]));

  if (connect(client, (struct sockaddr *)&serAddr, sizeof(serAddr)) == -1) {
    error_handling("connect() error");
  }

  pthread_t send, recv;
  pthread_create(&send, NULL, send_msg, (void *)&client);
  pthread_create(&recv, NULL, recv_msg, (void *)&client);

  void *thread_return;
  pthread_join(send, &thread_return);
  pthread_join(recv, &thread_return);

  close(client);
  return 0;
}
