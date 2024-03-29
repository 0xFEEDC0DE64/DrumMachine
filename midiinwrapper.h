#pragma once

#include <QObject>
#include <QString>

#include "rtmidi/RtMidi.h"

namespace midi { struct MidiMessage; }

class MidiInWrapper : public QObject
{
    Q_OBJECT

public:
    MidiInWrapper(RtMidi::Api api = RtMidi::UNSPECIFIED,
                  const QString &clientName = "RtMidi Input Client",
                  unsigned int queueSizeLimit = 100,
                  QObject *parent = nullptr);

    void openPort(unsigned int portNumber, const QString &portName);
    void openVirtualPort(const QString &portName);
    void closePort();
    bool isPortOpen() const;

    QStringList portNames();

signals:
    void midiReceived(const midi::MidiMessage &message);
    void onQuarterNote();

private:
    static void mycallback(double deltatime, std::vector<unsigned char> *message, void *userData);

    RtMidiIn midiIn;

    int m_timerCounter{};
    bool m_counting{true};
};
