#pragma once

#include <QObject>
#include <QString>

#include "rtmidi/RtMidi.h"

#include "midicontainers.h"

class MidiInWrapper : public QObject
{
    Q_OBJECT

public:
    MidiInWrapper(RtMidi::Api api = RtMidi::UNSPECIFIED,
                const QString &clientName = "RtMidi Input Client",
                unsigned int queueSizeLimit = 100,
                QObject *parent = nullptr);

    void openPort(unsigned int portNumber);
    void openVirtualPort(const QString &portName);
    void closePort();
    bool isPortOpen() const;

    QStringList portNames();

signals:
    void messageReceived(const midi::MidiMessage &message);

private:
    static void mycallback(double deltatime, std::vector<unsigned char> *message, void *userData);

    RtMidiIn midiIn;
};
