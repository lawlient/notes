#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#ifdef HAVE_GETOPT_H
    #include <getopt.h>
#endif

enum Color_t { NON, RED, GREEN, CYAN };

struct option_t {
    enum Color_t color;
    int count_down;
} goption;


void usage() {
    printf(" Usage: countdown -[hV] [-c color] [-t sec]\n");
    printf(" -h: Print usage and exit\n");
    printf(" -V: Print version information and exit\n");
    printf(" -c [color]: Use this color for text \n");
    printf(" -t [sec]: countdonw this seconds \n");
}

void version(void) {
    printf(" countdown version %s (compiled %s, %s)\n", VERSION, __TIME__, __DATE__);
    printf("  Email: jiuanx2011@163.com\n");
    printf("  Web: https://github.com/lawlient/notes/ncurses/countdown\n");
}

void parse_option(int argc, char* argv[]) {
    opterr = 0;
    int optchr;
    while ((optchr = getopt(argc, argv, "c:t:hV")) != EOF) {
        switch(optchr) {
            case 'h': usage(); exit(0);
            case 'V': version(); exit(0);
            case 'c': {
                if (!strcasecmp(optarg, "red")) {
                    goption.color = RED;
                } else if (!strcasecmp(optarg, "green")) {
                    goption.color = GREEN;
                } else if (!strcasecmp(optarg, "cyan")) {
                    goption.color = CYAN;
                } else {
                    goption.color = NON;
                }
                continue;
            }
            case 't': {
                goption.count_down = atoi(optarg);
                continue;
	        }
            default: usage(); exit(0);
        }
    }
    return;
}

void initcolor() {
    if (!has_colors()) return;
    start_color();
    init_pair(RED, COLOR_RED,  COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN,  COLOR_BLACK);
    init_pair(CYAN, COLOR_CYAN,  COLOR_BLACK);
}

void init() {
    initscr();
    noecho();
    nodelay(stdscr, true);
    curs_set(0); /* hide cursor */
    keypad(stdscr, true);
    initcolor();
}

void quit() {
    endwin();
    exit(0);
}

void draw() {
    clear();

    int sec = goption.count_down;
    int day = sec / 86400; sec = sec % 86400;
    int hour = sec / 3600; sec = sec % 3600;
    int minute = sec / 60; sec = sec % 60;
    int second = sec;

    char daystr[50] = {0};
    char buf[50] = {0};
    
    time_t now = time(0);
    struct tm* tm = localtime(&now);
    tm->tm_hour = hour;
    tm->tm_min  = minute;
    tm->tm_sec  = second;
    strftime(buf, 50, "%H:%M:%S", tm);

    switch (goption.color) {
        case RED: attron(COLOR_PAIR(RED)); break;
        case GREEN: attron(COLOR_PAIR(GREEN)); break;
        case CYAN: attron(COLOR_PAIR(CYAN)); break;
        default: break;
    }

    if (day) {
        snprintf(daystr, 50, "%d days", day);
        move(LINES/2 - 1, (COLS - strlen(daystr))/2);
        addstr(daystr);

        move(LINES/2 + 1, (COLS - strlen(buf))/2);
        addstr(buf);
    } else {
        move(LINES/2, (COLS - strlen(buf))/2);
        addstr(buf);
    }

    refresh();
    sleep(1);
}

int main(int argc, char* argv[]) {
    parse_option(argc, argv);

    init();

    int c = 0;
    bool pause = false;
    do {
        if ((c = getch()) != ERR) {
            switch (c) {
                case 'r': goption.color = RED; break;
                case 'g': goption.color = GREEN; break;
                case 'y': goption.color = CYAN; break;
                case 'p': pause = !pause; break;
                case 'q': quit();
                default: break;
            }
        }

        if (pause) { sleep(1); continue; }

        draw();
        --goption.count_down;

    } while (goption.count_down);

    quit();
    return 0;
}
