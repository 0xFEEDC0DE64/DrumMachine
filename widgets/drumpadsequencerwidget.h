#pragma once

#include <memory>
#include <vector>
#include <array>

#include <QWidget>
#include <QTimer>

class QLabel;
namespace Ui { class DrumPadSequencerWidget; }
namespace drumpad_presets { struct Preset; struct Sequence; }
class DrumMachineSettings;
namespace midi { struct MidiMessage; }

class DrumPadSequencerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrumPadSequencerWidget(QWidget *parent = nullptr);
    ~DrumPadSequencerWidget() override;

    void loadSettings(DrumMachineSettings &settings);
    void unsendColors();
    void sendColors();

    void setPreset(const drumpad_presets::Preset &preset);

signals:
    void sendMidi(const midi::MidiMessage &midiMsg);
    void triggerSample(int index);

public slots:
    void midiReceived(const midi::MidiMessage &message);

private slots:
    void playPause();
    void stop();

    void tempoChanged(int tempo);
    void sequenceSelected();
    void timeout();

    void updateStatusLabel();

    void selectPrevSequence();
    void selectNextSequence();

private:
    const std::unique_ptr<Ui::DrumPadSequencerWidget> m_ui;

    DrumMachineSettings *m_settings{};

    std::vector<drumpad_presets::Sequence> m_sequences;
    const drumpad_presets::Sequence *m_selectedSequence{};

    QTimer m_timer;

    int m_pos{};
    std::array<QLabel*, 24> m_sampleLabels;

    bool m_sendColors{};
};
