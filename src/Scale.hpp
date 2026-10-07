#pragma once

#include <string>
#include <type_traits>
#include <cstdint>
#include <unordered_map>
#include <vector>

enum class SCALE : uint8_t {
    MAJOR,
    MINOR,
    HARMONIC_MINOR,
    PHRYGIAN,
    LYDIAN,
    MIXOLYDIAN,
    AEOLIAN,
    LOCRIAN,
    MAJOR_PENTATONIC,
    MINOR_PENTATONIC,
    MAJOR_BLUES,
    MINOR_BLUES,
    WHOLE_TONE,
    DIMINISHED_WH,
    DIMINISHED_HW,
    CHROMATIC,
    END
};

SCALE &operator++(SCALE &s) {
    using IntType = std::underlying_type_t<SCALE>;
    s = static_cast<SCALE>(static_cast<IntType>(s) + 1);
    if (s == SCALE::END)
        s = static_cast<SCALE>(0);

    return s;
}

const char *display_scale(SCALE scale) {
    switch (scale) {
    case SCALE::MAJOR:
        return "MAJOR";
    case SCALE::MINOR:
        return "MINOR";
    case SCALE::HARMONIC_MINOR:
        return "HARMONIC_MINOR";
    case SCALE::PHRYGIAN:
        return "PHRYGIAN";
    case SCALE::LYDIAN:
        return "LYDIAN";
    case SCALE::MIXOLYDIAN:
        return "MIXOLYDIAN";
    case SCALE::AEOLIAN:
        return "AEOLIAN";
    case SCALE::LOCRIAN:
        return "LOCRIAN";
    case SCALE::MAJOR_PENTATONIC:
        return "MAJOR_PENTATONIC";
    case SCALE::MINOR_PENTATONIC:
        return "MINOR_PENTATONIC";
    case SCALE::MAJOR_BLUES:
        return "MAJOR_BLUES";
    case SCALE::MINOR_BLUES:
        return "MINOR_BLUES";
    case SCALE::WHOLE_TONE:
        return "WHOLE_TONE";
    case SCALE::DIMINISHED_WH:
        return "DIMINISHED_WH";
    case SCALE::DIMINISHED_HW:
        return "DIMINISHED_HW";
    case SCALE::CHROMATIC:
        return "CHROMATIC";
    default:
        return "";
    }
}

const std::vector<std::string> base_notes = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B",
};

const std::unordered_map<SCALE, std::vector<size_t>> scales = {
    {SCALE::MAJOR, {0, 2, 4, 5, 7, 9, 11}},
    {SCALE::MINOR, {0, 2, 3, 5, 7, 8, 10}},
    {SCALE::HARMONIC_MINOR, {0, 2, 3, 5, 7, 8, 11}},
    {SCALE::PHRYGIAN, {0, 1, 3, 5, 7, 8, 10}},
    {SCALE::LYDIAN, {0, 2, 4, 6, 7, 9, 11}},
    {SCALE::MIXOLYDIAN, {0, 2, 4, 5, 7, 9, 10}},
    {SCALE::AEOLIAN, {0, 2, 3, 5, 7, 8, 10}},
    {SCALE::LOCRIAN, {0, 1, 3, 5, 6, 8, 10}},
    {SCALE::MAJOR_PENTATONIC, {0, 2, 4, 7, 9}},
    {SCALE::MINOR_PENTATONIC, {0, 3, 5, 7, 10}},
    {SCALE::MAJOR_BLUES, {0, 2, 3, 4, 7, 9}},
    {SCALE::MINOR_BLUES, {0, 3, 5, 6, 7, 9}},
    {SCALE::WHOLE_TONE, {0, 2, 4, 6, 8, 10}},
    {SCALE::DIMINISHED_WH, {0, 2, 3, 5, 6, 8, 9, 11}},
    {SCALE::DIMINISHED_HW, {0, 1, 3, 4, 6, 7, 9, 10}},
    {SCALE::CHROMATIC, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}},
};

const std::vector<float> base_freqs = {
    261.63f, 277.18f, 293.66f, 311.13f, 329.63f, 349.23f,
    369.99f, 392.0f,  415.3f,  440.0f,  466.16f, 493.88f,
};
