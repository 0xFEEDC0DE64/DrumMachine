#pragma once

#include <QObject>

struct frame_t;

class DrumMachineSettings;

class Synthisizer : public QObject
{
public:
    float volume() const { return m_volume; }
    void setVolume(float volume) { m_volume = volume; }

    int16_t frequency() const { return m_frequency; }
    void setFrequency(int16_t frequency) { m_frequency = frequency; }

    void writeSamples(frame_t *begin, frame_t *end);

private:
    float m_volume{1.f};
    int16_t m_frequency{};
    int16_t m_actualFrequency{};
    double m_phase{};
};
