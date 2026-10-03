#pragma once

#include <array>

#include <spa/param/audio/format-utils.h>

#include <pipewire/pipewire.h>

class AudioEngine {
  public:
    AudioEngine();
    ~AudioEngine();

    void run();

    AudioEngine(const AudioEngine &) = delete;
    AudioEngine &operator=(const AudioEngine &) = delete;

    AudioEngine(AudioEngine &&) = delete;
    AudioEngine &operator=(AudioEngine &&) = delete;

  private:
    static AudioEngine *s_instance;
    static void on_process(void *userdata);
    static const struct pw_stream_events stream_events;

    pw_main_loop *loop_ = nullptr;
    pw_stream *stream_ = nullptr;
    double accumulator_{};

    std::array<const struct spa_pod *, 1> params{};

    std::array<int16_t, 1024> buffer{};
    struct spa_pod_builder b{};
};
