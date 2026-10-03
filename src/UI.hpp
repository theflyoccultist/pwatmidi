#include <cstdlib>
#include <ncurses.h>

class UI {
  public:
    UI();
    UI(UI &&) = default;
    UI(const UI &) = default;
    UI &operator=(UI &&) = default;
    UI &operator=(const UI &) = default;

    void run();
    static void finish(int sig);

    ~UI();

  private:
    int num{};
};
