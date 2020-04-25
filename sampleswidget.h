#pragma once

#include <memory>
#include <array>
#include <functional>

#include <QWidget>
#include <QThread>

#include "presets.h"

namespace Ui { class SamplesWidget; }
namespace midi { class MidiMessage; }
class SampleWidget;
class QAudioDeviceInfo;

class SamplesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SamplesWidget(QWidget *parent = nullptr);
    ~SamplesWidget() override;

    void setPreset(const presets::Preset &preset);

    void messageReceived(const midi::MidiMessage &message);

    void setAudioDevice(const QAudioDeviceInfo &device);

private slots:
    void chokeTriggered(int choke);
    void updateWidgets();
    void sequencerTriggerSample(int index);
    void stopAll();

private:
    std::array<std::reference_wrapper<SampleWidget>, 24> getWidgets();

    const std::unique_ptr<Ui::SamplesWidget> m_ui;

    presets::Preset m_preset;

    QThread m_audioThread;
};
