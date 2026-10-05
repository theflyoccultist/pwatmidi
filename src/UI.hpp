#include <cstdlib>
#include <ncurses.h>
#include "AudioEngine.hpp"

class UI {
  public:
    explicit UI(AudioState &state);
    UI(UI &&) = delete;
    UI(const UI &) = delete;
    UI &operator=(UI &&) = delete;
    UI &operator=(const UI &) = delete;

    void run();
    static void finish(int sig);

    ~UI();

  private:
    int num{};
    AudioState &state_;
};
