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

    unsigned int framesPerBuffer() const;
    void setFramesPerBuffer(unsigned int framesPerBuffer);

    QString lastMidiInDevice() const;
    void setLastMidiInDevice(const QString &lastMidiInDevice);

    QString lastMidiOutDevice() const;
    void setLastMidiOutDevice(const QString &lastMidiOutDevice);

    MidiLearnSetting tabWidget(quint8 index) const;
    void setTabWidget(quint8 index, const MidiLearnSetting &value);



    QString drumpadLastPresetId() const;
    void setDrumpadLastPresetId(const QString &lastPresetId);

    MidiLearnSetting drumpadPrevPreset() const;
    void setDrumpadPrevPreset(const MidiLearnSetting &value);

    MidiLearnSetting drumpadNextPreset() const;
    void setDrumpadNextPreset(const MidiLearnSetting &value);

    MidiLearnSetting drumpadPrevSequence() const;
    void setDrumpadPrevSequence(const MidiLearnSetting &value);

    MidiLearnSetting drumpadNextSequence() const;
    void setDrumpadNextSequence(const MidiLearnSetting &value);

    MidiLearnSetting drumpadPlayPause() const;
    void setDrumpadPlayPause(const MidiLearnSetting &value);

    MidiLearnSetting drumpadStop() const;
    void setDrumpadStop(const MidiLearnSetting &value);

    MidiLearnSetting drumpadSample(quint8 pad) const;
    void setDrumpadSample(quint8 pad, const MidiLearnSetting &value);



    QString loopstationLastPresetId() const;
    void setLoopstationLastPresetId(const QString &lastPresetId);

    MidiLearnSetting loopstationPrevPreset() const;
    void setLoopstationPrevPreset(const MidiLearnSetting &value);

    MidiLearnSetting loopstationNextPreset() const;
    void setLoopstationNextPreset(const MidiLearnSetting &value);

    MidiLearnSetting loopstationSample(quint8 pad) const;
    void setLoopstationSample(quint8 pad, const MidiLearnSetting &value);

private:
    MidiLearnSetting learnSetting(const QString &key) const;
    void setLearnSetting(const QString &key, const MidiLearnSetting &value);
};
