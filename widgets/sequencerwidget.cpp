#include "sequencerwidget.h"
#include "ui_sequencerwidget.h"

#include <algorithm>

#include <QDebug>

#include "drumpadpresets.h"
#include "drummachinesettings.h"
#include "midicontainers.h"

SequencerWidget::SequencerWidget(QWidget *parent) :
    QWidget{parent},
    m_ui{std::make_unique<Ui::SequencerWidget>()}
{
    m_ui->setupUi(this);

    connect(m_ui->spinBoxTempo, qOverload<int>(&QSpinBox::valueChanged), this, &SequencerWidget::tempoChanged);

    connect(m_ui->comboBoxSequence, qOverload<int>(&QComboBox::currentIndexChanged), this, &SequencerWidget::sequenceSelected);

    connect(m_ui->pushButtonUp, &QAbstractButton::pressed, this, &SequencerWidget::selectPrevSequence);
    connect(m_ui->pushButtonDown, &QAbstractButton::pressed, this, &SequencerWidget::selectNextSequence);
    connect(m_ui->pushButtonPlayPause, &QAbstractButton::pressed, this, &SequencerWidget::playPause);
    connect(m_ui->pushButtonStop, &QAbstractButton::pressed, this, &SequencerWidget::stop);

    connect(m_ui->horizontalSlider, &QSlider::valueChanged, this, [=](int value){ m_pos = value; updateStatusLabel(); });


    connect(&m_timer, &QTimer::timeout, this, &SequencerWidget::timeout);

    updateStatusLabel();
}

SequencerWidget::~SequencerWidget() = default;

void SequencerWidget::loadSettings(DrumMachineSettings &settings)
{
    m_ui->pushButtonUp->setLearnSetting(settings.drumpadPrevSequence());
    m_ui->pushButtonDown->setLearnSetting(settings.drumpadNextSequence());
    m_ui->pushButtonPlayPause->setLearnSetting(settings.drumpadPlayPause());
    m_ui->pushButtonStop->setLearnSetting(settings.drumpadStop());

    connect(m_ui->pushButtonUp, &MidiButton::learnSettingChanged, &settings, &DrumMachineSettings::setDrumpadPrevSequence);
    connect(m_ui->pushButtonDown, &MidiButton::learnSettingChanged, &settings, &DrumMachineSettings::setDrumpadNextSequence);
    connect(m_ui->pushButtonPlayPause, &MidiButton::learnSettingChanged, &settings, &DrumMachineSettings::setDrumpadPlayPause);
    connect(m_ui->pushButtonStop, &MidiButton::learnSettingChanged, &settings, &DrumMachineSettings::setDrumpadStop);
}

void SequencerWidget::unsendColors()
{
    m_sendColors = false;

    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonUp->learnSetting().channel,
        .cmd = m_ui->pushButtonUp->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonUp->learnSetting().note,
        .velocity = 0
    });
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonDown->learnSetting().channel,
        .cmd = m_ui->pushButtonDown->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonDown->learnSetting().note,
        .velocity = 0
    });
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonPlayPause->learnSetting().channel,
        .cmd = m_ui->pushButtonPlayPause->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonPlayPause->learnSetting().note,
        .velocity = 0
    });
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonStop->learnSetting().channel,
        .cmd = m_ui->pushButtonStop->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonStop->learnSetting().note,
        .velocity = 0
    });
}

void SequencerWidget::sendColors()
{
    m_sendColors = true;

    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonUp->learnSetting().channel,
        .cmd = m_ui->pushButtonUp->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonUp->learnSetting().note,
        .velocity = 127
    });
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonDown->learnSetting().channel,
        .cmd = m_ui->pushButtonDown->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonDown->learnSetting().note,
        .velocity = 127
    });
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonPlayPause->learnSetting().channel,
        .cmd = m_ui->pushButtonPlayPause->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonPlayPause->learnSetting().note,
        .velocity = 60
    });
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonStop->learnSetting().channel,
        .cmd = m_ui->pushButtonStop->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonStop->learnSetting().note,
        .velocity = 3
    });
}

