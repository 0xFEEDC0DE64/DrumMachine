#include "drummachinesettings.h"

#include <QStandardPaths>
#include <QDebug>

#include "midilearnsetting.h"
#include "futurecpp.h"

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

MidiLearnSetting DrumMachineSettings::tabWidget(quint8 index) const
{
    return learnSetting(QString{"tabWidget%0"}.arg(index));
}

void DrumMachineSettings::setTabWidget(quint8 index, const MidiLearnSetting &value)
{
    setLearnSetting(QString{"tabWidget%0"}.arg(index), value);
}

QString DrumMachineSettings::drumpadLastPresetId() const
{
    return value("drumpad/lastPresetId").toString();
}

void DrumMachineSettings::setDrumpadLastPresetId(const QString &lastPresetId)
{
    setValue("drumpad/lastPresetId", lastPresetId);
}

MidiLearnSetting DrumMachineSettings::drumpadPrevPreset() const
{
    return learnSetting("drumpad/prevPreset");
}

void DrumMachineSettings::setDrumpadPrevPreset(const MidiLearnSetting &value)
{
    setLearnSetting("drumpad/prevPreset", value);
}

MidiLearnSetting DrumMachineSettings::drumpadNextPreset() const
{
    return learnSetting("drumpad/nextPreset");
}

void DrumMachineSettings::setDrumpadNextPreset(const MidiLearnSetting &value)
{
    setLearnSetting("drumpad/nextPreset", value);
}

MidiLearnSetting DrumMachineSettings::drumpadPrevSequence() const
{
    return learnSetting("drumpad/prevSequence");
}

void DrumMachineSettings::setDrumpadPrevSequence(const MidiLearnSetting &value)
{
    setLearnSetting("drumpad/prevSequence", value);
}

MidiLearnSetting DrumMachineSettings::drumpadNextSequence() const
{
    return learnSetting("drumpad/nextSequence");
}

void DrumMachineSettings::setDrumpadNextSequence(const MidiLearnSetting &value)
{
    setLearnSetting("drumpad/nextSequence", value);
}

MidiLearnSetting DrumMachineSettings::drumpadPlayPause() const
{
    return learnSetting("drumpad/playPause");
}

void DrumMachineSettings::setDrumpadPlayPause(const MidiLearnSetting &value)
{
    setLearnSetting("drumpad/playPause", value);
}

MidiLearnSetting DrumMachineSettings::drumpadStop() const
{
    return learnSetting("drumpad/stop");
}

void DrumMachineSettings::setDrumpadStop(const MidiLearnSetting &value)
{
    setLearnSetting("drumpad/stop", value);
}

MidiLearnSetting DrumMachineSettings::drumpadSample(quint8 pad) const
{
    return learnSetting(QString{"drumpad/pad%0"}.arg(pad));
}

void DrumMachineSettings::setDrumpadSample(quint8 pad, const MidiLearnSetting &value)
{
    setLearnSetting(QString{"drumpad/pad%0"}.arg(pad), value);
}

QString DrumMachineSettings::loopstationLastPresetId() const
{
    return value("loopstation/lastPresetId").toString();
}

void DrumMachineSettings::setLoopstationLastPresetId(const QString &lastPresetId)
{
    setValue("loopstation/lastPresetId", lastPresetId);
}

MidiLearnSetting DrumMachineSettings::loopstationPrevPreset() const
{
    return learnSetting("loopstation/prevPreset");
}

void DrumMachineSettings::setLoopstationPrevPreset(const MidiLearnSetting &value)
{
    setLearnSetting("loopstation/prevPreset", value);
}

MidiLearnSetting DrumMachineSettings::loopstationNextPreset() const
{
    return learnSetting("loopstation/nextPreset");
}

void DrumMachineSettings::setLoopstationNextPreset(const MidiLearnSetting &value)
{
    setLearnSetting("loopstation/nextPreset", value);
}

MidiLearnSetting DrumMachineSettings::learnSetting(const QString &key) const
{
    return MidiLearnSetting{
        .cmd = midi::Command(value(QString{"%0_cmd"}.arg(key), std::to_underlying(midi::Command::NoteOn)).value<std::underlying_type_t<midi::Command>>()),
        .channel = value(QString{"%0_channel"}.arg(key), 99).value<quint8>(),
        .note = value(QString{"%0_note"}.arg(key), 99).value<quint8>()
    };
}

void DrumMachineSettings::setLearnSetting(const QString &key, const MidiLearnSetting &value)
{
    setValue(QString{"%0_note"}.arg(key), std::to_underlying(value.cmd));
    setValue(QString{"%0_channel"}.arg(key), value.channel);
    setValue(QString{"%0_note"}.arg(key), value.note);
}
