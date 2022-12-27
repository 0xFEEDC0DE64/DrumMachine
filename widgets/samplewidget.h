#pragma once

#include <memory>

#include <QFrame>

#include "audioformat.h"
#include "presets.h"
#include "audioplayer.h"

namespace Ui { class SampleWidget; }
class QNetworkAccessManager;
class QNetworkReply;
class QAudioBuffer;
class AudioDecoder;
class DrumMachineSettings;
namespace midi { struct MidiMessage; }

class SampleWidget : public QFrame
{
    Q_OBJECT

public:
    explicit SampleWidget(QWidget *parent = nullptr);
    ~SampleWidget() override;

    quint8 padNr() const { return m_padNr; }
    void setPadNr(quint8 padNr) { m_padNr = padNr; }

    void loadSettings(DrumMachineSettings &settings);

    void setFile(const QString &presetId, const presets::File &file);

    quint8 channel() const;
    void setChannel(quint8 channel);

    quint8 note() const;
    void setNote(quint8 note);

    int speed() const;
    void setSpeed(int speed);

    int volume() const;
    void setVolume(int volume);

    std::optional<int> choke() const;

    void pressed(quint8 velocity);
    void released();

    void forceStop();

    void injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager);
    void injectDecodingThread(QThread &thread);

    void writeSamples(frame_t *begin, frame_t *end);

    bool isLearning() const { return m_learning; }
    void learn(quint8 channel, quint8 note);

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
    void learnPressed();

private:
    void startRequest();

    const std::unique_ptr<Ui::SampleWidget> m_ui;

    DrumMachineSettings *m_settings{};

    std::shared_ptr<QNetworkReply> m_reply;

    std::unique_ptr<AudioDecoder> m_decoder;

    AudioPlayer m_player;

    QString m_presetId;
    std::optional<presets::File> m_file;

    QNetworkAccessManager *m_networkAccessManager{};

    quint8 m_padNr{};

    bool m_learning{};
    QColor m_oldColor;
    QBrush m_oldBrush;

    bool m_sendColors{};
};
