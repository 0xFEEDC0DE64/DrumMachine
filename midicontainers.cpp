#include "midicontainers.h"

#include <QCoreApplication>
#include <QDebugStateSaver>
#include <QDataStream>

#include "futurecpp.h"

namespace midi {
bool MidiMessage::operator==(const MidiMessage &other) const
{
    return channel == other.channel &&
           cmd == other.cmd &&
           flag == other.flag &&
           note == other.note &&
           velocity == other.velocity;
}
} // namespace midi

namespace {
void registerMidiMessageMetatype()
{
    qRegisterMetaType<midi::Command>();
    qRegisterMetaType<midi::MidiMessage>();

    qRegisterMetaTypeStreamOperators<midi::Command>("midi::Command");
    qRegisterMetaTypeStreamOperators<midi::MidiMessage>("midi::MidiMessage");
}

Q_COREAPP_STARTUP_FUNCTION(registerMidiMessageMetatype)
} // namespace

QDebug operator<<(QDebug debug, const midi::MidiMessage &value)
{
    QDebugStateSaver saver{debug};
    debug.nospace() << "midi::MidiMessage{.channel=" << value.channel << ",.cmd=" << value.cmd << ",.flag=" << value.flag << ",.note=" << value.note << ",.velocity=" << value.velocity << "}";
    return debug;
}

QDataStream &operator<<(QDataStream &out, const midi::Command &value)
{
    return out << std::to_underlying(value);
}

QDataStream &operator>>(QDataStream &in, midi::Command &value)
{
    std::underlying_type_t<midi::Command> v;
    in >> v;
    value = midi::Command(v);
    return in;
}

QDataStream &operator<<(QDataStream &out, const midi::MidiMessage &value)
{
    return out << value.channel << value.cmd << value.flag << value.note << value.velocity;
}

QDataStream &operator>>(QDataStream &in, midi::MidiMessage &value)
{
    {
        typeof(value.channel) channel;
        in >> channel;
        value.channel = channel;
    }
    {
        typeof(value.cmd) cmd;
        in >> cmd;
        value.cmd = cmd;
    }
    {
        typeof(value.flag) flag;
        in >> flag;
        value.flag = flag;
    }
    in >> value.note;
    in >> value.velocity;
    return in;
}
