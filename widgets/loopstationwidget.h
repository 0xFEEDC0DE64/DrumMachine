#pragma once

#include <QSplitter>

#include <memory>

namespace Ui { class LoopStationWidget; }
namespace midi { struct MidiMessage; }
class QNetworkAccessManager;
class QNetworkReply;
class DrumMachineSettings;
struct frame_t;

class LoopStationWidget : public QSplitter
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

private slots:
    void loadPresets();
    void requestFinished();
    void selectFirstPreset();
    void selectPrevPreset();
    void selectNextPreset();

private:
    const std::unique_ptr<Ui::LoopStationWidget> m_ui;

    QNetworkAccessManager *m_networkAccessManager{};

    std::unique_ptr<QNetworkReply> m_reply;
};
