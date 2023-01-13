#include "log_agent.h"




void log_consumer() {
    AsyncLog *alog = AsyncLog_new();
    if (NULL == alog) return;
    int rc = 0;

    do {
        LogItem *log = AsyncLog_peekqueue(alog);
        if (NULL == log /* empty */) {
            usleep(10 * 1000);
            continue;
        }

        rc = AsyncLog_logger(alog, log);

        AsyncLog_dequeue(alog);

        rc++;

    } while (1);

}
