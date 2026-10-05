#include "AudioEngine.hpp"
#include "UI.hpp"
#include <thread>

void run() {
    UI ui;
    AudioState state = {.freq = 440.0f, .is_muted = false, .vol = 0.8f};
    ui.run(state);
}

int main(int argc, char *argv[]) {
    pw_init(&argc, &argv);

    AudioEngine audio;
    std::jthread uiThread(run);

    audio.run();

    return 0;
}
