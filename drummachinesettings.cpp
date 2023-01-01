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

bool DrumMachineSettings::autoOpenAudioDevice() const
{
    return value("autoOpenAudioDevice").toBool();
}

void DrumMachineSettings::setAutoOpenAudioDevice(bool autoOpenAudioDevice)
{
    setValue("autoOpenAudioDevice", autoOpenAudioDevice);
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

bool DrumMachineSettings::autoOpenMidiIn() const
{
    return value("autoOpenMidiIn").toBool();
}

void DrumMachineSettings::setAutoOpenMidiIn(bool autoOpenMidiIn)
{
    setValue("autoOpenMidiIn", autoOpenMidiIn);
}

QString DrumMachineSettings::lastMidiOutDevice() const
{
    return value("lastMidiOutDevice").toString();
}

void DrumMachineSettings::setLastMidiOutDevice(const QString &lastMidiOutDevice)
{
    setValue("lastMidiOutDevice", lastMidiOutDevice);
}

bool DrumMachineSettings::autoOpenMidiOut() const
{
    return value("autoOpenMidiOut").toBool();
}

void DrumMachineSettings::setAutoOpenMidiOut(bool autoOpenMidiOut)
{
    setValue("autoOpenMidiOut", autoOpenMidiOut);
}

quint8 DrumMachineSettings::colorOff() const
{
    return value("colorOff", defaultColorOff()).value<quint8>();
}

quint8 DrumMachineSettings::defaultColorOff() const
{
    return 0;
}

void DrumMachineSettings::setColorOff(quint8 colorOff)
{
    setValue("colorOff", colorOff);
}

MidiLearnSetting DrumMachineSettings::tabWidget(quint8 index) const
{
    return learnSetting(QString{"tabWidget%0"}.arg(index));
}

void DrumMachineSettings::setTabWidget(quint8 index, const MidiLearnSetting &value)
{
    setLearnSetting(QString{"tabWidget%0"}.arg(index), value);
}

quint8 DrumMachineSettings::colorTabWidget() const
{
    return value("colorTabWidget", defaultColorTabWidget()).value<quint8>();
}

quint8 DrumMachineSettings::defaultColorTabWidget() const
{
    return 3;
}

void DrumMachineSettings::setColorTabWidget(quint8 colorTabWidget)
{
    setValue("colorTabWidget", colorTabWidget);
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

quint8 DrumMachineSettings::drumpadColorPrevPreset() const
{
    return value("drumpad/colorPrevPreset", defaultDrumpadColorPrevPreset()).value<quint8>();
}

quint8 DrumMachineSettings::defaultDrumpadColorPrevPreset() const
{
    return 127;
}

void DrumMachineSettings::setDrumpadColorPrevPreset(quint8 drumpadColorPrevPreset)
{
    setValue("drumpad/colorPrevPreset", drumpadColorPrevPreset);
}

MidiLearnSetting DrumMachineSettings::drumpadNextPreset() const
{
    return learnSetting("drumpad/nextPreset");
}

void DrumMachineSettings::setDrumpadNextPreset(const MidiLearnSetting &value)
{
    setLearnSetting("drumpad/nextPreset", value);
}

quint8 DrumMachineSettings::drumpadColorNextPreset() const
{
    return value("drumpad/colorNextPreset", defaultDrumpadColorNextPreset()).value<quint8>();
}

quint8 DrumMachineSettings::defaultDrumpadColorNextPreset() const
{
    return 127;
}

void DrumMachineSettings::setDrumpadColorNextPreset(quint8 drumpadColorNextPreset)
{
    setValue("drumpad/colorNextPreset", drumpadColorNextPreset);
}

MidiLearnSetting DrumMachineSettings::drumpadPrevSequence() const
{
    return learnSetting("drumpad/prevSequence");
}

void DrumMachineSettings::setDrumpadPrevSequence(const MidiLearnSetting &value)
{
    setLearnSetting("drumpad/prevSequence", value);
}

quint8 DrumMachineSettings::drumpadColorPrevSequence() const
{
    return value("drumpad/colorPrevSequence", defaultDrumpadColorPrevSequence()).value<quint8>();
}

quint8 DrumMachineSettings::defaultDrumpadColorPrevSequence() const
{
    return 127;
}

void DrumMachineSettings::setDrumpadColorPrevSequence(quint8 drumpadColorPrevSequence)
{
    setValue("drumpad/colorPrevSequence", drumpadColorPrevSequence);
}

MidiLearnSetting DrumMachineSettings::drumpadNextSequence() const
{
    return learnSetting("drumpad/nextSequence");
}

void DrumMachineSettings::setDrumpadNextSequence(const MidiLearnSetting &value)
{
    setLearnSetting("drumpad/nextSequence", value);
}

quint8 DrumMachineSettings::drumpadColorNextSequence() const
{
    return value("drumpad/colorNextSequence", defaultDrumpadColorNextSequence()).value<quint8>();
}

quint8 DrumMachineSettings::defaultDrumpadColorNextSequence() const
{
    return 127;
}

void DrumMachineSettings::setDrumpadColorNextSequence(quint8 drumpadColorNextSequence)
{
    setValue("drumpad/colorNextSequence", drumpadColorNextSequence);
}

MidiLearnSetting DrumMachineSettings::drumpadPlayPause() const
{
    return learnSetting("drumpad/playPause");
}

void DrumMachineSettings::setDrumpadPlayPause(const MidiLearnSetting &value)
{
    setLearnSetting("drumpad/playPause", value);
}

quint8 DrumMachineSettings::drumpadColorPlayPause() const
{
    return value("drumpad/colorPlayPause", defaultDrumpadColorPlayPause()).value<quint8>();
}

quint8 DrumMachineSettings::defaultDrumpadColorPlayPause() const
{
    return 3;
}

void DrumMachineSettings::setDrumpadColorPlayPause(quint8 drumpadColorPlayPause)
{
    setValue("drumpad/colorPlayPause", drumpadColorPlayPause);
}

MidiLearnSetting DrumMachineSettings::drumpadStopSequence() const
{
    return learnSetting("drumpad/stopSequence");
}

void DrumMachineSettings::setDrumpadStopSequence(const MidiLearnSetting &value)
{
    setLearnSetting("drumpad/stopSequence", value);
}

quint8 DrumMachineSettings::drumpadColorStopSequence() const
{
    return value("drumpad/colorStopSequence", defaultDrumpadColorStopSequence()).value<quint8>();
}

quint8 DrumMachineSettings::defaultDrumpadColorStopSequence() const
{
    return 60;
}

void DrumMachineSettings::setDrumpadColorStopSequence(quint8 drumpadColorStopSequence)
{
    setValue("drumpad/colorStopSequence", drumpadColorStopSequence);
}

MidiLearnSetting DrumMachineSettings::drumpadSwap() const
{
    return learnSetting("drumpad/swap");
}

void DrumMachineSettings::setDrumpadSwap(const MidiLearnSetting &value)
{
    setLearnSetting("drumpad/swap", value);
}

quint8 DrumMachineSettings::drumpadColorSwap() const
{
    return value("drumpad/colorSwap", defaultDrumpadColorSwap()).value<quint8>();
}

quint8 DrumMachineSettings::defaultDrumpadColorSwap() const
{
    return 127;
}

void DrumMachineSettings::setDrumpadColorSwap(quint8 drumpadColorSwap)
{
    setValue("drumpad/colorSwap", drumpadColorSwap);
}

MidiLearnSetting DrumMachineSettings::drumpadStopAll() const
{
    return learnSetting("drumpad/stopAll");
}

void DrumMachineSettings::setDrumpadStopAll(const MidiLearnSetting &value)
{
    setLearnSetting("drumpad/stopAll", value);
}

quint8 DrumMachineSettings::drumpadColorStopAll() const
{
    return value("drumpad/colorStopAll", defaultDrumpadColorStopAll()).value<quint8>();
}

quint8 DrumMachineSettings::defaultDrumpadColorStopAll() const
{
    return 60;
}

void DrumMachineSettings::setDrumpadColorStopAll(quint8 drumpadColorStopAll)
{
    setValue("drumpad/colorStopAll", drumpadColorStopAll);
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

quint8 DrumMachineSettings::loopstationColorPrevPreset() const
{
    return value("loopstation/colorPrevPreset", defaultLoopstationColorPrevPreset()).value<quint8>();
}

quint8 DrumMachineSettings::defaultLoopstationColorPrevPreset() const
{
    return 127;
}

void DrumMachineSettings::setLoopstationColorPrevPreset(quint8 loopstationColorPrevPreset)
{
    setValue("loopstation/colorPrevPreset", loopstationColorPrevPreset);
}

MidiLearnSetting DrumMachineSettings::loopstationNextPreset() const
{
    return learnSetting("loopstation/nextPreset");
}

void DrumMachineSettings::setLoopstationNextPreset(const MidiLearnSetting &value)
{
    setLearnSetting("loopstation/nextPreset", value);
}

quint8 DrumMachineSettings::loopstationColorNextPreset() const
{
    return value("loopstation/colorNextPreset", defaultLoopstationColorNextPreset()).value<quint8>();
}

quint8 DrumMachineSettings::defaultLoopstationColorNextPreset() const
{
    return 127;
}

void DrumMachineSettings::setLoopstationColorNextPreset(quint8 loopstationColorNextPreset)
{
    setValue("loopstation/colorNextPreset", loopstationColorNextPreset);
}

MidiLearnSetting DrumMachineSettings::loopstationPlayPause() const
{
    return learnSetting("loopstation/playPause");
}

void DrumMachineSettings::setLoopstationPlayPause(const MidiLearnSetting &value)
{
    setLearnSetting("loopstation/playPause", value);
}

quint8 DrumMachineSettings::loopstationColorPlayPause() const
{
    return value("loopstation/colorPlayPause", defaultLoopstationColorPlayPause()).value<quint8>();
}

quint8 DrumMachineSettings::defaultLoopstationColorPlayPause() const
{
    return 3;
}

void DrumMachineSettings::setLoopstationColorPlayPause(quint8 loopstationColorPlayPause)
{
    setValue("loopstation/colorPlayPause", loopstationColorPlayPause);
}

MidiLearnSetting DrumMachineSettings::loopstationStop() const
{
    return learnSetting("loopstation/stop");
}

void DrumMachineSettings::setLoopstationStop(const MidiLearnSetting &value)
{
    setLearnSetting("loopstation/stop", value);
}

quint8 DrumMachineSettings::loopstationColorStop() const
{
    return value("loopstation/colorStop", defaultLoopstationColorStop()).value<quint8>();
}

quint8 DrumMachineSettings::defaultLoopstationColorStop() const
{
    return 60;
}

void DrumMachineSettings::setLoopstationColorStop(quint8 loopstationColorStop)
{
    setValue("loopstation/colorStop", loopstationColorStop);
}

MidiLearnSetting DrumMachineSettings::loopstationSample(quint8 pad) const
{
    return learnSetting(QString{"loopstation/pad%0"}.arg(pad));
}

void DrumMachineSettings::setLoopstationSample(quint8 pad, const MidiLearnSetting &value)
{
    setLearnSetting(QString{"loopstation/pad%0"}.arg(pad), value);
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
    setValue(QString{"%0_cmd"}.arg(key), std::to_underlying(value.cmd));
    setValue(QString{"%0_channel"}.arg(key), value.channel);
    setValue(QString{"%0_note"}.arg(key), value.note);
}
