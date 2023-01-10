#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

void add_proc(int fd) {
    char buf[1024];
    while (read(fd, buf, 1024)) {
        fprintf(stdout, "%d %s\n", getpid(), buf);
        buf[0] += 1;
        write(fd, buf, 1024);
    }
    close(fd);
    printf("%d exit\n", getpid());
    exit(0);
}


int main() {
    int fd[2] = {0};
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd)) {
        printf("create socketpair fail\n");
        exit(1);
    }

    int child = fork();
    switch(child) {
        case -1: printf("fork fail\n"); exit(1);
        case 0: close(fd[0]); add_proc(fd[1]);
        default: break;
    }

    close(fd[1]);

    sleep(3);

    char buf[1024] = {0};
    buf[0] = 'a';
    buf[1] = '\0';
    write(fd[0],  buf, 1024);
    read(fd[0], buf, 1024);
    printf("%d, %s\n", getpid(), buf);
    sleep(1);
    printf("%d exit after 2s\n", getpid());
    sleep(2);
    close(fd[0]);
    exit(0);
}
