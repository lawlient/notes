#include "logging.h"

#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>

#include <assert.h>


static void Log_attach(Log *this);
static int Log_severity_valid(Severity s);
static int Log_prefix(Severity severity, const char* file, int line, const char* func, char buf[], size_t len);



const char *kSeverityName[] = {
    "debug",
    "info",
    "warning",
    "error",
};





Log *Log_new(Severity severity, int max, const char* path) {
    assert(path);
    Log *log = malloc(sizeof(Log));
    if (NULL == log) return NULL;
    log->severity = severity;
    log->max      = max;
    strncpy((char *)log->path, path, PATH_LEN);
    log->shmm = NULL;
    return log;
}

int Log_register(Log *this) {
    if (this->shmm) return 0;
    Log_attach(this);
    AsyncLog *alog = this->shmm;
    size_t rsize = sizeof(LogItem) + sizeof(Item);
    LogItem *log = AsyncLog_enqueue(alog, rsize);
    if (NULL == log) return 1;
    memset(log, 0, rsize);
    log->version = 0;
    log->magic = LOG_MAGIC;
    log->type  = REGISTER;
    log->len   = sizeof(Item);

    Item *item = (Item *)log->data;
    strncpy(item->path, this->path, PATH_LEN);
    item->valid = 1;
    item->max = this->max;
    item->level = this->severity;
    return 0;
}


int Log_log(Log *this, Severity severity, const char* file, int line, const char* func, const char *fmt, ...) {
    if (!Log_severity_valid(severity)) {
        return 1;
    }
    if (this->shmm == NULL) {
        Log_attach(this);
        if (this->shmm == NULL) {
            /* todo syslog */
            return 2;
        }
    }


    const int len = 1024;
    char buf[len];
    uint16_t size = 0;
    size += Log_prefix(severity, file, line, func, buf, len-1);

    va_list ap;
    va_start(ap, fmt);
    size += vsnprintf(buf+size, len-1-size, fmt, ap);
    va_end(ap);


    AsyncLog *alog = this->shmm;
    LogItem *log = AsyncLog_enqueue(alog, sizeof(LogItem) + size);
    log->len = size;
    log->magic = LOG_MAGIC;
    log->type = LOG;
    log->version = 0;
    log->flag = 0;
    memcpy(log->path, this->path, PATH_LEN);
    memcpy(log->data, buf, size);
    return 0;
}







void Log_attach(Log *this) { if (!this->shmm) this->shmm = AsyncLog_attach(); }

int Log_prefix(Severity severity, const char* file, int line, const char* func,
               char buf[], size_t len) {
    char *p =  buf;

    /* time */
    time_t now = time(0);
    struct tm tm;
    localtime_r(&now, &tm);
    strftime(p, len, "%F %T ", &tm); len -= 20;
    p += 20;

    /* level */
    if (Log_severity_valid(severity)) {
        const char* lname = kSeverityName[severity];
        size_t llen = strlen(lname);
        strncpy(p, lname, llen);
        len -= llen;
        p += llen;
    }

    /* pid */
    snprintf(p, len, " %5d", getpid()); p += 5; len -= 5;
    // snprintf(p, len, " %5d", getpid()); p += 5;

    /* filename:line [funcname] */
    snprintf(p, len, " %s:%5d [%s] ", file, line, func);
    size_t poslen = strlen(file) + 5 + strlen(func) + 6;
    len -= poslen;
    p += poslen;
    return p - buf; /* used size */
}

inline int Log_severity_valid(Severity s) { return s >= 0 && s < Smax; }
