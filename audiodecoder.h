#pragma once

#include <memory>

#include <QObject>
#include <QAudioDecoder>
#include <QByteArray>
#include <QDateTime>

class QAudioBuffer;
class QIODevice;

class AudioDecoder : public QObject
{
    Q_OBJECT

public:
    explicit AudioDecoder(QObject *parent = nullptr);

signals:
    void progress(int progress, int total);
    void decodingFinished(const QAudioBuffer &buffer);

public slots:
    void startDecodingFilename(const QString &filename);
    void startDecodingDevice(std::shared_ptr<QIODevice> device);

private slots:
    void error(const QAudioDecoder::Error error);
    void finished();
    void bufferReady();
    void durationChanged(const qint64 duration);

private:
    std::shared_ptr<QIODevice> m_device;
    QAudioDecoder m_decoder;
    QByteArray m_bytearray;
    QDateTime m_lastProgressUpdate;
};

Q_DECLARE_METATYPE(std::shared_ptr<QIODevice>)
