#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define TOTAL  100
#define STR(x) "[%-"#x"s][%3d%%]\r"
#define FORMAT(x) STR(x)


// 显示进度条
void display_progress(double rate) {
    char proc[TOTAL] = { '\0'};

    int abs = rate * TOTAL;
    int r = rate * 100;
    memset(proc, '#', abs);
    printf(FORMAT(TOTAL), proc, r);
    fflush(stdout);
}

int main(int arc, char *argv[]) {
    time_t begin = time(0);
    double rate = 0;
    do {
        time_t now = time(0);
        rate += now - begin % 3 ? 0.005 : 0.01;
        display_progress(rate);
        sleep(1.1);
    } while (rate <= 1);

    printf("\n");
    return 0;
}
