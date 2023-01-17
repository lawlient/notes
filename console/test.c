#include "console.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



#define DEFAULT_PORT 10196

static int version(int cfd, int argc, char *argv[]) {
    const char* VERSION = "1.0.0";
    dprintf(cfd, "%s\n", VERSION);
    return 0;
}

static int help(int cfd, int argc, char *argv[]) {
    dprintf(cfd, ">  version    show version info\n");
    dprintf(cfd, ">  help       show those help info\n");
    return 0;
}

int main(int argc, char *argv[]) {
    Console console;

    int port = DEFAULT_PORT;
    if (argc > 1)  port = atoi(argv[1]);

    static const Command cmds[] = {
        {"version", version},
        {"help", help},
        {0, 0},
    };

    if (-1 == Console_init(&console, port, cmds)) {
        printf("console init fail\n");
        return -1;
    }

    printf("start server, listenning on port: %d\n", port);
    do {
        sleep(3); /* idle for telnel testing */
    } while(1);
}

