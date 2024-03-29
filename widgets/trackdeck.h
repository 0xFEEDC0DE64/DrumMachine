#pragma once

#include <memory>
#include <optional>
#include <tuple>

#include <QWidget>
#include <QButtonGroup>
#include <QTimer>

#include "audioplayer.h"

namespace Ui { class TrackDeck; }
class AudioDecoder;
class QAudioBuffer;
struct frame_t;

class TrackDeck : public QWidget
{
    Q_OBJECT

public:
    explicit TrackDeck(QWidget *parent = nullptr);
    ~TrackDeck() override;

    void loadTrack(const QString &filename);

    void injectDecodingThread(QThread &thread);

    void writeSamples(frame_t *begin, frame_t *end);

signals:
    void startDecoding(const QString &filename);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void decodingProgress(int progress, int total);
    void decodingFinished(const QAudioBuffer &buffer);
    void scratchBegin();
    void scratchEnd();
    void speedChanged(int value);
    void updatePlayButtonText(bool playing);
    void bpmTap();
    void timeout();
    void updatePlaybackBpm();

private:
    const std::unique_ptr<Ui::TrackDeck> m_ui;

    QButtonGroup m_loopGroup;

    std::unique_ptr<AudioDecoder> m_decoder;

    AudioPlayer m_player;

    QString m_filename;

    bool m_playingBeforeScratch;
    float m_speedBeforeScratch;
    bool m_stopOnEndBeforeScratch;

    QTimer m_timer;

    std::optional<std::tuple<double, double, int>> m_bpmTap;
};
