#include "drummachinesettings.h"

#include <QStandardPaths>
#include <QDebug>

QString DrumMachineSettings::defaultCacheDir() const
{
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}

QString DrumMachineSettings::cacheDir() const
{
    return value("cacheDir", defaultCacheDir()).toString();
}

void DrumMachineSettings::setCacheDir(const QString &cacheDir)
{
    setValue("cacheDir", cacheDir);
}

qint64 DrumMachineSettings::defaultMaximumCacheSize() const
{
    return 2ull * 1024 * 1024 * 1024;
}

qint64 DrumMachineSettings::maximumCacheSize() const
{
    return value("maximumCacheSize", defaultMaximumCacheSize()).value<qint64>();
}

void DrumMachineSettings::setMaximumCacheSize(qint64 maximumCacheSize)
{
    setValue("maximumCacheSize", maximumCacheSize);
}

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

QString DrumMachineSettings::drumpadLastPresetId() const
{
    return value("drumpad/lastPresetId").toString();
}

void DrumMachineSettings::setDrumpadLastPresetId(const QString &lastPresetId)
{
    setValue("drumpad/lastPresetId", lastPresetId);
}

quint8 DrumMachineSettings::drumpadChannelPrevPreset() const
{
    return value("drumpad/prevPreset_channel", 99).toUInt();
}

void DrumMachineSettings::setDrumpadChannelPrevPreset(quint8 channel)
{
    setValue("drumpad/prevPreset_channel", channel);
}

quint8 DrumMachineSettings::drumpadNotePrevPreset() const
{
    return value("drumpad/prevPreset_note", 99).toUInt();
}

void DrumMachineSettings::setDrumpadNotePrevPreset(quint8 note)
{
    setValue("drumpad/prevPreset_note", note);
}

quint8 DrumMachineSettings::drumpadChannelNextPreset() const
{
    return value("drumpad/nextPreset_channel", 99).toUInt();
}

void DrumMachineSettings::setDrumpadChannelNextPreset(quint8 channel)
{
    setValue("drumpad/nextPreset_channel", channel);
}

quint8 DrumMachineSettings::drumpadNoteNextPreset() const
{
    return value("drumpad/nextPreset_note", 99).toUInt();
}

void DrumMachineSettings::setDrumpadNoteNextPreset(quint8 note)
{
    setValue("drumpad/nextPreset_note", note);
}

quint8 DrumMachineSettings::drumpadChannelPrevSequence() const
{
    return value("drumpad/prevSequence_channel", 99).toUInt();
}

void DrumMachineSettings::setDrumpadChannelPrevSequence(quint8 channel)
{
    setValue("drumpad/prevSequence_channel", channel);
}

quint8 DrumMachineSettings::drumpadNotePrevSequence() const
{
    return value("drumpad/prevSequence_note", 99).toUInt();
}

void DrumMachineSettings::setDrumpadNotePrevSequence(quint8 note)
{
    setValue("drumpad/prevSequence_note", note);
}

quint8 DrumMachineSettings::drumpadChannelNextSequence() const
{
    return value("drumpad/nextSequence_channel", 99).toUInt();
}

void DrumMachineSettings::setDrumpadChannelNextSequence(quint8 channel)
{
    setValue("drumpad/nextSequence_channel", channel);
}

quint8 DrumMachineSettings::drumpadNoteNextSequence() const
{
    return value("drumpad/nextSequence_note", 99).toUInt();
}

void DrumMachineSettings::setDrumpadNoteNextSequence(quint8 note)
{
    setValue("drumpad/nextSequence_note", note);
}

quint8 DrumMachineSettings::drumpadChannelPlayPause() const
{
    return value("drumpad/playPause_channel", 99).toUInt();
}

void DrumMachineSettings::setDrumpadChannelPlayPause(quint8 channel)
{
    setValue("drumpad/playPause_channel", channel);
}

quint8 DrumMachineSettings::drumpadNotePlayPause() const
{
    return value("drumpad/playPause_note", 99).toUInt();
}

void DrumMachineSettings::setDrumpadNotePlayPause(quint8 note)
{
    setValue("drumpad/playPause_note", note);
}

quint8 DrumMachineSettings::drumpadChannelStop() const
{
    return value("drumpad/stop_channel", 99).toUInt();
}

void DrumMachineSettings::setDrumpadChannelStop(quint8 channel)
{
    setValue("drumpad/stop_channel", channel);
}

quint8 DrumMachineSettings::drumpadNoteStop() const
{
    return value("drumpad/stop_note", 99).toUInt();
}

void DrumMachineSettings::setDrumpadNoteStop(quint8 note)
{
    setValue("drumpad/stop_note", note);
}

quint8 DrumMachineSettings::drumpadChannel(quint8 pad) const
{
    return value(QString{"drumpad/pad%0_channel"}.arg(pad), 99).toUInt();
}

void DrumMachineSettings::setDrumpadChannel(quint8 pad, quint8 channel)
{
    setValue(QString{"drumpad/pad%0_channel"}.arg(pad), channel);
}

quint8 DrumMachineSettings::drumpadNote(quint8 pad) const
{
    return value(QString{"drumpad/pad%0_note"}.arg(pad), 99).toUInt();
}

void DrumMachineSettings::setDrumpadNote(quint8 pad, quint8 note)
{
    setValue(QString{"drumpad/pad%0_note"}.arg(pad), note);
}
