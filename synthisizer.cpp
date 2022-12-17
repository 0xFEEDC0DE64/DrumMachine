#include "synthisizer.h"

#include <cmath>

#include "midicontainers.h"

constexpr double pi = std::acos(-1);

void Synthisizer::writeSamples(frame_t *begin, frame_t *end)
{
    const auto frequency = m_frequency;
    if (frequency)
        std::transform(begin, end, begin, [&](frame_t frame){
            std::transform(std::cbegin(frame), std::cend(frame), std::begin(frame),
                           [value=std::sin(m_phase)](const sample_t &sample) { return sample + value; });

            m_phase += pi*2./frameRate*m_actualFrequency;
            if (m_phase >= pi*2.)
                m_phase -= pi*2.;

            return frame;
        });
    m_actualFrequency = float(m_actualFrequency+m_frequency)/2.f;
}

void Synthisizer::messageReceived(const midi::MidiMessage &message)
{
    if (message.cmd == midi::Command::NoteOff || (message.cmd == midi::Command::NoteOn && message.velocity == 0))
    {
        if (m_frequency == int16_t(440.*std::pow(std::pow(2., 1./12.), message.note-48)))
            m_frequency = 0;
    }
    else if (message.cmd == midi::Command::NoteOn)
        m_frequency = 440.*std::pow(std::pow(2., 1./12.), message.note-48);
}
