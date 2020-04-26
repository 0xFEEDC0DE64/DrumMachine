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

    const auto frames = std::min<size_t>(std::distance(begin, end), m_buffer.frameCount()-m_position);

    if (!frames)
    {
        m_playing = false;
        emit playingChanged(false);
        return;
    }

    std::transform(static_cast<const frame_t *>(begin), static_cast<const frame_t *>(begin+frames), m_buffer.constData<frame_t>() + m_position, begin,
                   [](const frame_t &frame, const frame_t &frame2)->frame_t{
        frame_t newFrame;
        std::transform(std::begin(frame), std::end(frame), std::begin(frame2), std::begin(newFrame),
                       [](const sample_t &left, const sample_t &right) { return left + right; });
        return newFrame;
    });

    m_position += frames;
}

void AudioPlayer::restart()
{
    setPosition(0);
    setPlaying(true);
}

void AudioPlayer::stop()
{
    setPosition(0);
    setPlaying(false);
}
