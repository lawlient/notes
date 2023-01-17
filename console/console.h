#ifndef CONSOLE_HEADER__
#define CONSOLE_HEADER__





typedef int (*Handle)(int cfd, int argc, char *argv[]);


typedef struct Command_ {
    char *name;
    Handle handler;
} Command;

typedef struct Console_ {
    int fd;
    int port;
    const Command *cmds;
} Console;


int Console_init(Console *this, int port, const Command* commands);









#endif
