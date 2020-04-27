#pragma once

#include <QWidget>

#include <memory>

#include "audioformat.h"
#include "audioplayer.h"

namespace Ui { class TrackDeck; }
class AudioDecoder;
class QAudioBuffer;

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

private:
    const std::unique_ptr<Ui::TrackDeck> m_ui;

    std::unique_ptr<AudioDecoder> m_decoder;

    AudioPlayer m_player;

    QString m_filename;

    bool m_playingBeforeScratch;
    float m_speedBeforeScratch;
    bool m_stopOnEndBeforeScratch;
};
