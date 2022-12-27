#pragma once

#include <QWidget>
#include <memory>

#include "synthisizer.h"
#include "audioformat.h"

namespace Ui { class SynthisizerWidget; }
class DrumMachineSettings;

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
    void messageReceived(const midi::MidiMessage &message);

private:
    const std::unique_ptr<Ui::SynthisizerWidget> m_ui;

    Synthisizer m_synthisizer;
};
