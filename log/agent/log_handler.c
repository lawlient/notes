#include "log_agent.h"

#define BUFSIZE 256

int fds[MODUSIZE] = {0};

static inline int magic_valid(LogItem *log);

static int prepare_logfile(AsyncLog *this, int id);

static int AsyncLog_filename(AsyncLog *this, int id, char buf[], int len);


static int handle_register(AsyncLog *this, LogItem *log);
static int handle_log(AsyncLog *this, LogItem *log);

int AsyncLog_logger(AsyncLog *this, LogItem *log) {
    if (!magic_valid(log)) return 1;
    switch (log->type) {
    case IDLE: return 0;
    case REGISTER: return handle_register(this, log);
    case LOG: return handle_log(this, log);
    default: break;
    }
    return 3;
}

int handle_register(AsyncLog *this, LogItem *log) {
    if (log->len != sizeof(Item)) return 2;
    const Item *ritem = (Item *)log->data;

    Item *item = &this->module.item[log->id];
    memcpy(item, ritem, sizeof(Item));

    State *stat = &this->module.stat[log->id];
    stat->full  = 0;
    stat->registered = 1;
    return 0;
}

int handle_log(AsyncLog *this, LogItem *log) {
    if (log->id < 0 || log->id >= MODUSIZE) return 2;

    if (prepare_logfile(this, log->id)) {
        return 5;
    }

    ModuleCache *mc = &mcaches[log->id];
    write(mc->fd, log->data, log->len);

    return 0;
}

int magic_valid(LogItem *log) { return log->magic == LOG_MAGIC; }


int prepare_logfile(AsyncLog *this, int id) {
    ModuleCache *mc = &mcaches[id];
    char filename[BUFSIZE];
    AsyncLog_filename(this, id, filename, BUFSIZE);

    struct stat st;
    if (stat(filename, &st) && st.st_ino == mc->inode) {
        return 0;
    }

    if (mc->fd) {
        close(mc->fd);
        mc->fd = -1;
    }

    mc->fd = open(filename, O_APPEND | O_CREAT | O_RDWR, 0666);
    if (mc->fd == -1 && errno == ENOENT) {
        int err = mkdir(dirname(filename), 0755); 
        if (err)
            return 1;

        mc->fd = open(filename, O_APPEND | O_CREAT | O_RDWR, 0666);
        if (mc->fd == -1)
            return 1;
    }

    if (fstat(mc->fd, &st)) {
        mc->inode = st.st_ino;
    }

    return 0;
}

int AsyncLog_filename(AsyncLog *this, int id, char buf[], int len) {
    Item *item   = &this->module.item[id];
    int size = 0;
    size += snprintf(buf, len, LOG_PREFIX"%s/", item->path);
    time_t now = time(0);
    struct tm tm;
    localtime_r(&now, &tm);
    size += strftime(buf+size, len-size, "%F-%H.log", &tm);
    return size;
}
