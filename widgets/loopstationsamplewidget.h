#pragma once

#include <QFrame>

#include <memory>

#include "audioplayer.h"

namespace Ui { class LoopStationSampleWidget; }
class QNetworkAccessManager;
class QNetworkReply;
class QAudioBuffer;
class AudioDecoder;
class DrumMachineSettings;
namespace midi { struct MidiMessage; }
struct frame_t;

class LoopStationSampleWidget : public QFrame
{
    Q_OBJECT

public:
    explicit LoopStationSampleWidget(QWidget *parent = nullptr);
    ~LoopStationSampleWidget() override;

    quint8 padNr() const { return m_padNr; }
    void setPadNr(quint8 padNr) { m_padNr = padNr; }

    quint8 category() const { return m_category; }
    void setCategory(quint8 category);

    void loadSettings(DrumMachineSettings &settings);

    void setSample(const QString &presetId, const QString &filename, const QString &type);

    void injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager);
    void injectDecodingThread(QThread &thread);

    void writeSamples(frame_t *begin, frame_t *end);

    void midiReceived(const midi::MidiMessage &message);

    void unsendColor();
    void sendColor();

public slots:
    void timeout();
    void setLoopEnabled(bool enabled);
    void stop();

signals:
    void startDecoding(std::shared_ptr<QIODevice> device);
    void sendMidi(const midi::MidiMessage &midiMsg);
    void loopEnabled(quint8 category);

private slots:
    void updateStatus();
    void requestFinished();
    void decodingFinished(const QAudioBuffer &buffer);

private:
    void startRequest();

    const std::unique_ptr<Ui::LoopStationSampleWidget> m_ui;

    DrumMachineSettings *m_settings{};

    std::shared_ptr<QNetworkReply> m_reply;

    std::unique_ptr<AudioDecoder> m_decoder;

    AudioPlayer m_player;

    QString m_presetId;
    QString m_filename;

    QNetworkAccessManager *m_networkAccessManager{};

    quint8 m_padNr{};
    quint8 m_category{};

    bool m_sendColors{};

    QColor m_lastColor;
    quint8 m_lastMidiColor{};
};
