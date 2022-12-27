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

    QString drumpadLastPresetId() const;
    void setDrumpadLastPresetId(const QString &lastPresetId);

    quint8 drumpadChannelPrevPreset() const;
    void setDrumpadChannelPrevPreset(quint8 channel);
    quint8 drumpadNotePrevPreset() const;
    void setDrumpadNotePrevPreset(quint8 note);

    quint8 drumpadChannelNextPreset() const;
    void setDrumpadChannelNextPreset(quint8 channel);
    quint8 drumpadNoteNextPreset() const;
    void setDrumpadNoteNextPreset(quint8 note);

    quint8 drumpadChannelPrevSequence() const;
    void setDrumpadChannelPrevSequence(quint8 channel);
    quint8 drumpadNotePrevSequence() const;
    void setDrumpadNotePrevSequence(quint8 note);

    quint8 drumpadChannelNextSequence() const;
    void setDrumpadChannelNextSequence(quint8 channel);
    quint8 drumpadNoteNextSequence() const;
    void setDrumpadNoteNextSequence(quint8 note);

    quint8 drumpadChannelPlayPause() const;
    void setDrumpadChannelPlayPause(quint8 channel);
    quint8 drumpadNotePlayPause() const;
    void setDrumpadNotePlayPause(quint8 note);

    quint8 drumpadChannelStop() const;
    void setDrumpadChannelStop(quint8 channel);
    quint8 drumpadNoteStop() const;
    void setDrumpadNoteStop(quint8 note);

    quint8 drumpadChannel(quint8 pad) const;
    void setDrumpadChannel(quint8 pad, quint8 channel);
    quint8 drumpadNote(quint8 pad) const;
    void setDrumpadNote(quint8 pad, quint8 note);
};
