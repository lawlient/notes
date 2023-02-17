#include "http_server.h"


static int start(int *port);
static int add_event(int epfd, int evfd, int flag);
static int del_event(int epfd, int evfd);
static int event_loop(int epfd, int server);
static int conn_event(int epfd, int fd);
static int read_event(int epfd, int fd);



int main(int argc, char *argv[]) {
    int port = 0;
    int server;
    int epfd;
    if (argc > 1) port = atoi(argv[1]);

    server = start(&port);

    epfd = epoll_create1(0);
    assert(epfd != -1);

    if (add_event(epfd, server, EPOLLIN) < 0) {
        return -1;
    }

    event_loop(epfd, server);

    return 0;
}


int start(int *port) {
    int s;
    struct sockaddr_in addr;
    struct sockaddr *paddr = (struct sockaddr *)&addr;

    s = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == s) return -1;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (0 == *port) *port = 8989;
    addr.sin_port = htons(*port);
    if (bind(s, paddr, sizeof(addr)) < 0)
        return -1;

    if (listen(s, 5) == -1)
        return -1;
    
    return s;
}



int add_event(int epfd, int evfd, int flag) {
    struct epoll_event ev;
    ev.events  = flag;
    ev.data.fd = evfd;
    return epoll_ctl(epfd, EPOLL_CTL_ADD, evfd, &ev);
}

int del_event(int epfd, int evfd) {
    return epoll_ctl(epfd, EPOLL_CTL_DEL, evfd, 0);
}

int event_loop(int epfd, int server) {
    struct epoll_event events[EPOLL_EVENT_MAX];
    int fd;
    handle_pt handler;
    for (;;) {
        int n = epoll_wait(epfd, events, EPOLL_EVENT_MAX, -1); 

        if (n < 0) {
            perror("epoll wait");
            exit(errno);
        }

        if (0 == n) continue;

        for (int i = 0; i < n; i++) {
            fd = events[i].data.fd;

            handler = (fd == server) ? conn_event : read_event;

            handler(epfd, fd);
        }
    }
    return 0;
}

int conn_event(int epfd, int fd) {
    int c;
    struct sockaddr_in addr;
    socklen_t addrn;

    if ((c = accept(fd, (struct sockaddr*)&addr, &addrn)) == -1) 
        return -1;

    SET_NONBLOCK(c);
    return add_event(epfd, c, EPOLLIN|EPOLLET);
}

int read_event(int epfd, int c) {
    http_request_t *request;
    int fd;

    request = http_request_create();


    for (;;) {
        request->size = read(c, request->buf, BUFLEN);
        if (size < 0 && errno == EAGAIN) {
            continue;
        }
        if (size == 0) {
            del_event(epfd, c);
            close(c);
            return 0;
        }
        fd = open("./http_request", O_CREAT | O_RDWR, 0666);
        if (fd == -1) {
            continue;
        }
        write(fd, buf, size);
        fsync(fd);
        close(fd);
    }
    return 0;
}


http_request_t *http_request_create() {
    http_request_t *request;

    request = malloc(sizeof(http_request_t));
    memset(request, 0, sizeof(http_request_t));
    request->buf = malloc(4096);
    request->client = -1;

    return request;
}

void http_reqeust_destroy(http_request_t* request) {
    if (request->buf) free(request->buf);
    free(request->buf);
}