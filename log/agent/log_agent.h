#ifndef ASYNCLOG_AGENT_HEADER__
#define ASYNCLOG_AGENT_HEADER__


#include "alog.h"


#define LOCKFILE "/data/services/log_agent/lock.pid"


extern int AsyncLog_logger(AsyncLog *this, LogItem *log);

extern int module_hash(const char *path);

/* 子进程消费共享内存循环队列中的日志记录 */
extern void log_consumer();





#endif
