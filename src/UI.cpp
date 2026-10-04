#include "UI.hpp"
#include <ncurses.h>
#include <string>

UI::UI() {
    initscr();            /* initialize the curses library */
    keypad(stdscr, TRUE); /* enable keyboard mapping */
    nonl();               /* tell curses not to do NL->CR/NL on output */
    cbreak();             /* take input chars one at a time, no wait for \n */
    echo();               /* echo input - in color */

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
    std::string mesg = "Enter a string: "; /* message to be appeared on the screen */
    std::string str{};
    int row{}, col{};           /* to store the number of rows and *
                                 * the number of colums of the screen */
    initscr();                  /* start the curses mode */
    getmaxyx(stdscr, row, col); /* get the number of rows and columns */
    mvprintw(row / 2, (int)(col - mesg.length()) / 2, "%s", mesg.c_str());
    /* print the message at the center of the screen */
    getstr(str.data());
    mvprintw(LINES - 2, 0, "You Entered: %s", str.c_str());
    getch();

    /* process the command keystroke */
}

UI::~UI() { finish(0); /* we're done */ }

void UI::finish(int sig) {
    endwin();

    /* do your non-curses wrapup here */
    exit(0);
}
