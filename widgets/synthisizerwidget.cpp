#include "synthisizerwidget.h"
#include "ui_synthisizerwidget.h"

#include <cmath>

#include <QDebug>
#include <QTimer>

#include "audioformat.h"
#include "midicontainers.h"

SynthisizerWidget::SynthisizerWidget(QWidget *parent) :
    QWidget{parent},
    m_ui{std::make_unique<Ui::SynthisizerWidget>()}
{
    m_ui->setupUi(this);

    for (Synthisizer &synthisizer : m_synthisizers)
        connect(m_ui->horizontalSliderVolume, &QSlider::valueChanged,
                &synthisizer, [&synthisizer](int value){ synthisizer.setVolume(float(value) / 100.f); });

//    m_synthisizers.at(8).setFrequency(1209);
//    m_synthisizers.at(9).setFrequency(697);
}

SynthisizerWidget::~SynthisizerWidget() = default;

void SynthisizerWidget::writeSamples(frame_t *begin, frame_t *end)
{
    for (Synthisizer &synthisizer : m_synthisizers)
        synthisizer.writeSamples(begin, end);
}

void SynthisizerWidget::loadSettings(DrumMachineSettings &settings)
{
    Q_UNUSED(settings)
}

void SynthisizerWidget::unsendColors()
{
}

void SynthisizerWidget::sendColors()
{
}

void SynthisizerWidget::midiReceived(const midi::MidiMessage &message)
{
    auto freq = 440.*std::pow(std::pow(2., 1./12.), message.note-48);
    if (message.cmd == midi::Command::NoteOff || (message.cmd == midi::Command::NoteOn && message.velocity == 0))
    {
        for (Synthisizer &synthisizer : m_synthisizers)
            if (synthisizer.frequency() == int16_t(freq))
                synthisizer.setFrequency(0);
    }
    else if (message.cmd == midi::Command::NoteOn)
    {
        for (Synthisizer &synthisizer : m_synthisizers)
            if (!synthisizer.frequency())
            {
                synthisizer.setFrequency(freq);
                break;
            }
    }
}

void SynthisizerWidget::onQuarterNote()
{
    auto &synthisizer = m_synthisizers.back();
    synthisizer.setFrequency(440.);
    QTimer::singleShot(6, &synthisizer, [&synthisizer](){ synthisizer.setFrequency(0.); });
}
