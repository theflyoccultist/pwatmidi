#pragma once

#include <cstdint>
#include "AudioState.hpp"

enum class ENVELOPE : uint8_t { IDLE, ATTACK, SUSTAIN, RELEASE };

inline const char *display_envelope(ENVELOPE env) {
    switch (env) {
    case ENVELOPE::IDLE:
        return "IDLE";
    case ENVELOPE::ATTACK:
        return "ATTACK";
    case ENVELOPE::SUSTAIN:
        return "SUSTAIN";
    case ENVELOPE::RELEASE:
        return "RELEASE";
    default:
        return "";
    }
}

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
    void trigger_note();
    float process_sample(float raw_oscillator_sample);
    bool is_active() const { return env_state_ != ENVELOPE::IDLE; }
};
