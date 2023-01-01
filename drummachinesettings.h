#pragma once

#include <QSettings>

struct MidiLearnSetting;

class DrumMachineSettings : public QSettings
{
public:
    using QSettings::QSettings;

    QString defaultCacheDir() const;
    QString cacheDir() const;
    void setCacheDir(const QString &cacheDir);

    qint64 defaultMaximumCacheSize() const;
    qint64 maximumCacheSize() const;
    void setMaximumCacheSize(qint64 maximumCacheSize);

    QString lastAudioDevice() const;
    void setLastAudioDevice(const QString &lastAudioDevice);

    bool autoOpenAudioDevice() const;
    void setAutoOpenAudioDevice(bool autoOpenAudioDevice);

    unsigned int framesPerBuffer() const;
    void setFramesPerBuffer(unsigned int framesPerBuffer);

    QString lastMidiInDevice() const;
    void setLastMidiInDevice(const QString &lastMidiInDevice);

    bool autoOpenMidiIn() const;
    void setAutoOpenMidiIn(bool autoOpenMidiIn);

    QString lastMidiOutDevice() const;
    void setLastMidiOutDevice(const QString &lastMidiOutDevice);

    bool autoOpenMidiOut() const;
    void setAutoOpenMidiOut(bool autoOpenMidiOut);

    quint8 colorOff() const;
    quint8 defaultColorOff() const;
    void setColorOff(quint8 colorOff);

    MidiLearnSetting tabWidget(quint8 index) const;
    void setTabWidget(quint8 index, const MidiLearnSetting &value);

    quint8 colorTabWidget() const;
    quint8 defaultColorTabWidget() const;
    void setColorTabWidget(quint8 colorTabWidget);



    QString drumpadLastPresetId() const;
    void setDrumpadLastPresetId(const QString &lastPresetId);

    MidiLearnSetting drumpadPrevPreset() const;
    void setDrumpadPrevPreset(const MidiLearnSetting &value);

    quint8 drumpadColorPrevPreset() const;
    quint8 defaultDrumpadColorPrevPreset() const;
    void setDrumpadColorPrevPreset(quint8 drumpadColorPrevPreset);

    MidiLearnSetting drumpadNextPreset() const;
    void setDrumpadNextPreset(const MidiLearnSetting &value);

    quint8 drumpadColorNextPreset() const;
    quint8 defaultDrumpadColorNextPreset() const;
    void setDrumpadColorNextPreset(quint8 drumpadColorNextPreset);

    MidiLearnSetting drumpadPrevSequence() const;
    void setDrumpadPrevSequence(const MidiLearnSetting &value);

    quint8 drumpadColorPrevSequence() const;
    quint8 defaultDrumpadColorPrevSequence() const;
    void setDrumpadColorPrevSequence(quint8 drumpadColorPrevSequence);

    MidiLearnSetting drumpadNextSequence() const;
    void setDrumpadNextSequence(const MidiLearnSetting &value);

    quint8 drumpadColorNextSequence() const;
    quint8 defaultDrumpadColorNextSequence() const;
    void setDrumpadColorNextSequence(quint8 drumpadColorNextSequence);

    MidiLearnSetting drumpadPlayPause() const;
    void setDrumpadPlayPause(const MidiLearnSetting &value);

    quint8 drumpadColorPlayPause() const;
    quint8 defaultDrumpadColorPlayPause() const;
    void setDrumpadColorPlayPause(quint8 drumpadColorPlayPause);

    MidiLearnSetting drumpadStopSequence() const;
    void setDrumpadStopSequence(const MidiLearnSetting &value);

    quint8 drumpadColorStopSequence() const;
    quint8 defaultDrumpadColorStopSequence() const;
    void setDrumpadColorStopSequence(quint8 drumpadColorStopSequence);

    MidiLearnSetting drumpadSwap() const;
    void setDrumpadSwap(const MidiLearnSetting &value);

    quint8 drumpadColorSwap() const;
    quint8 defaultDrumpadColorSwap() const;
    void setDrumpadColorSwap(quint8 drumpadColorSwap);

    MidiLearnSetting drumpadStopAll() const;
    void setDrumpadStopAll(const MidiLearnSetting &value);

    quint8 drumpadColorStopAll() const;
    quint8 defaultDrumpadColorStopAll() const;
    void setDrumpadColorStopAll(quint8 drumpadColorStopAll);

    MidiLearnSetting drumpadSample(quint8 pad) const;
    void setDrumpadSample(quint8 pad, const MidiLearnSetting &value);



    QString loopstationLastPresetId() const;
    void setLoopstationLastPresetId(const QString &lastPresetId);

    MidiLearnSetting loopstationPrevPreset() const;
    void setLoopstationPrevPreset(const MidiLearnSetting &value);

    MidiLearnSetting loopstationNextPreset() const;
    void setLoopstationNextPreset(const MidiLearnSetting &value);

    MidiLearnSetting loopstationPlayPause() const;
    void setLoopstationPlayPause(const MidiLearnSetting &value);

    MidiLearnSetting loopstationStop() const;
    void setLoopstationStop(const MidiLearnSetting &value);

    MidiLearnSetting loopstationSample(quint8 pad) const;
    void setLoopstationSample(quint8 pad, const MidiLearnSetting &value);

private:
    MidiLearnSetting learnSetting(const QString &key) const;
    void setLearnSetting(const QString &key, const MidiLearnSetting &value);
};
