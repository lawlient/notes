#ifndef ASYNCLOG_ERROR_HEADER__
#define ASYNCLOG_ERROR_HEADER__


#include <errno.h>

typedef int log_err_t;

#define E_OK                          0     /* everything's fine */
#define E_INVALID_LEVEL               1
#define E_NOT_ATTACH                  2
#define E_SHMM_RESETING               3
#define E_MODULE_NOT_REG              4
#define E_MODULE_FULL                 5
#define E_CAS_FAIL                    6
#define E_QUEUE_FULL                  7
#define E_LOG_MAGIC_ERR               8
#define E_LOG_WRITING                 9
#define E_QUEUE_BROKEN               10
#define E_MODULE_ID_ERR              11
#define E_LOG_FILE_FULL              12
#define E_CREAT_LOGDIR_FAIL          13
#define E_OPEN_LOG_FILE_FAIL         14
#define E_INVALID_LOGTYPE            15









#endif
