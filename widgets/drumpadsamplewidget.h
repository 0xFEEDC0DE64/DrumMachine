#pragma once

#include <memory>

#include <QFrame>

#include "drumpadpresets.h"
#include "audioplayer.h"

namespace Ui { class DrumPadSampleWidget; }
class QNetworkAccessManager;
class QNetworkReply;
class QAudioBuffer;
class AudioDecoder;
class DrumMachineSettings;
namespace midi { struct MidiMessage; }
struct frame_t;

class DrumPadSampleWidget : public QFrame
{
    Q_OBJECT

public:
    explicit DrumPadSampleWidget(QWidget *parent = nullptr);
    ~DrumPadSampleWidget() override;

    quint8 padNr() const { return m_padNr; }
    void setPadNr(quint8 padNr) { m_padNr = padNr; }

    void loadSettings(DrumMachineSettings &settings);

    void setFile(const QString &presetId, const drumpad_presets::File &file);

    int speed() const;
    void setSpeed(int speed);

    int volume() const;
    void setVolume(int volume);

    std::optional<int> choke() const;

    void pressed();
    void released();

    void forceStop();

    void injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager);
    void injectDecodingThread(QThread &thread);

    void writeSamples(frame_t *begin, frame_t *end);

    void midiReceived(const midi::MidiMessage &message);

    void unsendColor();
    void sendColor();

signals:
    void chokeTriggered(int choke);
    void startDecoding(std::shared_ptr<QIODevice> device);
    void sendMidi(const midi::MidiMessage &midiMsg);

private slots:
    void updateStatus();
    void requestFinished();
    void decodingFinished(const QAudioBuffer &buffer);

private:
    void startRequest();

    const std::unique_ptr<Ui::DrumPadSampleWidget> m_ui;

    DrumMachineSettings *m_settings{};

    std::shared_ptr<QNetworkReply> m_reply;

    std::unique_ptr<AudioDecoder> m_decoder;

    AudioPlayer m_player;

    QString m_presetId;
    std::optional<drumpad_presets::File> m_file;

    QNetworkAccessManager *m_networkAccessManager{};

    quint8 m_padNr{};

    bool m_sendColors{};

    QColor m_lastColor;
    quint8 m_lastMidiColor{};
};
