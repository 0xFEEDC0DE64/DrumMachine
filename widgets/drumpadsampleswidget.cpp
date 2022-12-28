#include "drumpadsampleswidget.h"
#include "ui_drumpadsampleswidget.h"

#include <iterator>

#include <QDebug>
#include <QtGlobal>

#include "audioformat.h"
#include "midicontainers.h"

DrumPadSamplesWidget::DrumPadSamplesWidget(QWidget *parent) :
    QWidget{parent},
    m_ui{std::make_unique<Ui::DrumPadSamplesWidget>()}
{
    m_ui->setupUi(this);

    connect(m_ui->checkBox, &QCheckBox::toggled, this, &DrumPadSamplesWidget::updateWidgets);

    connect(m_ui->pushButtonStopAll, &QAbstractButton::pressed, this, &DrumPadSamplesWidget::stopAll);

    quint8 padNr{};
    for (DrumPadSampleWidget &widget : getWidgets())
    {
        widget.setPadNr(padNr++);
        connect(&widget, &DrumPadSampleWidget::chokeTriggered, this, &DrumPadSamplesWidget::chokeTriggered);
        connect(&widget, &DrumPadSampleWidget::sendMidi, this, &DrumPadSamplesWidget::sendMidi);
    }
}

DrumPadSamplesWidget::~DrumPadSamplesWidget() = default;

void DrumPadSamplesWidget::loadSettings(DrumMachineSettings &settings)
{
    for (DrumPadSampleWidget &widget : getWidgets())
        widget.loadSettings(settings);
}

void DrumPadSamplesWidget::setPreset(const drumpad_presets::Preset &preset)
{
    m_preset = preset;

    updateWidgets();
}

void DrumPadSamplesWidget::midiReceived(const midi::MidiMessage &message)
{
    if (message == midi::MidiMessage{.channel=0,.cmd=midi::Command::ControlChange,.flag=true,.note=64,.velocity=127})
    {
        m_ui->checkBox->toggle();
        return;
    }

    if (message.cmd != midi::Command::NoteOn &&
        message.cmd != midi::Command::NoteOff &&
        message.cmd != midi::Command::ControlChange)
        return;

    for (DrumPadSampleWidget &widget : getWidgets())
    {
        if (widget.isLearning())
        {
            widget.learn(message.channel, message.note);
        }
        else if (widget.channel() == message.channel && widget.note() == message.note)
        {
            switch (message.cmd)
            {
            case midi::Command::NoteOn:
            case midi::Command::ControlChange:
                if (message.velocity != 0)
                    widget.pressed(message.velocity);
                else
            Q_FALLTHROUGH();
            case midi::Command::NoteOff:
                widget.released();
                break;
            default:
                __builtin_unreachable();
            }
        }
    }
}

void DrumPadSamplesWidget::writeSamples(frame_t *begin, frame_t *end)
{
    for (DrumPadSampleWidget &widget : getWidgets())
        widget.writeSamples(begin, end);
}

void DrumPadSamplesWidget::injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager)
{
    for (DrumPadSampleWidget &widget : getWidgets())
        widget.injectNetworkAccessManager(networkAccessManager);
}

void DrumPadSamplesWidget::injectDecodingThread(QThread &thread)
{
    for (DrumPadSampleWidget &widget : getWidgets())
        widget.injectDecodingThread(thread);
}

void DrumPadSamplesWidget::unsendColors()
{
    for (DrumPadSampleWidget &widget : getWidgets())
        widget.unsendColor();
}

void DrumPadSamplesWidget::sendColors()
{
    for (DrumPadSampleWidget &widget : getWidgets())
        widget.sendColor();
}

void DrumPadSamplesWidget::sequencerTriggerSample(int index)
{
    const auto widgets = getWidgets();
    if (index < 0 || index >= int(std::size(widgets)))
    {
        qDebug() << "index out of range" << index;
        return;
    }
    widgets[index].get().pressed(127);
}

void DrumPadSamplesWidget::chokeTriggered(int choke)
{
    for (DrumPadSampleWidget &widget : getWidgets())
    {
        if (&widget == sender())
            continue;

        if (widget.choke() && *widget.choke() && *widget.choke() == choke)
            widget.forceStop();
    }
}

void DrumPadSamplesWidget::updateWidgets()
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

void DrumPadSamplesWidget::stopAll()
{
    for (DrumPadSampleWidget &widget : getWidgets())
        widget.forceStop();
}

std::array<std::reference_wrapper<DrumPadSampleWidget>, 24> DrumPadSamplesWidget::getWidgets()
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
