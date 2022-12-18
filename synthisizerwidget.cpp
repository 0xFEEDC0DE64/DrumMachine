#include "synthisizerwidget.h"
#include "ui_synthisizerwidget.h"

SynthisizerWidget::SynthisizerWidget(QWidget *parent) :
    QWidget{parent},
    m_ui{std::make_unique<Ui::SynthisizerWidget>()}
{
    m_ui->setupUi(this);
}

SynthisizerWidget::~SynthisizerWidget() = default;

void SynthisizerWidget::writeSamples(frame_t *begin, frame_t *end)
{
    m_synthisizer.writeSamples(begin, end);
}

void SynthisizerWidget::loadSettings(DrumMachineSettings &settings)
{
}

void SynthisizerWidget::unsendColors()
{
}

void SynthisizerWidget::sendColors()
{
}

void SynthisizerWidget::messageReceived(const midi::MidiMessage &message)
{
    m_synthisizer.messageReceived(message);
}
