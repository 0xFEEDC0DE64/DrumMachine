#include "synthisizerwidget.h"
#include "ui_synthisizerwidget.h"

#include <cmath>

#include "midicontainers.h"

SynthisizerWidget::SynthisizerWidget(QWidget *parent) :
    QWidget{parent},
    m_ui{std::make_unique<Ui::SynthisizerWidget>()}
{
    m_ui->setupUi(this);

    connect(m_ui->horizontalSliderVolume, &QSlider::valueChanged,
            &m_synthisizer, [&synthisizer=m_synthisizer](int value){ synthisizer.setVolume(float(value) / 100.f); });
}

SynthisizerWidget::~SynthisizerWidget() = default;

void SynthisizerWidget::writeSamples(frame_t *begin, frame_t *end)
{
    m_synthisizer.writeSamples(begin, end);
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
    if (message.cmd == midi::Command::NoteOff || (message.cmd == midi::Command::NoteOn && message.velocity == 0))
    {
        if (m_synthisizer.frequency() == int16_t(440.*std::pow(std::pow(2., 1./12.), message.note-48)))
            m_synthisizer.setFrequency(0);
    }
    else if (message.cmd == midi::Command::NoteOn)
        m_synthisizer.setFrequency(440.*std::pow(std::pow(2., 1./12.), message.note-48));
}
