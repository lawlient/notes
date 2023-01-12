#ifndef ASYNCLOG_HEADER__
#define ASYNCLOG_HEADER__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#define SHMM_KEY      0x2838bf25
#define SHMM_SIZE  (1 << 23)  /* 8M */
#define SHMM_MAGIC 827
#define LOG_MAGIC  88

#define PATH_LEN   128
#define ITEM_SIZE   (1 << 10)

typedef struct Header_ {
    int magic;      // 用于生产者attach时简单校验
    int head;       // 循环队列头指针，相对值
    int tail;       // 循环队列尾指针，相对值
    int len;        // body length
    int reset;      // 队列损坏，需要重置
} Header;


typedef struct Item_ {
    char path[PATH_LEN];    // 日志路径【不包含前缀】
    int valid;              // 
    int max;                // 单文件日志byte上限
    int level;              // 日志等级
} Item;

typedef struct State_ {
    int full;
    int registered;
} State;

typedef struct Module_ {
    Item item;
    State stat;
} Module;

typedef struct AsyncLog_ {
    Header header;
    Module module[ITEM_SIZE];
    char   body[];
} AsyncLog;


typedef enum LogType_ {
    REGISTER  =  0,
    LOG       =  1,
} LogType;

typedef struct LogItem_ {
    uint8_t version;
    uint8_t flag;
    uint8_t magic;
    uint8_t type;
    char path[PATH_LEN];
    uint16_t len;
    char    data[];
} LogItem;


AsyncLog *AsyncLog_new();
AsyncLog *AsyncLog_attach();
int AsyncLog_delete();


/* 修改head指针，获取指定长度的队列空间 */
LogItem *AsyncLog_enqueue(AsyncLog *this, int len);
/* 修改tail指针，获取一条log */
LogItem *AsyncLog_dequeue(AsyncLog *this);
int AsyncLog_empty(AsyncLog *this);





#endif
