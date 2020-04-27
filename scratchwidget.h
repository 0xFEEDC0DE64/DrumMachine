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

    std::size_t position() const { return m_position; }
    void setPosition(std::size_t position) { m_position = position; repaint(); }

signals:
    void scratchSpeed(float speed);

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
    std::size_t m_position{};
    QCache<int, QPixmap> m_graphCache;

    bool m_scratching{};
    int m_mouseX;
    QDateTime m_timestamp;
    QTimer m_timer;
};
