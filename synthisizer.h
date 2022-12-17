#pragma once

#include "audioformat.h"

namespace midi { class MidiMessage; }

class DrumMachineSettings;

class Synthisizer
{
public:
    void loadSettings(const DrumMachineSettings &settings);

    void setFrequency(int16_t frequency) { m_frequency = frequency; }

    void writeSamples(frame_t *begin, frame_t *end);

    void messageReceived(const midi::MidiMessage &message);

private:
    int16_t m_frequency{};
    int16_t m_actualFrequency{};
    double m_phase{};
};
