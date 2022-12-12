#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>


#define BUF_LEN 50

int main(int argc, char* argv[]) {

    // struct tm t;
    // memset(&t, 0, sizeof(struct tm));
    // strptime(argv[1], argv[2], &t);

    time_t now = time(NULL);
    struct tm* t = localtime(&now);

    int rc = 0;
    if (argv[1]) {
        rc = getdate_r(argv[1], t);
        printf("%d\n", rc);
    }

    
    char buf[BUF_LEN];
    strftime(buf, BUF_LEN, "%F %T", t);

    printf("%s\n", buf);

    return 0;
}



