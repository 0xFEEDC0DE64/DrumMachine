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
    void setPlaying(bool playing);

    double position() const { return m_position; }
    void setPosition(double position);

    float speed() const { return m_speed; }
    void setSpeed(float speed);

    float volume() const { return m_volume; }
    void setVolume(float volume);

    const QAudioBuffer &buffer() const { return m_buffer; }
    void setBuffer(const QAudioBuffer &buffer);

    void togglePlaying();
    void restart();
    void stop();

signals:
    void playingChanged(bool playing);
    void positionChanged(double position);
    void speedChanged(float speed);
    void volumeChanged(float volume);
    void bufferChanged(const QAudioBuffer &buffer);

private:
    bool m_playing{false};
    double m_position{};
    float m_speed{1.f};
    float m_volume{1.f};
    QAudioBuffer m_buffer;

    QDateTime m_lastPositionUpdate;
};
