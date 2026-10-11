#pragma once

#include <MidiFile.h>

using namespace smf;

void write() {
    MidiFile midifile;
    midifile.read("pwat.mid");
    midifile.write("otpt.mid");
}
