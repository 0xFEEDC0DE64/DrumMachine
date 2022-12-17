#pragma once

#include "rtmidi/RtMidi.h"

#include "midicontainers.h"

class MidiOutWrapper
{
public:
    MidiOutWrapper(RtMidi::Api api = RtMidi::UNSPECIFIED,
                   const QString &clientName = "RtMidi Input Client");

    void openPort(unsigned int portNumber, const QString &portName);
    void openVirtualPort(const QString &portName);
    void closePort();
    bool isPortOpen() const;

    QStringList portNames();

    void sendMessage(const midi::MidiMessage &midiMsg);

private:
    RtMidiOut midiOut;
};
