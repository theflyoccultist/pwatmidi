#pragma once

#include <cstdint>
#include <MidiFile.h>

enum class ENVELOPE : uint8_t { ATTACK, DECAY, SUSTAIN, RELEASE };

inline const char *display_envelope(ENVELOPE env) {
    switch (env) {
    case ENVELOPE::ATTACK:
        return "ATTACK";
    case ENVELOPE::DECAY:
        return "DECAY";
    case ENVELOPE::SUSTAIN:
        return "SUSTAIN";
    case ENVELOPE::RELEASE:
        return "RELEASE";
    default:
        return "";
    }
}
