#include "AudioEngine.hpp"
#include "UI.hpp"
#include <functional>
#include <thread>

void runUI(AudioState &shared_state) {
    UI ui(shared_state);
    ui.run();
}

int main(int argc, char *argv[]) {
    pw_init(&argc, &argv);

    AudioState shared_state;
    AudioEngine audio(shared_state);
    std::jthread uiThread(runUI, std::ref(shared_state));

    audio.run();

    return 0;
}
