#include "AudioEngine.hpp"
#include "UI.hpp"

int main(int argc, char *argv[]) {
    pw_init(&argc, &argv);

    AudioEngine audio;
    UI ui;

    audio.run();
    ui.run();

    return 0;
}
