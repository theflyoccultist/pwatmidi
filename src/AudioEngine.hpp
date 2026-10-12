#pragma once

#include <array>
#include <atomic>
#include "Voice.hpp"

#include <spa/param/audio/format-utils.h>

#include <pipewire/pipewire.h>

constexpr double M_PI_M2(M_PI + M_PI);

constexpr int DEFAULT_RATE = 44100;
constexpr int DEFAULT_CHANNELS = 2;

constexpr float NOTE_DURATION_SEC = 0.5f;
constexpr uint32_t DURATION_SAMPLES = static_cast<uint32_t>(DEFAULT_RATE * NOTE_DURATION_SEC);

struct AudioState {
    std::atomic<float> freq{523.3f};
    std::atomic<bool> is_muted{true};
    std::atomic<float> vol{0.8f};
    std::atomic<uint32_t> samples_remaining{0};
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

class SimpleVoice {
  public:
    explicit SimpleVoice(AudioState &state);

  private:
    AudioState &state_;

    float attack_time_sec_ = 0.005f;  // 5ms
    float release_time_sec_ = 0.020f; // 20ms

    float attack_step_ = 0.0f;
    float release_step_ = 0.0f;

  public:
    ENVELOPE env_state_ = ENVELOPE::IDLE;
    void prepare(float sample_rate);
    void trigger_note(uint32_t sustain_duration_samples);
    float process_sample(float raw_oscillator_sample);
    bool is_active() const { return env_state_ != ENVELOPE::IDLE; }
};
