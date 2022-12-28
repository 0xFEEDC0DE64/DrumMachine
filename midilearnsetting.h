#pragma once

#include <QDebug>
#include <QMetaType>

#include "midicontainers.h"

struct MidiLearnSetting
{
    midi::Command cmd{midi::Command::NoteOn};
    quint8 channel{99};
    quint8 note{99};

    bool operator==(const MidiLearnSetting &other) const;
    QDebug operator<<(QDebug debug) const;
};

Q_DECLARE_METATYPE(MidiLearnSetting);

QDebug operator<<(QDebug debug, const MidiLearnSetting &value);

QDataStream &operator<<(QDataStream &out, const MidiLearnSetting &value);
QDataStream &operator>>(QDataStream &in, MidiLearnSetting &value);
