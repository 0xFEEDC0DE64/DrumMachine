#pragma once

#include <vector>

#include <QWidget>
#include <QAudioBuffer>

class ScratchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScratchWidget(QWidget *parent = nullptr);

    const QAudioBuffer &buffer() const { return m_buffer; }
    void setBuffer(const QAudioBuffer &buffer) { m_buffer = buffer; m_graphCache.clear(); repaint(); }

    double position() const { return m_position; }
    void setPosition(double position) { m_position = position; repaint(); }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QAudioBuffer m_buffer;
    double m_position{};
    std::vector<QPixmap> m_graphCache;
};
