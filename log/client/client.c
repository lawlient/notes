#include "alog.h"

#include "logging.h"

#define KB (1 << 10)
#define MB (1 << 20)


int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s id module_path\n", argv[0]);
        return 0;
    }

    int id = atoi(argv[1]);
    const char* module = argv[2];
    int max = atoi(argv[3]);

    Log *log = Log_new(0, id, max * KB, module);
    if (!log) {
        printf("init log fail\n");
        return 0;
    }

    log_err_t err = E_MODULE_NOT_REG;
    do {

        if (err) 
            err = Log_register(log);
        if (err) {
            perror("log register error");
            exit(err);
        }


        err = LOG(log, 1, "this is log test client, id: %d, path: %s", id, module);

        sleep (1);

    } while (1);

    return 0;
}
