#ifndef ASYNCLOG_ERROR_HEADER__
#define ASYNCLOG_ERROR_HEADER__


#include <errno.h>

typedef int log_err_t;

#define E_OK               0
#define E_INVALID_LEVEL    1
#define E_NOT_ATTACH       2
#define E_SHMM_RESETING    3
#define E_MODULE_NOT_REG   4
#define E_MODULE_FULL      5
#define E_CAS_FAIL         6
#define E_QUEUE_FULL       7









#endif
