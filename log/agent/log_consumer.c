#include "log_agent.h"




void log_consumer() {
    AsyncLog *alog = AsyncLog_new();
    if (NULL == alog) return;
    log_err_t err = E_OK;

    do {
        LogItem *log = AsyncLog_peekqueue(alog);
        if (NULL == log /* empty */) {
            usleep(1 * 1000);
            continue;
        }

        err = AsyncLog_logger(alog, log);

        AsyncLog_dequeue(alog);

        err++;

    } while (1);

}
