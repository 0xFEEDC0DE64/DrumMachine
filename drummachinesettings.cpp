#include "drummachinesettings.h"

QString DrumMachineSettings::lastAudioDevice() const
{
    return value("lastAudioDevice").toString();
}

void DrumMachineSettings::setLastAudioDevice(const QString &lastAudioDevice)
{
    setValue("lastAudioDevice", lastAudioDevice);
}

unsigned int DrumMachineSettings::framesPerBuffer() const
{
    return value("framesPerBuffer", 32).toUInt();
}

void DrumMachineSettings::setFramesPerBuffer(unsigned int framesPerBuffer)
{
    setValue("framesPerBuffer", framesPerBuffer);
}

QString DrumMachineSettings::lastMidiInDevice() const
{
    return value("lastMidiInDevice").toString();
}

void DrumMachineSettings::setLastMidiInDevice(const QString &lastMidiInDevice)
{
    setValue("lastMidiInDevice", lastMidiInDevice);
}

QString DrumMachineSettings::lastMidiOutDevice() const
{
    return value("lastMidiOutDevice").toString();
}

void DrumMachineSettings::setLastMidiOutDevice(const QString &lastMidiOutDevice)
{
    setValue("lastMidiOutDevice", lastMidiOutDevice);
}

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
