#include "alog.h"


#include <sys/ipc.h>
#include <sys/shm.h>

#include <errno.h>
#include <stdio.h>



AsyncLog *AsyncLog_new() {
    int shmmid = shmget(SHMM_KEY, SHMM_SIZE, IPC_EXCL | IPC_CREAT | 0666);
    if (shmmid == -1 && errno == EEXIST) {
        shmmid = shmget(SHMM_KEY, 0, IPC_CREAT | 0666);
    }
    if (shmmid == -1) return NULL;

    void *shmm = shmat(shmmid, NULL, 0);
    if (shmm == (void *)-1) {
        perror(" shmat fail");
        exit(errno);
    }

    AsyncLog *alog = shmm;
    return alog;
}

AsyncLog *AsyncLog_attach() {
    int shmmid = shmget(SHMM_KEY, 0, IPC_CREAT | 0666);
    if (-1 == shmmid) return NULL;

    void *shmm = shmat(shmmid, NULL, 0);
    if (shmm == (void *)-1) {
        perror(" shmat fail");
        exit(errno);
    }

    AsyncLog *alog = shmm;
    return alog;
}



int AsyncLog_delete() {
    return 0;
}

/* todo: 
 * 1. cas
 * 2. circle
 */
LogItem *AsyncLog_enqueue(AsyncLog *this, int len) {
    LogItem *log = (LogItem *)this->body + this->header.head;
    this->header.head += sizeof(LogItem) + len;
    return log;
}

LogItem *AsyncLog_dequeue(AsyncLog *this) {
    if (AsyncLog_empty(this)) {
        return NULL;
    }
    LogItem *log = (LogItem *)this->body + this->header.tail;
    this->header.tail += sizeof(LogItem) + log->len;
    return log;
}

int AsyncLog_empty(AsyncLog *this) {
    return (this->header.head == this->header.tail) ? 1 : 0;
}