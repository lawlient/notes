#ifndef LOGGING_HEADER__
#define LOGGING_HEADER__

#include "alog.h"




#define LOG(log, s, fmt, ...)  Log_log((log), (s), __FILE__, __LINE__, __func__, (fmt), __VA_ARGS__)



typedef enum Severity_ {
    Debug    =  0,
    Info     =  1,
    Warning  =  2,
    Error    =  3,
    Smax,
} Severity;

extern const char *kSeverityName[];



typedef struct Log_ {
    Severity severity;
    int id;
    int max;
    const char path[PATH_LEN];
    void *shmm;
} Log;

Log *Log_new(Severity severity, int id, int max, const char* path);

/* send register log to queue */
log_err_t Log_register(Log *this);

/* send normal log to queue */
log_err_t Log_log(Log *this, Severity severity, const char* file, int line, const char* func, const char *fmt, ...);







#endif
