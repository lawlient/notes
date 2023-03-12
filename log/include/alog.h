#ifndef ASYNCLOG_HEADER__
#define ASYNCLOG_HEADER__

#include "error.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <libgen.h>

#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#define SHMM_KEY       0x2838bf25
#define SHMM_SIZE      (1 << 18)  /* 8M */
#define SHMM_MAGIC     827
#define LOG_MAGIC      27
#define PATH_LEN       128
#define MODUSIZE       (1 << 10)
#define LOG_LINE_LEN   (1 << 10)

#define LOG_PREFIX "/data/log/"

#define WRITING     'w'
#define READABLE    'r'

#define sbarrier() __asm__ __volatile__("sfence" :: : "memory")
#define lbarrier() __asm__ __volatile__("lfence" :: : "memory")
#define CAS(m, o, n) __sync_bool_compare_and_swap(&(m), (o), (n))

typedef struct Header_ {
    int magic;      /* 用于生产者attach时简单校验 */
    int head;       /* 循环队列头指针，相对值     */
    int tail;       /* 循环队列尾指针，相对值     */
    int len;        /* body length                */
    int reset;      /* 队列损坏，需要重置         */
} Header;


typedef struct Item_ {
    int max;                /* 单文件日志byte上限     */
    int level;              /* 日志等级               */
    char path[PATH_LEN];    /* 日志路径【不包含前缀】 */
} Item;

typedef struct State_ {
    int full;               /* 日志文件满    */
    int registered;         /* 模块是否注册  */
} State;

typedef struct Module_ {
    Item item[MODUSIZE];
    State stat[MODUSIZE];
} Module;


typedef enum LogType_ {
    IDLE      =  0,
    REGISTER  =  1,
    LOG       =  2,
    LOGMAX
} LogType;

typedef struct LogItem_ {
    uint8_t flag;              // 'w': writing, 'r': readable
    uint8_t version;
    int8_t magic;
    int8_t type;
    uint16_t id;
    uint16_t len;
    char    data[];
} LogItem;

typedef struct AsyncLog_ {
    Header header;
    Module module;
    char   body[];
} AsyncLog;


AsyncLog *AsyncLog_new();
AsyncLog *AsyncLog_attach();
int AsyncLog_delete();



/* 修改head指针，获取指定长度的队列空间 */
log_err_t AsyncLog_enqueue(AsyncLog *this, int len, LogItem **log);
/* 获取一条log， 不修改tail指针 */
LogItem *AsyncLog_peekqueue(AsyncLog *this);
/* 修改tail指针 */
void AsyncLog_dequeue(AsyncLog *this);

int AsyncLog_filename(AsyncLog *this, int id, char buf[], int len);

static inline int AsyncLog_empty(AsyncLog *this) { return (this->header.head == this->header.tail) ? 1 : 0; }
static inline int Module_id_valid(int id) { return id >= 0 && id < MODUSIZE; }
static inline int Log_type_valid(int type) { return type >= 0 && type < LOGMAX; }
static inline int Log_magic_valid(LogItem* log) { return log->magic == LOG_MAGIC; }

#endif
