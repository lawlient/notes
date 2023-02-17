/*
 * add a html file, content doesn't matter
 * open a browser, like firefox
 * input ‘http://localhost:port/filename.html' in address filed
 * test whether the program can read date from the html file or not
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void *request_handler(void* arg);
void send_data(FILE* fp, char* ct, char* file_name);
char* content_type(char* file);
void send_error(FILE *fp);
void error_handling(char* message);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  // set SO_REUSEADDR to avoid TIME_WAIT
  int option = 1;
  socklen_t optlen = sizeof(option);
  setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&option, optlen);

  struct sockaddr_in serv_adr;
  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));

  if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
    error_handling("bind() error");
  }

  if (listen(serv_sock, 20) == -1) {
    error_handling("listen() error");
  }

  while (1) {
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_size = sizeof(clnt_adr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_size);
    printf("Connection Request : %s:%d\n", inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));
    pthread_t t_id;
    pthread_create(&t_id, NULL, request_handler, &clnt_sock);
    pthread_detach(t_id);
  }

  close(serv_sock);
  return 0;
}

void* request_handler(void *arg) {
  int clnt_sock = *((int *)arg);
  char req_line[SMALL_BUF];

  FILE *clnt_read = fdopen(clnt_sock, "r");
  FILE *clnt_write = fdopen(dup(clnt_sock), "w");
  fgets(req_line, SMALL_BUF, clnt_read);
  if (strstr(req_line, "HTTP/") == NULL) {
    send_error(clnt_write);
    fclose(clnt_read);
    fclose(clnt_write);
    return 0;
  }

  char method[10];
  strcpy(method, strtok(req_line, " /"));
  if (strcmp(method, "GET") != 0) {
    send_error(clnt_write);
    fclose(clnt_read);
    fclose(clnt_write);
    return 0;
  }

  char file_name[30];
  strcpy(file_name, strtok(NULL, " /"));

  char ct[15];
  strcpy(ct, content_type(file_name));

  fclose(clnt_read);
  send_data(clnt_write, ct, file_name);
  fclose(clnt_write);
  return 0;
}

void send_data(FILE *fp, char *ct, char *file_name) {
  char protocol[] = "HTTP/1.0 200 OK\r\n";
  char server[] = "Server:Linux Web Server \r\n";
  char cnt_len[] = "content-length:2048\r\n";

  char cnt_type[SMALL_BUF];
  sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
  FILE *send_file = fopen(file_name, "r");
  if (send_file == NULL) {
    send_error(fp);
    return;
  }

  /*  传输头信息 */
  fputs(protocol, fp);
  fputs(server, fp);
  fputs(cnt_len, fp);
  fputs(cnt_type, fp);

  /* 传输请求数据 */
  char buf[BUF_SIZE];
  while (fgets(buf, BUF_SIZE, send_file) != NULL) {
    fputs(buf, fp);
    fflush(fp);
  }
  fflush(fp);
}

char *content_type(char *file) {
  char extension[SMALL_BUF];
  char file_name[SMALL_BUF];
  strcpy(file_name, file);
  strtok(file_name, ".");
  strcpy(extension, strtok(NULL, "."));

  if (!strcmp(extension, "html") ||
      !strcmp(extension, "htm"))
    return "text/html";
  else
    return "text/palin";
}

void send_error(FILE *fp) {
  char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
  char server[] = "server:Linux Web Server \r\n";
  char cnt_len[] = "Content-length:2048\r\n";
  char cnt_type[] = "Content-type:text/html\r\n\r\n";
  char content[] = "<html><head><title>NETWORK</title></head>"
    "<body><font size=+5><br>error : check request filename and method!"
    "</font></body></html>";

  fputs(protocol, fp);
  fputs(server, fp);
  fputs(cnt_len, fp);
  fputs(cnt_type, fp);
  fputs(content, fp);
  fflush(fp);
}

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
