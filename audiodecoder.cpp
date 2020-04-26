#include "audiodecoder.h"

#include <QAudioBuffer>
#include <QMetaType>
#include <QCoreApplication>
#include <QDebug>

#include "audioformat.h"

AudioDecoder::AudioDecoder(QObject *parent) :
    QObject(parent)
{
    QObject::connect(&m_decoder, qOverload<QAudioDecoder::Error>(&QAudioDecoder::error),
                     this, &AudioDecoder::error);
    QObject::connect(&m_decoder, &QAudioDecoder::finished,
                     this, &AudioDecoder::finished);
    QObject::connect(&m_decoder, &QAudioDecoder::bufferReady,
                     this, &AudioDecoder::bufferReady);
    QObject::connect(&m_decoder, &QAudioDecoder::durationChanged,
                     this, &AudioDecoder::durationChanged);

    m_decoder.setAudioFormat(audioFormat());
}

void AudioDecoder::startDecoding(std::shared_ptr<QIODevice> device)
{
    qDebug() << "called" << device.get();

    if (m_decoder.state() == QAudioDecoder::DecodingState)
        m_decoder.stop();

    m_decoder.setSourceDevice(device.get());
    m_device = device;
    m_bytearray.clear();
    m_decoder.start();
}

void AudioDecoder::error(const QAudioDecoder::Error error)
{
    qDebug() << "error:" << error << m_decoder.errorString() << m_decoder.sourceFilename();
}

void AudioDecoder::finished()
{
    qDebug() << "called";
    emit decodingFinished(QAudioBuffer{std::move(m_bytearray), audioFormat()});
}

void AudioDecoder::bufferReady()
{
    const auto &format = m_decoder.audioFormat();
    const auto buffer = m_decoder.read();

    Q_ASSERT(buffer.byteCount() == buffer.sampleCount() * (format.sampleSize()/8));

    m_bytearray.append(buffer.constData<char>(), buffer.byteCount());

    const auto now = QDateTime::currentDateTime();
    if (m_lastProgressUpdate.isNull() || m_lastProgressUpdate.msecsTo(now) > 100)
    {
        emit progress(m_decoder.position(), m_decoder.duration());
        m_lastProgressUpdate = now;
    }
}

void AudioDecoder::durationChanged(const qint64 duration)
{
    if (duration == -1)
        return;

    const auto &format = m_decoder.audioFormat();
    const auto reserve = (format.sampleSize()/8) * format.sampleRate() * format.channelCount() * (duration + 1000) / 1000;
    m_bytearray.reserve(reserve);
    qDebug() << "duration:" << duration << reserve;
}

namespace {
void registerMetaType()
{
    qRegisterMetaType<std::shared_ptr<QIODevice>>();
}
Q_COREAPP_STARTUP_FUNCTION(registerMetaType)
}
