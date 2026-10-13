#include "Voice.hpp"

SimpleVoice::SimpleVoice(AudioState &state) : state_(state) {}

void SimpleVoice::prepare(float sample_rate) {
    attack_step_ = 1.0f / (attack_time_sec_ * sample_rate);
    release_step_ = 1.0f / (release_time_sec_ * sample_rate);
}

void SimpleVoice::trigger_note() { env_state_ = ENVELOPE::ATTACK; }

float SimpleVoice::process_sample(float scale) {
    uint32_t samples_remaining = state_.samples_remaining.load(std::memory_order_relaxed);
    float current_gain = 0;

    switch (env_state_) {
    case ENVELOPE::ATTACK:
        current_gain += attack_step_;
        if (current_gain >= 0.8f) {
            current_gain = 1.0f;
            env_state_ = ENVELOPE::SUSTAIN;
        }
        break;

    case ENVELOPE::SUSTAIN:
        if (samples_remaining > 0) {
            --samples_remaining;
        } else {
            env_state_ = ENVELOPE::RELEASE;
        }
        break;

    case ENVELOPE::RELEASE:
        current_gain -= release_step_;
        if (current_gain <= 0.0f) {
            current_gain = 0.0f;
            env_state_ = ENVELOPE::IDLE;
        }
        break;

    case ENVELOPE::IDLE:
        state_.is_muted.store(true, std::memory_order_relaxed);
        break;
    }

    return current_gain * scale;
}
