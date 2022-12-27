#pragma once

#include <memory>
#include <array>
#include <functional>

#include <QWidget>

#include "drumpadpresets.h"

namespace Ui { class DrumPadSamplesWidget; }
namespace midi { struct MidiMessage; }
class QNetworkAccessManager;
class DrumPadSampleWidget;
class DrumMachineSettings;
struct frame_t;

class DrumPadSamplesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrumPadSamplesWidget(QWidget *parent = nullptr);
    ~DrumPadSamplesWidget() override;

    void loadSettings(DrumMachineSettings &settings);

    void setPreset(const drumpad_presets::Preset &preset);

    void midiReceived(const midi::MidiMessage &message);

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
    std::array<std::reference_wrapper<DrumPadSampleWidget>, 24> getWidgets();

    const std::unique_ptr<Ui::DrumPadSamplesWidget> m_ui;

    drumpad_presets::Preset m_preset;
};
