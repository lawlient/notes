#include "alog.h"

#include "logging.h"



int main(int argc, char *argv[]) {
    Log *log = Log_new(0, 1024, "log_test_client");
    if (!log) {
        printf("init log fail\n");
        return 0;
    }

    do {
        LOG(log, 1, "%s", "this is a test log");

        sleep(3);
    } while (1);


}