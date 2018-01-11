#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>


void error_handling(const char *m) {
  fputs(m, stderr);
  fputc('\n', stderr);
  exit(0);
}

void printhost(const hostent *host) {
  printf("host name : %s\n", host->h_name);

  for (int i = 0; host->h_aliases[i]; i++) {
    printf("Aliase %d : %s\n", i+1, host->h_aliases[i]);
  }

  printf("Address Type : %s\n", (host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6"));

  for (int i = 0; host->h_addr_list[i]; i++) {
    printf("Address %d : %s\n", i+1, inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
  }
}

int main() {
  struct hostent *host = nullptr;
  while (1) {
    char buffer[30];
    printf("chose method : 1. hostname, 2. address, 3. exit\n");
    int method;
    scanf("%d", &method);
    fgetc(stdin);
    if (method == 1) {
      printf("please input host name : ");
      scanf("%s", buffer);
      fgetc(stdin);
      host = gethostbyname(buffer);
    } else if(method == 2) {
      printf("please input host address : ");
      scanf("%s", buffer);
      fgetc(stdin);
      struct sockaddr_in addr;
      memset(&addr, 0, sizeof(addr));
      addr.sin_addr.s_addr = inet_addr(buffer);
      host = gethostbyaddr((char *)&addr.sin_addr, 4, AF_INET);
    } else if (method == 3) {
      break;
    } else {
      continue;
    }
    if (!host) {
      continue;
    }

    printhost(host);
  }
  return 0;
}
