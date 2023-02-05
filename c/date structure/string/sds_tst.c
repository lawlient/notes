#include "sds.h"

#include <stdio.h>
#include <time.h>


int main() {
    Sds *s = Sds_newraw("hello world");

    printf("Sds is : %s\n", Sds_data(s));
    printf("Sds size is : %d\n", Sds_size(s));
    printf("Sds free space is : %d\n", Sds_free(s));


    s = Sds_append(s, ", I am lawlient. I am 31 years old.");

    printf("Sds is : %s\n", Sds_data(s));
    printf("Sds size is : %d\n", Sds_size(s));
    printf("Sds free space is : %d\n", Sds_free(s));

    char *c  = Sds_at(s, 10);
    *c = '\0';

    printf("Sds is : %s\n", Sds_data(s));
    printf("Sds size is : %d\n", Sds_size(s));
    printf("Sds free space is : %d\n", Sds_free(s));

    Sds_clear(s);

    printf("Sds is : %s\n", Sds_data(s));
    printf("Sds size is : %d\n", Sds_size(s));
    printf("Sds free space is : %d\n", Sds_free(s));


    char buf[50];
    time_t now = time(0);
    struct tm tm;
    localtime_r(&now, &tm);
    strftime(buf, 50, "%F %T", &tm);
    s = Sds_append(s, buf);

    printf("Sds is : %s\n", Sds_data(s));
    printf("Sds size is : %d\n", Sds_size(s));
    printf("Sds free space is : %d\n", Sds_free(s));

    Sds_delete(s);
    return 0;
}