void SequencerWidget::setPreset(const drumpad_presets::Preset &preset)
{
    if (preset.tempo)
        m_ui->spinBoxTempo->setValue(*preset.tempo);

    for (int i = 0; i < 24; i++)
        qobject_cast<QLabel*>(m_ui->gridLayout->itemAtPosition(2+(i*2), 0)->widget())->setText(*preset.files->at(i).filename);

    m_selectedSequence = nullptr;
    m_ui->horizontalSlider->setMaximum(0);

    m_ui->comboBoxSequence->clear();
    m_sequences.clear();
    m_selectedSequence = nullptr;

    const auto doit = [&](const QString &prefix, const std::optional<std::map<QString, std::vector<drumpad_presets::Sequence>>> &value)
    {
        if (!value)
            return;

        for (const auto &pair : *value)
        {
            for (const auto &sequence : pair.second)
            {
                m_ui->comboBoxSequence->addItem(QString{"%0/%1/%2"}.arg(prefix, pair.first, sequence.name?*sequence.name:"(null)"));
                m_sequences.emplace_back(sequence);
            }
        }
    };

    {
        QSignalBlocker blocker{m_ui->comboBoxSequence};
        doit("beatSchool", preset.beatSchool);
        doit("easyPlay", preset.easyPlay);
    }

    sequenceSelected();
}

void SequencerWidget::midiReceived(const midi::MidiMessage &message)
{
    m_ui->pushButtonUp->midiReceived(message);
    m_ui->pushButtonDown->midiReceived(message);
    m_ui->pushButtonPlayPause->midiReceived(message);
    m_ui->pushButtonStop->midiReceived(message);
}

void SequencerWidget::playPause()
{
    if (m_timer.isActive())
    {
        m_timer.stop();
        m_ui->pushButtonPlayPause->setText(tr("▶"));
    }
    else
    {
        m_timer.start();
        m_ui->pushButtonPlayPause->setText(tr("▮▮"));
    }
}

void SequencerWidget::stop()
{
    m_timer.stop();
    m_ui->pushButtonPlayPause->setText(tr("▶"));
    m_pos = 0;
    m_ui->horizontalSlider->setValue(0);
    updateStatusLabel();
}

void SequencerWidget::tempoChanged(int tempo)
{
    m_timer.setInterval(1000. * 60. / tempo / 4.);
}

void SequencerWidget::sequenceSelected()
{
    const auto index = m_ui->comboBoxSequence->currentIndex();

    if (index == -1)
        m_selectedSequence = nullptr;
    else
        m_selectedSequence = &m_sequences[index];

    m_ui->horizontalSlider->setMaximum(m_selectedSequence && m_selectedSequence->sequencerSize ? *m_selectedSequence->sequencerSize-2 : 0);

    m_ui->pushButtonPlayPause->setEnabled(m_selectedSequence != nullptr);
    m_ui->pushButtonStop->setEnabled(m_selectedSequence != nullptr);

    m_pos = 0;
    m_ui->horizontalSlider->setValue(0);
    updateStatusLabel();
}

void SequencerWidget::timeout()
{
    if (m_selectedSequence && m_selectedSequence->pads)
    {
        for (const auto &pair : *m_selectedSequence->pads)
        {
            const auto iter = std::find_if(std::cbegin(pair.second), std::cend(pair.second), [&](const drumpad_presets::SequencePad &sequencePad){
                return sequencePad.start && *sequencePad.start == m_pos;
            });

            if (iter == std::cend(pair.second))
                continue;

            //TODO: iter->duration;

            bool ok;
            const auto index = pair.first.toInt(&ok);
            if (!ok)
                continue;

            emit triggerSample(index);
        }
    }

    m_pos++;

    if (m_pos >= (m_selectedSequence && m_selectedSequence->sequencerSize ? *m_selectedSequence->sequencerSize-1 : -1))
        m_pos = 0;

    m_ui->horizontalSlider->setValue(m_pos);

    updateStatusLabel();
}

void SequencerWidget::updateStatusLabel()
{
    m_ui->labelStatus->setText(QString{"%0 / %1"}.arg(m_pos+1).arg(m_selectedSequence && m_selectedSequence->sequencerSize ? *m_selectedSequence->sequencerSize-1 : -1));
}

void SequencerWidget::selectPrevSequence()
{
    if (const auto index = m_ui->comboBoxSequence->currentIndex(); index > 0)
        m_ui->comboBoxSequence->setCurrentIndex(index - 1);
}

void SequencerWidget::selectNextSequence()
{
    if (const auto index = m_ui->comboBoxSequence->currentIndex(); index + 1 < m_ui->comboBoxSequence->count())
        m_ui->comboBoxSequence->setCurrentIndex(index + 1);
}
