#include "midiinwrapper.h"

#include <QCoreApplication>
#include <QMetaType>
#include <QDebug>
#include <QDateTime>

#include <chrono>

#include "midicontainers.h"

MidiInWrapper::MidiInWrapper(RtMidi::Api api, const QString& clientName, unsigned int queueSizeLimit, QObject *parent) :
    QObject{parent},
    midiIn{api, clientName.toStdString(), queueSizeLimit}
{
    midiIn.ignoreTypes(false, false, false);
    midiIn.setCallback(&mycallback, this);
}

void MidiInWrapper::openPort(unsigned int portNumber, const QString &portName)
{
    qDebug() << "opening" << QString::fromStdString(midiIn.getPortName(portNumber));
    midiIn.openPort(portNumber, portName.toStdString());
    if (!midiIn.isPortOpen())
        qFatal("port did not open");
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

    if (message->size() == 1)
    {
        if (const auto opcode = message->at(0); opcode == 250)
        {
            wrapper->m_counting = true;
            wrapper->m_timerCounter = 0;
        }
        else if (opcode == 252)
        {
            wrapper->m_counting = false;
        }
        else if (opcode == 248)
        {
            if (wrapper->m_counting)
            {
                if (wrapper->m_timerCounter == 0 || wrapper->m_timerCounter == 12)
                    wrapper->onQuarterNote();
                wrapper->m_timerCounter++;
                if (wrapper->m_timerCounter >= 24)
                    wrapper->m_timerCounter = 0;
            }
        }
        else
            qWarning() << "unknown opcode" << opcode;

        return;
    }

    if (message->size() < sizeof(midi::MidiMessage))
    {
        qCritical() << "called with message that is shorter than 3 bytes (" << message->size() << ')';
        return;
    }

    const midi::MidiMessage &midiMessage = reinterpret_cast<const midi::MidiMessage &>(message->at(0));
    wrapper->midiReceived(midiMessage);
}
