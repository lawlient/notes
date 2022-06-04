#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

static const int ELAPSE = 5;

int main(void) {
    time_t now;
    char *a_time;
    char day[15] = {0};
    char hour[15] = {0};
    printf("\033[2J"); // clear screen


    if ( fork () ) {
        exit(0);
    }

    time_t start = time(0);
    int elapse = 0;

    do {
        printf("\33[s"); // save cursor's position
        time(&now);
        a_time = asctime(localtime(&now));
        elapse = (int)difftime(now, start);

        memcpy(day, a_time, 10);
        memcpy(hour, a_time + 10, 10);
        printf("\033[1;45H\033[K\033[36m +-----------------------+");
        printf("\033[2;45H\033[K\033[36m + %s            +", day);
        printf("\033[3;45H\033[K\033[36m + %s            +", hour);
        printf("\033[4;45H\033[K\033[36m +-----------------------+");
        printf("\33[u"); // restore cursor's position
        fflush(stdout);
        sleep(1);
        
    } while (elapse < ELAPSE);

    return 0;
}