#pragma once

#include <QWidget>
#include <QAudioBuffer>
#include <QCache>
#include <QDateTime>
#include <QTimer>

class ScratchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScratchWidget(QWidget *parent = nullptr);

    const QAudioBuffer &buffer() const { return m_buffer; }
    void setBuffer(const QAudioBuffer &buffer) { m_buffer = buffer; m_graphCache.clear(); repaint(); }

    std::ptrdiff_t position() const { return m_position; }
    void setPosition(std::ptrdiff_t position) { m_position = position; repaint(); }

    int beatWidth() const { return m_beatWidth; };
    void setBeatWidth(int beatWidth) { m_beatWidth = beatWidth; m_graphCache.clear(); repaint(); };

    int framesPerBeat() const { return m_framesPerBeat; }
    void setFramesPerBeat(int framesPerBeat) { m_framesPerBeat = framesPerBeat; m_graphCache.clear(); repaint(); }

signals:
    void scratchBegin();
    void scratchSpeed(float speed);
    void scratchEnd();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void timeout();

private:
    QPixmap getPixmap(int index);

    QAudioBuffer m_buffer;
    std::ptrdiff_t m_position{};
    QCache<int, QPixmap> m_graphCache;

    int m_beatWidth{100};
    int m_framesPerBeat;

    bool m_scratching{};
    bool m_dragging{};
    int m_mouseX;
    QDateTime m_timestamp;
    QTimer m_timer;
};
