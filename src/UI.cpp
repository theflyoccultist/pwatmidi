#include "UI.hpp"
#include <algorithm>
#include <atomic>
#include <ncurses.h>

UI::UI() {
    initscr();            /* initialize the curses library */
    cbreak();             /* take input chars one at a time, no wait for \n */
    noecho();             /* no echo input */
    keypad(stdscr, TRUE); /* enable keyboard mapping */
    curs_set(0);

    timeout(33);
}

void UI::run(AudioState &state) {
    bool running = true;
    while (running) {
        int ch = getch();
        switch (ch) {
        case 'q':
        case 'Q':
            running = false;
            break;

        case 'm':
        case 'M': {
            // Toggle mute
            bool current = state.is_muted.load(std::memory_order_relaxed);
            state.is_muted.store(!current, std::memory_order_relaxed);
            break;
        }

        case KEY_UP: {
            // Increase volume (clamp to 1.0)
            float v = state.vol.load(std::memory_order_relaxed);
            state.vol.store(std::min(1.0f, v + 0.05f), std::memory_order_relaxed);
            break;
        }

        case KEY_DOWN: {
            // Decrease volume (clamp to 0.0)
            float v = state.vol.load(std::memory_order_relaxed);
            state.vol.store(std::max(0.0f, v - 0.05f), std::memory_order_relaxed);
            break;
        }

        case KEY_RIGHT: {
            // Step freqency up
            float f = state.freq.load(std::memory_order_relaxed);
            state.freq.store(std::min(20000.0f, f + 20.0f), std::memory_order_relaxed);
            break;
        }

        case KEY_LEFT: {
            // Step freqency up
            float f = state.freq.load(std::memory_order_relaxed);
            state.freq.store(std::max(20.0f, f - 10.0f), std::memory_order_relaxed);
            break;
        }

        case ERR:
            break;

        default:
            break;
        }
        // Render your ncurses interface here...
        erase();
        mvprintw(1, 2, "=== PipeWire Controller ===");
        mvprintw(3, 2, "Volume:    [%.2f]", state.vol.load());
        mvprintw(4, 2, "Frequency: [%.1f Hz]", state.freq.load());
        mvprintw(5, 2, "Muted:     [%s]", state.is_muted.load() ? "YES" : "NO");
        mvprintw(7, 2, "Controls: Up/Down (Vol), Left/Right (Freq), M (Mute), Q (Quit)");
        refresh();
    }
}

UI::~UI() { finish(0); }

void UI::finish(int sig) {
    endwin();

    /* do your non-curses wrapup here */
    exit(0);
}
