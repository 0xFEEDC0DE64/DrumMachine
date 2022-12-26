#pragma once

#include <QSettings>

class DrumMachineSettings : public QSettings
{
public:
    using QSettings::QSettings;

    QString lastAudioDevice() const;
    void setLastAudioDevice(const QString &lastAudioDevice);

    unsigned int framesPerBuffer() const;
    void setFramesPerBuffer(unsigned int framesPerBuffer);

    QString lastMidiInDevice() const;
    void setLastMidiInDevice(const QString &lastMidiInDevice);

    QString lastMidiOutDevice() const;
    void setLastMidiOutDevice(const QString &lastMidiOutDevice);

    quint8 padChannel(quint8 pad) const;
    void setPadChannel(quint8 pad, quint8 channel);

    quint8 padNote(quint8 pad) const;
    void setPadNote(quint8 pad, quint8 note);
};
