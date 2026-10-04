#include "AudioEngine.hpp"
#include "UI.hpp"
#include <thread>

void run() {
    UI ui;
    ui.run();
}

int main(int argc, char *argv[]) {
    pw_init(&argc, &argv);

    AudioEngine audio;
    std::jthread uiThread(run);

    audio.run();

    return 0;
}
