#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

int clnt_cnt = 0;
int clients[MAX_CLNT];
pthread_mutex_t mutex;

void send_msg(char *msg, int len) {
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < clnt_cnt; i++) {
    write(clients[i], msg, len);
  }
  pthread_mutex_unlock(&mutex);
}

void *handle_clnt(void *arg) {
  int client = *((int *)arg);
  char msg[BUF_SIZE];
  int strLen;
  while ((strLen = read(client, msg, sizeof(msg))) != 0) {
    send_msg(msg, strLen);
    fputs("sending done", stderr);
  }
  fputs("not read any more" stderr);
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < clnt_cnt; i++) {
    if (client == clients[i]) {
      while (i++ < clnt_cnt - 1) {
        clients[i] = clients[i+1];
      }
      break;
    }
  }
  clnt_cnt--;
  pthread_mutex_unlock(&mutex);
  close(client);
  return NULL;
}

void error_handling (const char *m) {
  fputs(m, stderr);
  fputc('\n', stderr);
  exit(0);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  pthread_mutex_init(&mutex, NULL);
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

  pthread_t t_id;
  while (1) {
    struct sockaddr_in cliAddr;
    socklen_t addrLen = sizeof(cliAddr);
    int client = accept(server, (struct sockaddr *)&cliAddr, &addrLen);
    if (client == -1) {
      error_handling("accept() error");
    }

    pthread_mutex_lock(&mutex);
    clients[clnt_cnt++] = client;
    pthread_mutex_unlock(&mutex);

    pthread_create(&t_id, NULL, handle_clnt, (void *)&client);
    pthread_detach(t_id);
    printf("Connected client IP : %s\n", inet_ntoa(cliAddr.sin_addr));
  }

  close(server);
  return 0;
}
