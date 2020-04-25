#pragma once

#include <QObject>

namespace midi {
Q_NAMESPACE

enum class Command : uint8_t {
    NoteOff,
    NoteOn,
    PolyphonicKeyPressure,
    ControlChange,
    ProgramChange,
    ChannelPressure,
    PitchBendChange
};
Q_ENUM_NS(Command)

struct MidiMessage
{
    uint8_t channel:4;
    Command cmd:3;
    bool flag:1;
    uint8_t note;
    uint8_t velocity;

    bool operator==(const MidiMessage &other) const;
};
}

Q_DECLARE_METATYPE(midi::MidiMessage)
