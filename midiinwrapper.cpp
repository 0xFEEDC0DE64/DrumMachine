#include "midiinwrapper.h"

#include <QCoreApplication>
#include <QMetaType>
#include <QDebug>

MidiInWrapper::MidiInWrapper(RtMidi::Api api, const QString& clientName, unsigned int queueSizeLimit, QObject *parent) :
    QObject{parent},
    midiIn{api, clientName.toStdString(), queueSizeLimit}
{
    midiIn.ignoreTypes(false, false, false);
    midiIn.setCallback(&mycallback, this);
}

void MidiInWrapper::openPort(unsigned int portNumber)
{
    midiIn.openPort(portNumber);
}

void MidiInWrapper::openVirtualPort(const QString &portName)
{
    midiIn.openVirtualPort(portName.toStdString());
}

void MidiInWrapper::closePort()
{
    midiIn.closePort();
}

bool MidiInWrapper::isPortOpen() const
{
    return midiIn.isPortOpen();
}

QStringList MidiInWrapper::portNames()
{
    QStringList names;

    const auto count = midiIn.getPortCount();

    for (unsigned int i = 0; i < count; i++)
        names.append(QString::fromStdString(midiIn.getPortName(i)));

    return names;
}

void MidiInWrapper::mycallback(double deltatime, std::vector<unsigned char> *message, void *userData)
{
    Q_UNUSED(deltatime)

    if (!userData)
    {
        qCritical() << "called without userData pointer to wrapper";
        return;
    }

    auto wrapper = static_cast<MidiInWrapper*>(userData);

    if (!message)
    {
        qCritical() << "called without message pointer";
        return;
    }

    if (message->size() < sizeof(midi::MidiMessage))
    {
        qCritical() << "called with message that is shorter than 3 bytes";
        return;
    }

    const midi::MidiMessage &midiMessage = reinterpret_cast<const midi::MidiMessage &>(message->at(0));
    wrapper->messageReceived(midiMessage);
}
