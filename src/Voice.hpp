#pragma once

#include <cstdint>
#include <MidiFile.h>

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
