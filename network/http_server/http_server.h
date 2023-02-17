#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <arpa/inet.h>



#define SET_NONBLOCK(fd) fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK)
#define EPOLL_EVENT_MAX 5
#define BUFLEN 10240

typedef int (*handle_pt)(int, int);



typedef struct http_request_s {
    char *buf;
    int size;

    int client;

    char *method;
    char *url;
    char *body;


    int done:1;

} http_request_t;


http_request_t *http_request_create();
void http_reqeust_destroy(http_request_t* request);