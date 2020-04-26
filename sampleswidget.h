#pragma once

#include <memory>
#include <array>
#include <functional>

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QThread>

#include "audioformat.h"
#include "presets.h"

namespace Ui { class SamplesWidget; }
namespace midi { class MidiMessage; }
class SampleWidget;

class SamplesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SamplesWidget(QWidget *parent = nullptr);
    ~SamplesWidget() override;

    void setPreset(const presets::Preset &preset);

    void messageReceived(const midi::MidiMessage &message);

    void writeSamples(frame_t *begin, frame_t *end);

public slots:
    void sequencerTriggerSample(int index);

private slots:
    void chokeTriggered(int choke);
    void updateWidgets();
    void stopAll();

private:
    std::array<std::reference_wrapper<SampleWidget>, 24> getWidgets();

    const std::unique_ptr<Ui::SamplesWidget> m_ui;

    QNetworkDiskCache m_cache;
    QNetworkAccessManager m_networkAccessManager;

    QThread m_decoderThread;

    presets::Preset m_preset;
};
