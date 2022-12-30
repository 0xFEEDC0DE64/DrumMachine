#pragma once

#include <QWidget>
#include <QTimer>

#include <memory>
#include <array>
#include <functional>

#include "loopstationpresets.h"

namespace Ui { class LoopStationSamplesWidget; }
namespace midi { struct MidiMessage; }
class QNetworkAccessManager;
class LoopStationSampleWidget;
class DrumMachineSettings;
struct frame_t;

class LoopStationSamplesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoopStationSamplesWidget(QWidget *parent = nullptr);
    ~LoopStationSamplesWidget();

    void loadSettings(DrumMachineSettings &settings);

    void setPreset(const loopstation_presets::Preset &preset);

    void midiReceived(const midi::MidiMessage &message);

    void writeSamples(frame_t *begin, frame_t *end);

    void injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager);
    void injectDecodingThread(QThread &thread);

    void unsendColors();
    void sendColors();

signals:
    void sendMidi(const midi::MidiMessage &midiMsg);

private slots:
    void timeout();
    void tempoChanged(int tempo);
    void loopEnabled(quint8 category);
    void playPausePressed();
    void stopPressed();

private:
    std::array<std::reference_wrapper<LoopStationSampleWidget>, 48> getWidgets();

    const std::unique_ptr<Ui::LoopStationSamplesWidget> m_ui;

    QTimer m_timer;

    quint8 m_pos{};
};
