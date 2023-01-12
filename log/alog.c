#include "alog.h"


#include <sys/ipc.h>
#include <sys/shm.h>

#include <errno.h>
#include <stdio.h>

static AsyncLog *AsyncLog_reset(AsyncLog *this);


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

    return AsyncLog_reset(shmm);
}

AsyncLog *AsyncLog_reset(AsyncLog *this) {
    memset(this, 0, SHMM_SIZE);
    this->header.magic = SHMM_MAGIC;
    this->header.head  = 0;
    this->header.tail  = 0;
    this->header.len   = (char*)this + SHMM_SIZE - this->body;
    this->header.reset = 0;
    return this;
}

AsyncLog *AsyncLog_attach() {
    int shmmid = shmget(SHMM_KEY, 0, IPC_CREAT | 0666);
    if (-1 == shmmid) return NULL;

    void *shmm = shmat(shmmid, NULL, 0);
    if (shmm == (void *)-1) return NULL;

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
    LogItem *log = (LogItem *)(this->body + this->header.head);
    this->header.head += len;
    return log;
}

LogItem *AsyncLog_dequeue(AsyncLog *this) {
    if (AsyncLog_empty(this)) {
        return NULL;
    }
    LogItem *log = (LogItem *)(this->body + this->header.tail);
    this->header.tail += sizeof(LogItem) + log->len;
    return log;
}

int AsyncLog_empty(AsyncLog *this) {
    return (this->header.head == this->header.tail) ? 1 : 0;
}
