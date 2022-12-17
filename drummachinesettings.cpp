#include "drummachinesettings.h"

quint8 DrumMachineSettings::padChannel(quint8 pad) const
{
    return value(QString{"pad%0/channel"}.arg(pad)).toUInt();
}

void DrumMachineSettings::setPadChannel(quint8 pad, quint8 channel)
{
    setValue(QString{"pad%0/channel"}.arg(pad), channel);
}

quint8 DrumMachineSettings::padNote(quint8 pad) const
{
    return value(QString{"pad%0/note"}.arg(pad)).toUInt();
}

void DrumMachineSettings::setPadNote(quint8 pad, quint8 note)
{
    setValue(QString{"pad%0/note"}.arg(pad), note);
}
