#include "console.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <fcntl.h>


#define CMDLEN 1024
#define ARGSIZE 12

typedef struct Args_ {
    int argc;
    char *argv[ARGSIZE];
} Args;


static int cmd_parse(char cmd[], Args *c);
static const Command *Console_cmd_find(const Console *this, const Args *c);

static void *consoler(void *console);
static void worker(Console *, int);

static int setnonblocking(int fd);


int Console_init(Console *this, int port, const Command* commands) {
    this->epollfd = -1;
    this->port = port;
    this->cmds = commands;

    pthread_t cid;
    if (-1 == pthread_create(&cid, NULL, consoler, this)) {
        perror(strerror(errno));
        exit(errno);
    }
    pthread_detach(cid);

    return 0;
}



int cmd_parse(char cmd[], Args *arg) {
    memset(arg, 0, sizeof(Args));
    char *p, *lp;
    arg->argc = 0;
    p = cmd;
    while (*p) {
        while (*p == ' ' || *p == '\n') p++;
        lp = p;
        while (*p && *p != ' ' && *p != '\n') p++;
        arg->argv[arg->argc++] = lp;
        if (0 == *p) break;
        if (arg->argc >= ARGSIZE) return 1;
        *p++ = 0;
    }

    return (arg->argc) ? 0 : 2;
}

const Command *Console_cmd_find(const Console *this, const Args *arg) {
    if (arg->argc == 0 || !arg->argv) return NULL;
    for (const Command *cmd = this->cmds; cmd->name; cmd++) {
        if (0 == strncmp(cmd->name, arg->argv[0], strlen(cmd->name))) {
            return cmd;
        }
    }
    return NULL;
}

void *consoler(void *console) {
    Console* this = console;
    int consol = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == consol) {
        perror(strerror(errno));
        exit(errno);
    }

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(this->port);

    if (bind(consol, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)  {
        perror(strerror(errno));
        exit(errno);
    }

    if (listen(consol, MAXCONSOLE) == -1) {
        perror(strerror(errno));
        exit(errno);
    }

    int epollfd, nfds;
    struct epoll_event ev;

    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        perror(strerror(errno));
        exit(errno);
    }
    this->epollfd = epollfd;

    ev.events = EPOLLIN;
    ev.data.fd = consol;
    setnonblocking(consol);
    if (-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, consol, &ev)) {
        perror(strerror(errno));
        exit(errno);
    }

    do {
        nfds = epoll_wait(epollfd, this->events, MAXCONSOLE, -1);
        if (-1 == nfds) {
            break;
        }

        for (int n = 0; n < nfds; n++) {
            if (this->events[n].data.fd == consol) {
                struct sockaddr_in caddr;
                int kAddrSize = sizeof(caddr);
                int cli = accept(consol, (struct sockaddr *)&caddr, (socklen_t *)&kAddrSize);
                if (-1 == cli) continue;
                setnonblocking(cli);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = cli;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, cli, &ev) == -1)  {
                    continue;
                }
            } else {
                worker(this, n);
            }
        }
    } while (1);

    epoll_ctl(epollfd, EPOLL_CTL_DEL, consol, NULL);
    close(epollfd);
    return NULL;
}

void worker(Console *this, int n) {
    int clientfd = this->events[n].data.fd;

    char cmd[CMDLEN];
    int size = 0;
    Args arg;
    while (1) {
        size = read(clientfd, cmd, CMDLEN);
        if ((size == -1 && errno == EAGAIN)) {
            return;
        }
        if (size == 0) {
            break;
        }
        if (cmd_parse(cmd, &arg)) {
            dprintf(clientfd, "CMD PARSE ERROR: '%s'\n", cmd);
            memset(cmd, 0, CMDLEN);
            continue;
        }

        const Command *c = Console_cmd_find(this, &arg);
        if (c == NULL) {
            dprintf(clientfd, "cannot found command $=> %s\n", arg.argv[0]);
            memset(cmd, 0, CMDLEN);
            continue;
        }

        c->handler(clientfd, arg.argc, arg.argv);
        memset(cmd, 0, CMDLEN);
    }

    epoll_ctl(this->epollfd, EPOLL_CTL_DEL, clientfd, NULL);
    close(clientfd);
    return;
}


int setnonblocking(int fd) {
    /* fcntl函数可以改变文件描述符的性质 */
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}