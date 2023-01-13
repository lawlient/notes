#ifndef ASYNCLOG_AGENT_HEADER__
#define ASYNCLOG_AGENT_HEADER__


#include "alog.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#define LOCKFILE "/data/services/log_agent/lock.pid"


extern int AsyncLog_logger(AsyncLog *this, LogItem *log);

extern int module_hash(const char *path);

/* 子进程消费共享内存循环队列中的日志记录 */
extern void log_consumer();



typedef struct ModuleCache_ {
    int fd;
    ino_t inode;
} ModuleCache;

ModuleCache mcaches[MODUSIZE];





#endif
