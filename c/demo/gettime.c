#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>


#define BUF_LEN 50

int main(int argc, char* argv[]) {

    struct tm *t = getdate(argv[1]);

    if (t == NULL){
        printf("%s cast fail, err:%d\n", argv[1], getdate_err);
        return -1;
    }


    char buf[BUF_LEN];
    strftime(buf, BUF_LEN, "%F %T", t);

    printf("%s\n", buf);

    return 0;
}



