#include "midilearnsetting.h"

#include <QCoreApplication>
#include <QDebugStateSaver>
#include <QDataStream>

namespace {
void registerMidiLearnMessageMetatype()
{
    qRegisterMetaType<MidiLearnSetting>();

    qRegisterMetaTypeStreamOperators<MidiLearnSetting>("MidiLearnSetting");
}

Q_COREAPP_STARTUP_FUNCTION(registerMidiLearnMessageMetatype)
} // namespace

bool MidiLearnSetting::operator==(const MidiLearnSetting &other) const
{
    return cmd == other.cmd &&
           channel == other.channel &&
           note == other.note;
}

QDebug operator<<(QDebug debug, const MidiLearnSetting &value)
{
    QDebugStateSaver saver{debug};
    debug.nospace() << "MidiLearnSetting{.cmd=" << value.cmd << ",.channel=" << value.channel << ",.note=" << value.note << "}";
    return debug;
}

QDataStream &operator<<(QDataStream &out, const MidiLearnSetting &value)
{
    out << value.cmd << value.channel << value.note;
    return out;
}

QDataStream &operator>>(QDataStream &in, MidiLearnSetting &value)
{
    in >> value.cmd >> value.channel >> value.note;
    return in;
}
