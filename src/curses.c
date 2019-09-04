#include "curses.h"

void init_window() {
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    refresh();
}

void terminate_window() { endwin(); }
