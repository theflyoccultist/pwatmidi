#include <cstdlib>
#include <ncurses.h>
#include <atomic>

struct AudioState {
    std::atomic<float> freq;
    std::atomic<bool> is_muted;
    std::atomic<float> vol;
};

class UI {
  public:
    UI();
    UI(UI &&) = delete;
    UI(const UI &) = delete;
    UI &operator=(UI &&) = delete;
    UI &operator=(const UI &) = delete;

    void run(AudioState &state);
    static void finish(int sig);

    ~UI();

  private:
    int num{};
    AudioState state;
};
