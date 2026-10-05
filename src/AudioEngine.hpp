#pragma once

#include <array>
#include <atomic>
#include "AudioState.hpp"

#include <spa/param/audio/format-utils.h>

#include <pipewire/pipewire.h>

struct AudioState {
    std::atomic<float> freq{440.0f};
    std::atomic<bool> is_muted{false};
    std::atomic<float> vol{0.8f};
};

class AudioEngine {
  public:
    explicit AudioEngine(AudioState &state);
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
    AudioState &state_;

    std::array<const struct spa_pod *, 1> params{};

    std::array<int16_t, 1024> buffer{};
    struct spa_pod_builder b{};
};
