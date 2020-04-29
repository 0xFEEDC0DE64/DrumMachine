#pragma once

#include <atomic>
#include <optional>

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

    bool stopOnEnd() { return m_stopOnEnd; }
    void setStopOnEnd(bool stopOnEnd) { m_stopOnEnd = stopOnEnd; emit stopOnEndChanged(m_stopOnEnd); }

    const QAudioBuffer &buffer() const { return m_buffer; }
    void setBuffer(const QAudioBuffer &buffer);

    const std::optional<std::pair<double, double>> &loop() const { return m_loop; }
    void setLoop(const std::optional<std::pair<double, double>> &loop) { m_loop = loop; }

    void togglePlaying();
    void restart();
    void stop();

signals:
    void playingChanged(bool playing);
    void positionChanged(double position);
    void speedChanged(float speed);
    void volumeChanged(float volume);
    void stopOnEndChanged(bool stopOnEnd);
    void bufferChanged(const QAudioBuffer &buffer);

private:
    bool m_playing{false};
    double m_position{};
    float m_speed{1.f};
    float m_volume{1.f};
    bool m_stopOnEnd{true};
    QAudioBuffer m_buffer;
    std::optional<std::pair<double, double>> m_loop;

    QDateTime m_lastPositionUpdate;
};
