#pragma once

#include <QWidget>

#include <memory>

namespace Ui { class LoopStationWidget; }
namespace midi { struct MidiMessage; }
class QNetworkAccessManager;
class DrumMachineSettings;
struct frame_t;

class LoopStationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoopStationWidget(QWidget *parent = nullptr);
    ~LoopStationWidget() override;

    void writeSamples(frame_t *begin, frame_t *end);
    void injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager);
    void injectDecodingThread(QThread &thread);
    void loadSettings(DrumMachineSettings &settings);
    void unsendColors();
    void sendColors();

signals:
    void sendMidi(const midi::MidiMessage &midiMsg);

public slots:
    void midiReceived(const midi::MidiMessage &message);

private:
    const std::unique_ptr<Ui::LoopStationWidget> m_ui;
};
