#include <ncurses.h>

int main() {
  initscr(); // init ncurses mode, include MACRO like LINES COLS and so on
  noecho();  // not echo input on screen
  attron(A_BLINK); // enable blink
  move(0, COLS/2); // move cursor to 0,COLS/2
  int row = LINES/2;
  int col = COLS/2;
  mvaddstr(row, col, "Press 'q' to exit!"); // show string at (row, col)
  refresh(); // show text on screen now

  char c = ' ';
  while ((c = getch() /* get input from keyboard */) != 'q') {
      switch (c) {
        case '1':clear(); mvaddstr(row, col, "Mode 1"); break;
        case '2':clear(); mvaddstr(row, col, "Mode 2"); break;
        case '3':clear(); mvaddstr(row, col, "Mode 3"); break;
        default: mvaddstr(row, col, "Press 'q' to exit!"); break;
      }
      refresh();
  }
  
  endwin(); // exit ncurses mode
  return 0;
}
