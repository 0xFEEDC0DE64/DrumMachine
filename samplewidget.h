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

class SampleWidget : public QFrame
{
    Q_OBJECT

public:
    explicit SampleWidget(QWidget *parent = nullptr);
    ~SampleWidget() override;

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

signals:
    void chokeTriggered(int choke);
    void startDecoding(const std::shared_ptr<QIODevice> &device);

private slots:
    void updateStatus();
    void requestFinished();
    void decodingFinished(const QAudioBuffer &buffer);
    void learnPressed();

private:
    void startRequest();

    const std::unique_ptr<Ui::SampleWidget> m_ui;

    std::shared_ptr<QNetworkReply> m_reply;

    std::unique_ptr<AudioDecoder> m_decoder;

    AudioPlayer m_player;

    QString m_presetId;
    std::optional<presets::File> m_file;

    QNetworkAccessManager *m_networkAccessManager{};

    bool m_learning{};
    QColor m_oldColor;
    QBrush m_oldBrush;
};
