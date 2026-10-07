#include "UI.hpp"
#include <algorithm>
#include <atomic>
#include <cmath>
#include <ncurses.h>
#include <vector>
#include "Scale.hpp"

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

    SCALE scale = SCALE::MAJOR;
    std::vector<size_t> current_scale = scales.at(scale);
    size_t num_of_notes_in_octave = current_scale.size();

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

        case KEY_F(5): {
            // Switch scale
            ++scale;
            current_scale = scales.at(scale);
            num_of_notes_in_octave = current_scale.size();
            break;
        }

        case KEY_F(6): {
            // Transpose
            std::ranges::for_each(current_scale, [](size_t &s) {
                s++;
                s = s % 12;
            });
            break;
        }

        case 'a' ... 'z':
        case 'A' ... 'Z': {
            if (!muted) {
                key = (ch % num_of_notes_in_octave);
                int octave_shift = target_octave - BASE_OCTAVE;
                state_.freq.store(base_freqs[current_scale[key]] * std::pow(2.0f, octave_shift),
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
        mvprintw(7, 2,
                 "Controls: Up/Down (Vol), Left/Right (-/+ Octave), F3 (Mute), F4 "
                 "(Quit), F5(Switch Scale), F6(Transpose +1 Semitone)");
        mvprintw(9, 2, "Current Note: [%s%d]", base_notes[current_scale[key]].c_str(),
                 target_octave);
        mvprintw(10, 2, "Scale: [%s %s]", base_notes[current_scale[0]].c_str(),
                 display_scale(scale));
        mvprintw(12, 2, "Mute Button:     [%s]", muted ? "PRESSED" : "UNPRESSED");
        refresh();
    }
}

UI::~UI() { finish(0); }

void UI::finish(int sig) {
    endwin();

    /* do your non-curses wrapup here */
    exit(0);
}
