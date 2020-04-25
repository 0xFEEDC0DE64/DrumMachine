#include "midicontainers.h"

#include <QCoreApplication>

namespace midi {
bool MidiMessage::operator==(const MidiMessage &other) const
{
    return channel == other.channel &&
           cmd == other.cmd &&
           flag == other.flag &&
           note == other.note &&
           velocity == other.velocity;
}
}

namespace {
void registerMidiMessageMetatype()
{
    qRegisterMetaType<midi::MidiMessage>();
}

Q_COREAPP_STARTUP_FUNCTION(registerMidiMessageMetatype)
}
