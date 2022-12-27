#pragma once

#include <memory>
#include <array>
#include <functional>

#include <QWidget>

#include "audioformat.h"
#include "presets.h"

namespace Ui { class SamplesWidget; }
namespace midi { class MidiMessage; }
class QNetworkAccessManager;
class SampleWidget;
class DrumMachineSettings;

class SamplesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SamplesWidget(QWidget *parent = nullptr);
    ~SamplesWidget() override;

    void loadSettings(DrumMachineSettings &settings);

    void setPreset(const presets::Preset &preset);

    void messageReceived(const midi::MidiMessage &message);

    void writeSamples(frame_t *begin, frame_t *end);

    void injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager);
    void injectDecodingThread(QThread &thread);

    void unsendColors();
    void sendColors();

signals:
    void sendMidi(const midi::MidiMessage &midiMsg);

public slots:
    void sequencerTriggerSample(int index);

private slots:
    void chokeTriggered(int choke);
    void updateWidgets();
    void stopAll();

private:
    std::array<std::reference_wrapper<SampleWidget>, 24> getWidgets();

    const std::unique_ptr<Ui::SamplesWidget> m_ui;

    presets::Preset m_preset;
};
