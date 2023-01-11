#include "log_agent.h"



static inline int magic_valid(LogItem *log);
static int handle_register(AsyncLog *this, LogItem *log);
static int handle_log(AsyncLog *this, LogItem *log);


int AsyncLog_logger(AsyncLog *this, LogItem *log) {
    switch (log->type) {
    case REGISTER: return handle_register(this, log);
    case LOG: return handle_log(this, log);
    default: break;
    }
    return 3;
}

int handle_register(AsyncLog *this, LogItem *log) {
    if (!magic_valid(log)) return 1;
    if (log->len != sizeof(Item)) return 2;
    const Item *item = (Item *)log->data;

    log->path[PATH_LEN - 1] = '\0';
    int index = module_hash(log->path);
    Module *module = &this->module[index];
    module->item.level = item->level;
    module->item.max = item->max;
    strncpy(module->item.path, item->path, PATH_LEN);
    module->item.valid = item->valid;

    module->stat.full = 0;
    module->stat.registered = 1;
    return 0;
}

int handle_log(AsyncLog *this, LogItem *log) {
    if (!magic_valid(log)) return 1;
    return 0;
}

int magic_valid(LogItem *log) { return log->magic == LOG_MAGIC; }