#pragma once

#include <QWidget>
#include <memory>

#include "synthisizer.h"

namespace Ui { class SynthisizerWidget; }
class DrumMachineSettings;
namespace midi { struct MidiMessage; }
struct frame_t;

class SynthisizerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SynthisizerWidget(QWidget *parent = nullptr);
    ~SynthisizerWidget() override;

    void writeSamples(frame_t *begin, frame_t *end);
    void loadSettings(DrumMachineSettings &settings);
    void unsendColors();
    void sendColors();

signals:
    void sendMidi(const midi::MidiMessage &midiMsg);

public slots:
    void midiReceived(const midi::MidiMessage &message);
    void onQuarterNote();

private:
    const std::unique_ptr<Ui::SynthisizerWidget> m_ui;

    std::array<Synthisizer, 10> m_synthisizers;
};
