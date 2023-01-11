#include "log_agent.h"




void log_consumer() {
    AsyncLog *glog = AsyncLog_new();
    if (NULL == glog) return;
    int rc = 0;

    do {
        if (AsyncLog_empty(glog)) {
            // usleep(10 * 1000);
            sleep(1);
            continue;
        }

        sleep(3);

        /// LogItem *log = AsyncLog_dequeue(glog);
        /// if (NULL == log) {
        ///     usleep(10 * 1000);
        ///     continue;
        /// }

        /// rc = AsyncLog_logger(glog, log);

        /// rc++;

    } while (1);

}
