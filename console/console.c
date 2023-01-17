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


#define CMDLEN 1024
#define ARGSIZE 12

typedef struct Args_ {
    int argc;
    char *argv[ARGSIZE];
} Args;


static int cmd_parse(char cmd[], Args *c);
static const Command *Console_cmd_find(const Console *this, const Args *c);

static void *consoler(void *);
static void worker(Console *console, int clientfd);

int Console_init(Console *this, int port, const Command* commands) {
    this->fd = -1;
    this->port = port;
    this->cmds = commands;

    pthread_t cid;
    int err = pthread_create(&cid, NULL, consoler, this);
    if (err) {
        printf("error: %d msg:%s", errno, strerror(errno));
        return err;
    }

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

void *consoler(void *consoler) {
    Console *this = consoler;

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

    if (listen(consol, 10) == -1) {
        perror(strerror(errno));
        exit(errno);
    }

    do {
        struct sockaddr_in caddr;
        int kAddrSize = sizeof(caddr);
        int cli = accept(consol, (struct sockaddr *)&caddr, (socklen_t *)&kAddrSize);
        if (-1 == cli) continue;

        /* first version, it is easy that only one client is handled at once 
           As soon as possible, it will support mutli cliets works at once by thread pool
           and handle for disconnect */
        worker(this, cli);
    } while (1);
    return NULL;
}

void worker(Console *this, int clientfd) {
    char cmd[CMDLEN];
    memset(cmd, 0 , CMDLEN);
    int size = 0;
    Args arg;
    while ((size = read(clientfd, cmd, CMDLEN)) != 0) {
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
}

