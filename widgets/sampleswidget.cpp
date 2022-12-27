#include "sampleswidget.h"
#include "ui_sampleswidget.h"

#include <iterator>

#include <QDebug>

#include "audioformat.h"
#include "midicontainers.h"

SamplesWidget::SamplesWidget(QWidget *parent) :
    QWidget{parent},
    m_ui{std::make_unique<Ui::SamplesWidget>()}
{
    m_ui->setupUi(this);

    connect(m_ui->checkBox, &QCheckBox::toggled, this, &SamplesWidget::updateWidgets);

    connect(m_ui->pushButtonStopAll, &QAbstractButton::pressed, this, &SamplesWidget::stopAll);

    quint8 padNr{};
    for (SampleWidget &widget : getWidgets())
    {
        widget.setPadNr(padNr++);
        connect(&widget, &SampleWidget::chokeTriggered, this, &SamplesWidget::chokeTriggered);
        connect(&widget, &SampleWidget::sendMidi, this, &SamplesWidget::sendMidi);
    }
}

SamplesWidget::~SamplesWidget() = default;

void SamplesWidget::loadSettings(DrumMachineSettings &settings)
{
    for (SampleWidget &widget : getWidgets())
        widget.loadSettings(settings);
}

void SamplesWidget::setPreset(const drumpad_presets::Preset &preset)
{
    m_preset = preset;

    updateWidgets();
}

void SamplesWidget::midiReceived(const midi::MidiMessage &message)
{
    if (message == midi::MidiMessage{.channel=0,.cmd=midi::Command::ControlChange,.flag=true,.note=64,.velocity=127})
    {
        m_ui->checkBox->toggle();
        return;
    }

    if (message.cmd != midi::Command::NoteOn && message.cmd != midi::Command::NoteOff)
        return;

    for (SampleWidget &widget : getWidgets())
    {
        if (widget.isLearning())
        {
            widget.learn(message.channel, message.note);
        }
        else if (widget.channel() == message.channel && widget.note() == message.note)
        {
            if (message.cmd == midi::Command::NoteOff || (message.cmd == midi::Command::NoteOn && message.velocity == 0))
                widget.released();
            else if (message.cmd == midi::Command::NoteOn)
                widget.pressed(message.velocity);
        }
    }
}

void SamplesWidget::writeSamples(frame_t *begin, frame_t *end)
{
    for (SampleWidget &widget : getWidgets())
        widget.writeSamples(begin, end);
}

void SamplesWidget::injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager)
{
    for (SampleWidget &widget : getWidgets())
        widget.injectNetworkAccessManager(networkAccessManager);
}

void SamplesWidget::injectDecodingThread(QThread &thread)
{
    for (SampleWidget &widget : getWidgets())
        widget.injectDecodingThread(thread);
}

void SamplesWidget::unsendColors()
{
    for (SampleWidget &widget : getWidgets())
        widget.unsendColor();
}

void SamplesWidget::sendColors()
{
    for (SampleWidget &widget : getWidgets())
        widget.sendColor();
}

void SamplesWidget::sequencerTriggerSample(int index)
{
    const auto widgets = getWidgets();
    if (index < 0 || index >= std::size(widgets))
    {
        qDebug() << "index out of range" << index;
        return;
    }
    widgets[index].get().pressed(127);
}

void SamplesWidget::chokeTriggered(int choke)
{
    for (SampleWidget &widget : getWidgets())
    {
        if (&widget == sender())
            continue;

        if (widget.choke() && *widget.choke() && *widget.choke() == choke)
            widget.forceStop();
    }
}

void SamplesWidget::updateWidgets()
{
    const auto widgets = getWidgets();

    auto files = *m_preset.files;

    if (m_ui->checkBox->isChecked())
        for (int i = 0; i < 12; i++)
            std::swap(files[i], files[i+12]);

    auto filesIter = std::cbegin(files);
    auto widgetsIter = std::cbegin(widgets);

    for (; filesIter != std::cend(files) && widgetsIter != std::cend(widgets); std::advance(filesIter, 1), std::advance(widgetsIter, 1))
        widgetsIter->get().setFile(*m_preset.id, *filesIter);
}

void SamplesWidget::stopAll()
{
    for (SampleWidget &widget : getWidgets())
        widget.forceStop();
}

std::array<std::reference_wrapper<SampleWidget>, 24> SamplesWidget::getWidgets()
{
    return {
        std::ref(*m_ui->sampleWidget_1),
        std::ref(*m_ui->sampleWidget_2),
        std::ref(*m_ui->sampleWidget_3),
        std::ref(*m_ui->sampleWidget_4),
        std::ref(*m_ui->sampleWidget_5),
        std::ref(*m_ui->sampleWidget_6),
        std::ref(*m_ui->sampleWidget_7),
        std::ref(*m_ui->sampleWidget_8),
        std::ref(*m_ui->sampleWidget_9),
        std::ref(*m_ui->sampleWidget_10),
        std::ref(*m_ui->sampleWidget_11),
        std::ref(*m_ui->sampleWidget_12),
        std::ref(*m_ui->sampleWidget_13),
        std::ref(*m_ui->sampleWidget_14),
        std::ref(*m_ui->sampleWidget_15),
        std::ref(*m_ui->sampleWidget_16),
        std::ref(*m_ui->sampleWidget_17),
        std::ref(*m_ui->sampleWidget_18),
        std::ref(*m_ui->sampleWidget_19),
        std::ref(*m_ui->sampleWidget_20),
        std::ref(*m_ui->sampleWidget_21),
        std::ref(*m_ui->sampleWidget_22),
        std::ref(*m_ui->sampleWidget_23),
        std::ref(*m_ui->sampleWidget_24)
    };
}