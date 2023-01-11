#include "log_agent.h"

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>



static void try_lock_file();
static void run_consumer();
static void guard_consumer();



int main(int argc, char *argv[]) {
    if (daemon(0, 0) < 0) {
        perror("daemonize failure");
        exit(errno);
    }

    try_lock_file(LOCKFILE);

    run_consumer();

    guard_consumer();

    return 0;
}





void try_lock_file(const char *lockfile) {
    return;
    assert(lockfile);
    int fd = open(lockfile, O_WRONLY | O_CREAT, 0666);
    if (fd < 0) {
        perror("open lockfile fail");
        exit(errno);
    }

    fchmod(fd, 0666);
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &lock) < 0) {
        /* todo */
    }
}

void run_consumer() {
    int cid = fork();
    if (cid < 0) {

    } else if (cid == 0) {
        log_consumer();
        sleep(2);
        exit(0);
    } else {
        // syslog start consumer success
    }
}

void guard_consumer() {
    int cid, st;
    for (;;) {
        while ((cid = waitpid(-1, &st, WNOHANG)) > 0) {
            run_consumer();
        }
        usleep(1000);
    }
}

