#pragma once

#include <QWidget>
#include <QPixmap>
#include <QAudioBuffer>

class PreviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PreviewWidget(QWidget *parent = nullptr);

    const QAudioBuffer &buffer() const { return m_buffer; }
    void setBuffer(const QAudioBuffer &buffer) { m_buffer = buffer; m_graphCache = {}; repaint(); }

    double position() const { return m_position; }
    void setPosition(double position) { m_position = position; repaint(); }

signals:
    void positionSelected(double sample);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void clicked(int x, int y);

    QAudioBuffer m_buffer;
    double m_position{};
    QPixmap m_graphCache;
};
