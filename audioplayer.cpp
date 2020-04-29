#include "audioplayer.h"

#include <algorithm>
#include <cmath>

AudioPlayer::AudioPlayer(QObject *parent) :
    QObject{parent}
{
}

AudioPlayer::AudioPlayer(const QAudioBuffer &buffer, QObject *parent) :
    QObject{parent},
    m_buffer{buffer}
{
}

void AudioPlayer::writeSamples(frame_t *begin, frame_t *end)
{
    if (!m_playing)
        return;

    // thread safe copies
    auto position = m_position;
    const auto speed = m_speed;
    const auto buffer = m_buffer;
    const auto volume = m_volume;
    const auto stopOnEnd = m_stopOnEnd;
    const auto &data = buffer.constData<frame_t>();
    const auto loop = m_loop;

    const auto frames = std::min<size_t>(std::distance(begin, end), buffer.frameCount()-position);

    if (!frames)
    {
        m_playing = false;
        emit playingChanged(false);
        return;
    }

    bool ended{};
    std::transform(static_cast<const frame_t *>(begin), static_cast<const frame_t *>(begin+frames), begin,
                   [&](frame_t frame)->frame_t{
        if (ended && stopOnEnd)
            return frame;

        const auto index = std::ptrdiff_t(position);
        position += speed;

        if (loop)
        {
            if (speed < 0 && position < loop->first)
                position = loop->second;
            else if (speed > 0 && position > loop->second)
                position = loop->first;
        }

        if ((speed < 0.f && index < 0) || (speed > 0.f && index >= buffer.frameCount()))
        {
            ended = true;
            return frame;
        }

        const frame_t &frame2 = data[index];

        std::transform(std::cbegin(frame), std::cend(frame), std::begin(frame2), std::begin(frame),
                       [&volume](const sample_t &left, const sample_t &right) { return left + (right*volume); });

        return frame;
    });

    m_position = position;
    const auto now = QDateTime::currentDateTime();
    if (m_lastPositionUpdate.isNull() || m_lastPositionUpdate.msecsTo(now) > 1000/30)
    {
        emit positionChanged(m_position);
        m_lastPositionUpdate = now;
    }

    if (ended && stopOnEnd)
    {
        m_playing = false;
        emit playingChanged(m_playing);
    }
}

void AudioPlayer::setPlaying(bool playing)
{
    m_playing = playing;
    emit playingChanged(playing);
}

void AudioPlayer::setPosition(double position)
{
    m_position = position;
    emit positionChanged(position);
}

void AudioPlayer::setSpeed(float speed)
{
    m_speed = speed;
    emit speedChanged(speed);
}

void AudioPlayer::setVolume(float volume)
{
    m_volume = volume;
    emit volumeChanged(volume);
}

void AudioPlayer::setBuffer(const QAudioBuffer &buffer)
{
    stop();
    emit bufferChanged(m_buffer = buffer);
}

void AudioPlayer::togglePlaying()
{
    emit playingChanged(m_playing = !m_playing);
}

void AudioPlayer::restart()
{
    setPosition(0.);
    setPlaying(true);
}

void AudioPlayer::stop()
{
    setPosition(0.);
    setPlaying(false);
}
