#pragma once

#include <atomic>

#include <QObject>
#include <QAudioBuffer>
#include <QtGlobal>
#include <QDateTime>

#include "audioformat.h"

class AudioPlayer : public QObject
{
    Q_OBJECT

public:
    explicit AudioPlayer(QObject *parent = nullptr);
    explicit AudioPlayer(const QAudioBuffer &buffer, QObject *parent = nullptr);

    void writeSamples(frame_t *begin, frame_t *end);

    bool playing() const { return m_playing; }
    void setPlaying(bool playing) { m_playing = playing; emit playingChanged(playing); }

    std::size_t position() const { return m_position; }
    void setPosition(std::size_t position) { m_position = position; emit positionChanged(position); }

    const QAudioBuffer &buffer() const { return m_buffer; }
    void setBuffer(const QAudioBuffer &buffer) { emit bufferChanged(m_buffer = buffer); }

    void restart();
    void stop();

signals:
    void playingChanged(bool playing);
    void positionChanged(std::size_t position);
    void bufferChanged(const QAudioBuffer &buffer);

private:
    bool m_playing{false};
    std::size_t m_position{};
    QAudioBuffer m_buffer;

    QDateTime m_lastPositionUpdate;
};
