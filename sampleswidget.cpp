#include "sampleswidget.h"
#include "ui_sampleswidget.h"

#include <iterator>

#include <QDebug>

#include "midicontainers.h"

SamplesWidget::SamplesWidget(QWidget *parent) :
    QWidget{parent},
    m_ui{std::make_unique<Ui::SamplesWidget>()}
{
    m_ui->setupUi(this);

    m_cache.setCacheDirectory("cache");
    m_networkAccessManager.setCache(&m_cache);

    connect(m_ui->checkBox, &QCheckBox::toggled, this, &SamplesWidget::updateWidgets);

    connect(m_ui->pushButtonStopAll, &QAbstractButton::pressed, this, &SamplesWidget::stopAll);

    for (SampleWidget &widget : getWidgets())
    {
        widget.injectNetworkAccessManager(m_networkAccessManager);
        connect(&widget, &SampleWidget::chokeTriggered, this, &SamplesWidget::chokeTriggered);
    }

    m_ui->sampleWidget_1->setNote(48);
    m_ui->sampleWidget_2->setNote(50);
    m_ui->sampleWidget_3->setNote(52);
    m_ui->sampleWidget_4->setNote(53);
    m_ui->sampleWidget_5->setNote(55);
    m_ui->sampleWidget_6->setNote(57);
    m_ui->sampleWidget_7->setNote(59);
    m_ui->sampleWidget_8->setNote(60);
    m_ui->sampleWidget_9->setNote(62);
    m_ui->sampleWidget_10->setNote(64);
    m_ui->sampleWidget_11->setNote(65);
    m_ui->sampleWidget_12->setNote(67);
    m_ui->sampleWidget_22->setNote(69);
    m_ui->sampleWidget_23->setNote(71);
    m_ui->sampleWidget_24->setNote(72);
}

SamplesWidget::~SamplesWidget() = default;

void SamplesWidget::setPreset(const presets::Preset &preset)
{
    m_preset = preset;

    updateWidgets();
}

void SamplesWidget::messageReceived(const midi::MidiMessage &message)
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

void SamplesWidget::injectDecodingThread(QThread &thread)
{
    for (SampleWidget &widget : getWidgets())
        widget.injectDecodingThread(thread);
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
