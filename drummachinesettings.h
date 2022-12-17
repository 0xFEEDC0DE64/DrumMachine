#pragma once

#include <QSettings>

class DrumMachineSettings : public QSettings
{
public:
    using QSettings::QSettings;

    quint8 padChannel(quint8 pad) const;
    void setPadChannel(quint8 pad, quint8 channel);

    quint8 padNote(quint8 pad) const;
    void setPadNote(quint8 pad, quint8 note);
};
