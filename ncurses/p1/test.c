#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#ifdef HAVE_GETOPT_H
    #include <getopt.h>
#endif

enum Color_t { NON, RED, GREEN };

struct option_t {
    enum Color_t color;
} goption;


void usage() {
    printf(" Usage: mode -[hV] [-c color]\n");
    printf(" -h: Print usage and exit\n");
    printf(" -V: Print version information and exit\n");
    printf(" -c [color]: Use this color for text \n");
}

void version(void) {
    printf(" Mode version %s (compiled %s, %s)\n", VERSION, __TIME__, __DATE__);
    printf("Email: jiuanx2011@163.com\n");
    printf("Web: https://github.com/lawlient/notes/ncurses/p1\n");
}

void parse_option(int argc, char* argv[]) {
    opterr = 0;
    int optchr;
    while ((optchr = getopt(argc, argv, "c:hV")) != EOF) {
        switch(optchr) {
            case 'h': usage(); exit(0);
            case 'V': version(); exit(0);
            case 'c': {
                if (!strcasecmp(optarg, "red")) {
                    goption.color = RED;
                } else if (!strcasecmp(optarg, "green")) {
                    goption.color = GREEN;
                } else {
                    goption.color = NON;
                }
                break;
            }
            default: usage(); exit(0);
        }
    }
    usage(); exit(0);
}

int main(int argc, char* argv[]) {
    parse_option(argc, argv);

    initscr(); // init ncurses mode, include MACRO like LINES COLS and so on
    noecho();  // not echo input on screen
    keypad(stdscr, true);  // enable Fn Key input
    attron(A_BLINK); // enable blink
    move(0, COLS/2); // move cursor to 0,COLS/2
    int row = LINES/2;
    int col = COLS/2;
    mvaddstr(row, col, "Press 'q' to exit!"); // show string at (row, col)
    refresh(); // show text on screen now

    int c = ' ';
    while ((c = getch() /* get input from keyboard */) != 'q') {
        if (c == KEY_F(1)) {
            clear(); mvaddstr(row, col, "Fn 1");
        } else {
            switch (c) {
                case '1':clear(); mvaddstr(row, col, "Mode 1"); break;
                case '2':clear(); mvaddstr(row, col, "Mode 2"); break;
                case '3':clear(); mvaddstr(row, col, "Mode 3"); break;
                case KEY_F(1):clear(); mvaddstr(row, col, "Fn 1"); break;
                default: mvaddstr(row, col, "Press 'q' to exit!"); break;
            }
        }
        refresh();
    }

    endwin(); // exit ncurses mode
    return 0;
}
