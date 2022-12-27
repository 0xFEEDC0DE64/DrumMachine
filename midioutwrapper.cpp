#include "midioutwrapper.h"

#include <QDebug>

#include "midicontainers.h"

MidiOutWrapper::MidiOutWrapper(RtMidi::Api api, const QString &clientName) :
    midiOut{api, clientName.toStdString()}
{
}

void MidiOutWrapper::openPort(unsigned int portNumber, const QString &portName)
{
    qDebug() << "opening" << QString::fromStdString(midiOut.getPortName(portNumber));
    midiOut.openPort(portNumber, portName.toStdString());
    if (!midiOut.isPortOpen())
        qFatal("port did not open");
}

void MidiOutWrapper::openVirtualPort(const QString &portName)
{
    midiOut.openVirtualPort(portName.toStdString());
}

void MidiOutWrapper::closePort()
{
    midiOut.closePort();
}

bool MidiOutWrapper::isPortOpen() const
{
    return midiOut.isPortOpen();
}

QStringList MidiOutWrapper::portNames()
{
    QStringList names;

    const auto count = midiOut.getPortCount();

    for (unsigned int i = 0; i < count; i++)
        names.append(QString::fromStdString(midiOut.getPortName(i)));

    return names;
}

void MidiOutWrapper::sendMessage(const midi::MidiMessage &midiMsg)
{
    union Helper {
        Helper() {}

        midi::MidiMessage midiMsg;
        unsigned char buf[sizeof(midiMsg)];
    };

    Helper helper;
    helper.midiMsg = midiMsg;

    midiOut.sendMessage(helper.buf, sizeof(helper.buf));
}
