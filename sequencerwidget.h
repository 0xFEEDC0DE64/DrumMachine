#pragma once

#include <memory>
#include <vector>
#include <array>

#include <QWidget>
#include <QTimer>

namespace Ui { class SequencerWidget; }
namespace presets { class Preset; class Sequence; }
class QLabel;

class SequencerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SequencerWidget(QWidget *parent = nullptr);
    ~SequencerWidget() override;

    void setPreset(const presets::Preset &preset);

signals:
    void triggerSample(int index);

private slots:
    void playPause();
    void stop();

    void tempoChanged(int tempo);
    void sequenceSelected();
    void timeout();

    void updateStatusLabel();

private:
    const std::unique_ptr<Ui::SequencerWidget> m_ui;

    std::vector<presets::Sequence> m_sequences;
    const presets::Sequence *m_selectedSequence{};

    QTimer m_timer;

    int m_pos{};
    std::array<QLabel*, 24> m_sampleLabels;
};
