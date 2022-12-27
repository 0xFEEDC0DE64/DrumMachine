#include "synthisizer.h"

#include <cmath>

constexpr double pi = std::acos(-1);

void Synthisizer::writeSamples(frame_t *begin, frame_t *end)
{
    const auto volume = m_volume;
    const auto frequency = m_frequency;

    if (frequency)
        std::transform(begin, end, begin, [&](frame_t frame){
            std::transform(std::cbegin(frame), std::cend(frame), std::begin(frame),
                           [value=std::sin(m_phase),&volume](const sample_t &sample) { return (sample + value) * volume; });

            m_phase += pi*2./frameRate*m_actualFrequency;
            if (m_phase >= pi*2.)
                m_phase -= pi*2.;

            return frame;
        });
    m_actualFrequency = float(m_actualFrequency+m_frequency)/2.f;
}
