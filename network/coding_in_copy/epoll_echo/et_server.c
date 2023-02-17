#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 50

void error_handling(const char *m) {
  fputs(m, stderr);
  fputc('\n', stderr);
  exit(1);
}

void setnonblockingmode(int fd) {
  int flag = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, flag|O_NONBLOCK);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
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

  int epfd = epoll_create(EPOLL_SIZE);
  struct epoll_event *ep_events = (struct epoll_event *)malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

  setnonblockingmode(server);
  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = server;
  epoll_ctl(epfd, EPOLL_CTL_ADD, server, &event);

  while (1) {
    int event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
    if (event_cnt == -1) {
      puts("epoll_wait() error");
      break;
    }

    for (int i = 0; i < event_cnt; i++) {
      if (ep_events[i].data.fd == server) {
        struct sockaddr_in cliAddr;
        socklen_t addrLen = sizeof(cliAddr);
        int client = accept(server, (struct sockaddr *)&cliAddr, &addrLen);
        if (client == -1) {
          error_handling("accept() error");
        }
        setnonblockingmode(client);
        event.events = EPOLLIN | EPOLLET;
        event.data.fd = client;
        epoll_ctl(epfd, EPOLL_CTL_ADD, client, &event);
        printf("connected client : %d\n", client);
      } else {
        char buffer[BUF_SIZE];
        // edge trigger, read all data in buffer at once
        while (1) {
          int strLen = read(ep_events[i].data.fd, buffer, BUF_SIZE);
          if (strLen == -1) {
            if (errno == EAGAIN) {
              // no data need to be read
              break;
            }
          }
          if (strLen == 0) {
            epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
            close(ep_events[i].data.fd);
            printf("closed client: %d\n", ep_events[i].data.fd);
            break;
          } else {
            write(ep_events[i].data.fd, buffer, strLen);
          }
        }
      }
    }
  }
  close(server);
  close(epfd);
  return 0;
}
