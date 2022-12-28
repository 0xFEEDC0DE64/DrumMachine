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

} // namespace midi

Q_DECLARE_METATYPE(midi::Command)
Q_DECLARE_METATYPE(midi::MidiMessage)

QDebug operator<<(QDebug debug, const midi::MidiMessage &value);

QDataStream &operator<<(QDataStream &out, const midi::Command &value);
QDataStream &operator>>(QDataStream &in, midi::Command &value);

QDataStream &operator<<(QDataStream &out, const midi::MidiMessage &value);
QDataStream &operator>>(QDataStream &in, midi::MidiMessage &value);
