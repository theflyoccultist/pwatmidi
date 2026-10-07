#include "UI.hpp"
#include <algorithm>
#include <atomic>
#include <cmath>
#include <ncurses.h>
#include <string>
#include <vector>

UI::UI(AudioState &state) : state_(state) {
    initscr();            /* initialize the curses library */
    cbreak();             /* take input chars one at a time, no wait for \n */
    noecho();             /* no echo input */
    keypad(stdscr, TRUE); /* enable keyboard mapping */
    curs_set(0);

    timeout(33);
}

void UI::run() {
    bool running = true;

    constexpr int BASE_OCTAVE = 4;
    int target_octave = 5;

    const std::vector<std::string> base_notes = {
        "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B",
    };

    const std::vector<float> base_freqs = {
        261.63f, 277.18f, 293.66f, 311.13f, 329.63f, 349.23f,
        369.99f, 392.0f,  415.3f,  440.0f,  466.16f, 493.88f,
    };

    size_t key = 0;
    bool muted = false;
    while (running) {
        int ch = getch();

        switch (ch) {
        case KEY_F(3): {
            // Toggle mute
            muted = !muted;
            break;
        }

        case KEY_F(4):
            running = false;
            break;

        case 'a' ... 'z':
        case 'A' ... 'Z': {
            if (!muted) {
                key = (ch % 12);
                int octave_shift = target_octave - BASE_OCTAVE;
                state_.freq.store(base_freqs[key] * std::pow(2.0f, octave_shift),
                                  std::memory_order_relaxed);
                state_.samples_remaining.store(DURATION_SAMPLES, std::memory_order_relaxed);
                state_.is_muted.store(false, std::memory_order_relaxed);
            }
            break;
        }

        case KEY_UP: {
            // Increase volume (clamp to 1.0)
            float v = state_.vol.load(std::memory_order_relaxed);
            state_.vol.store(std::min(1.0f, v + 0.05f), std::memory_order_relaxed);
            break;
        }

        case KEY_DOWN: {
            // Decrease volume (clamp to 0.0)
            float v = state_.vol.load(std::memory_order_relaxed);
            state_.vol.store(std::max(0.0f, v - 0.05f), std::memory_order_relaxed);
            break;
        }

        case KEY_RIGHT: {
            // +1 octave
            target_octave++;
            break;
        }

        case KEY_LEFT: {
            // -1 octave
            target_octave--;
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
        mvprintw(3, 2, "Volume:    [%.2f]", state_.vol.load());
        mvprintw(4, 2, "Frequency: [%.1f Hz]", state_.freq.load());
        mvprintw(5, 2, "Muted:     [%s]", state_.is_muted.load() ? "YES" : "NO");
        mvprintw(7, 2, "Controls: Up/Down (Vol), Left/Right (-/+ Octave), F3 (Mute), F4 (Quit)");
        mvprintw(9, 2, "Current Note: [%s%d]", base_notes[key].c_str(), target_octave);
        mvprintw(11, 2, "Mute Button:     [%s]", muted ? "PRESSED" : "UNPRESSED");
        refresh();
    }
}

UI::~UI() { finish(0); }

void UI::finish(int sig) {
    endwin();

    /* do your non-curses wrapup here */
    exit(0);
}
