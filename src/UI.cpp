#include "UI.hpp"
#include <iostream>

UI::UI() {
    (void)initscr();      /* initialize the curses library */
    keypad(stdscr, TRUE); /* enable keyboard mapping */
    (void)nonl();         /* tell curses not to do NL->CR/NL on output */
    (void)cbreak();       /* take input chars one at a time, no wait for \n */
    (void)echo();         /* echo input - in color */

    if (has_colors()) {
        start_color();

        /*
         * Simple color assignment, often all we need.  Color pair 0 cannot
         * be redefined.  This example uses the same value for the color
         * pair as for the foreground color, though of course that is not
         * necessary:
         */
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_BLUE, COLOR_BLACK);
        init_pair(5, COLOR_CYAN, COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_WHITE, COLOR_BLACK);
    }
}

void UI::run() {
    for (;;) {
        int c = getch(); /* refresh, accept single keystroke of input */
        attrset(COLOR_PAIR(num % 8));
        num++;

        if (c == KEY_UP)
            std::cout << "Welcome to pwatmidi" << "\n";

        /* process the command keystroke */
    }
}

UI::~UI() { finish(0); /* we're done */ }

void UI::finish(int sig) {
    endwin();

    /* do your non-curses wrapup here */
    exit(0);
}
