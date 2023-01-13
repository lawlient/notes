#include "alog.h"


#include <sys/ipc.h>
#include <sys/shm.h>

#include <errno.h>
#include <stdio.h>

static AsyncLog *AsyncLog_reset(AsyncLog *this);

static const LogItem kLogItem = {
    .flag = 'r',
    .version = 0,
    .magic = LOG_MAGIC,
    .type = IDLE,
    .id = MODUSIZE,
    .len = 0,
};


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
    this->header.len   = (char*)this + SHMM_SIZE - this->body - sizeof(LogItem);
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
    int try = 0;
    int hsize = sizeof(LogItem);
    int ctail = 0, ntail = 0;
    do {
        if (try++ <= 10) break;
        ctail = this->header.tail;
        ntail = ctail + hsize + len;
        if (ntail > this->header.len) {
            if (this->header.head < hsize + len) {
                return NULL;
            }
            ntail = 0;
        }
    } while (!CAS(this->header.tail, ctail, ntail));

    if (ntail == 0) {
        memcpy(this->body + ctail, &kLogItem, sizeof(kLogItem));
    }

    LogItem *log = (LogItem *)(this->body + this->header.tail);
    return log;
}

LogItem *AsyncLog_peekqueue(AsyncLog *this) {
    if (AsyncLog_empty(this)) {
        return NULL;
    }
    LogItem *log = (LogItem *)(this->body + this->header.head);
    return log;
}

void AsyncLog_dequeue(AsyncLog *this) {
    if (AsyncLog_empty(this)) return;
    LogItem *log = (LogItem *)(this->body + this->header.head);
    this->header.head += sizeof(LogItem) + log->len;
    if (this->header.head >= this->header.len) {
        this->header.head = 0;
    }
}

int AsyncLog_empty(AsyncLog *this) {
    return (this->header.head == this->header.tail) ? 1 : 0;
}
