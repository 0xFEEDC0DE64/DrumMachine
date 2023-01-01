#include "drumpadsampleswidget.h"
#include "ui_drumpadsampleswidget.h"

#include <iterator>

#include <QDebug>
#include <QtGlobal>

#include "audioformat.h"
#include "midicontainers.h"
#include "drummachinesettings.h"
#include "drumpadsettingsdialog.h"

DrumPadSamplesWidget::DrumPadSamplesWidget(QWidget *parent) :
    QWidget{parent},
    m_ui{std::make_unique<Ui::DrumPadSamplesWidget>()}
{
    m_ui->setupUi(this);

    connect(m_ui->pushButtonSettings, &QAbstractButton::pressed, this, &DrumPadSamplesWidget::showSettings);
    connect(m_ui->pushButtonSwap, &QAbstractButton::toggled, this, &DrumPadSamplesWidget::updateWidgets);
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
    m_settings = &settings;

    m_ui->pushButtonSwap->setLearnSetting(settings.drumpadSwap());
    m_ui->pushButtonStopAll->setLearnSetting(settings.drumpadStopAll());

    connect(m_ui->pushButtonSwap, &MidiButton::learnSettingChanged, &settings, &DrumMachineSettings::setDrumpadSwap);
    connect(m_ui->pushButtonStopAll, &MidiButton::learnSettingChanged, &settings, &DrumMachineSettings::setDrumpadStopAll);

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
    m_ui->pushButtonSwap->midiReceived(message);
    m_ui->pushButtonStopAll->midiReceived(message);

    for (DrumPadSampleWidget &widget : getWidgets())
        widget.midiReceived(message);
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
    const quint8 color = m_settings ? m_settings->colorOff() : quint8{0};

    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonSwap->learnSetting().channel,
        .cmd = m_ui->pushButtonSwap->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonSwap->learnSetting().note,
        .velocity = color
    });
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonStopAll->learnSetting().channel,
        .cmd = m_ui->pushButtonStopAll->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonStopAll->learnSetting().note,
        .velocity = color
    });

    for (DrumPadSampleWidget &widget : getWidgets())
        widget.unsendColor();
}

void DrumPadSamplesWidget::sendColors()
{
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonSwap->learnSetting().channel,
        .cmd = m_ui->pushButtonSwap->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonSwap->learnSetting().note,
        .velocity = m_settings ? m_settings->drumpadColorSwap() : quint8{127}
    });
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonStopAll->learnSetting().channel,
        .cmd = m_ui->pushButtonStopAll->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonStopAll->learnSetting().note,
        .velocity = m_settings ? m_settings->drumpadColorStopAll() : quint8{127}
    });

    for (DrumPadSampleWidget &widget : getWidgets())
        widget.sendColor(true);
}

void DrumPadSamplesWidget::sequencerTriggerSample(int index)
{
    const auto widgets = getWidgets();
    if (index < 0 || index >= int(std::size(widgets)))
    {
        qDebug() << "index out of range" << index;
        return;
    }
    widgets[index].get().pressed();
}

void DrumPadSamplesWidget::showSettings()
{
    if (!m_settings)
    {
        qWarning() << "settings are missing";
        return;
    }

    DrumPadSettingsDialog dialog{*m_settings, this};
    dialog.exec();
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

    if (m_ui->pushButtonSwap->isChecked())
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
