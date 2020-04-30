#include "previewwidget.h"

#include <QPainter>
#include <QPen>
#include <QMouseEvent>

#include "graphrenderer.h"

PreviewWidget::PreviewWidget(QWidget *parent) :
    QWidget(parent)
{
}

void PreviewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    if (m_graphCache.isNull() || m_graphCache.size() != size())
    {
        m_graphCache = QPixmap{size()};

        QPainter painter;
        painter.begin(&m_graphCache);

        painter.setBrush(palette().base());
        painter.drawRect(m_graphCache.rect());

        painter.setPen(QPen{palette().color(QPalette::Text)});
        painter.setBrush(palette().text());

        if (m_buffer.isValid())
            GraphRenderer::render(m_graphCache.rect(), m_buffer.constData<frame_t>(), m_buffer.constData<frame_t>() + m_buffer.frameCount(), painter);

        painter.end();
    }

    QPainter painter;
    painter.begin(this);

    painter.drawPixmap(0, 0, m_graphCache);

    if (m_buffer.frameCount())
    {
        std::size_t currentSample = double(m_position) / m_buffer.frameCount() * width();

        QPen pen(Qt::red);
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawLine(currentSample, 0, currentSample, height());
    }

    painter.end();
}

void PreviewWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        setMouseTracking(true);
        clicked(event->x(), event->y());
    }

    QWidget::mousePressEvent(event);
}

void PreviewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        setMouseTracking(false);

    QWidget::mouseReleaseEvent(event);
}

void PreviewWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::LeftButton))
        clicked(event->x(), event->y());

    QWidget::mouseMoveEvent(event);
}

void PreviewWidget::clicked(int x, int y)
{
    if (!m_buffer.isValid())
        return;

    const auto samplesPerPixel = m_buffer.frameCount() / width();

    emit positionSelected(x * samplesPerPixel);
}
