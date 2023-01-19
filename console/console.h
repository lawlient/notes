#ifndef CONSOLE_HEADER__
#define CONSOLE_HEADER__


#include <sys/epoll.h>

#define MAXCONSOLE 10


typedef int (*Handle)(int cfd, int argc, char *argv[]);


typedef struct Command_ {
    char *name;
    Handle handler;
} Command;

typedef struct Console_ {
    int epollfd;
    struct epoll_event events[MAXCONSOLE];
    int port;
    const Command *cmds;
} Console;


int Console_init(Console *this, int port, const Command* commands);









#endif
