#include "scratchwidget.h"

#include <QPainter>
#include <QRect>
#include <QMouseEvent>
#include <QDebug>

#include "audioformat.h"
#include "graphrenderer.h"

ScratchWidget::ScratchWidget(QWidget *parent) :
    QWidget{parent},
    m_framesPerBeat{frameRate/4}
{
    connect(&m_timer, &QTimer::timeout, this, &ScratchWidget::timeout);
    m_timer.setSingleShot(true);
    m_timer.setInterval(100);
}

void ScratchWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter;
    painter.begin(this);

    painter.setPen({});
    painter.setBrush(palette().window());
    painter.drawRect(rect());

    if (m_buffer.isValid() && m_position < m_buffer.frameCount() - m_framesPerBeat)
    {
        {
            QPen pen{Qt::blue};
            pen.setWidth(3);
            painter.setPen(pen);
        }

        const auto doit = [&](int offset)
        {
            int x = ((width()/2)-(float(m_position % m_framesPerBeat) / m_framesPerBeat * m_beatWidth)) + (m_beatWidth*offset);
            const auto pixmap = getPixmap((m_position/m_framesPerBeat)+offset);
            if (!pixmap.isNull())
                painter.drawPixmap(x, 0, pixmap);
        };

        doit(-2);
        doit(-1);
        doit(0);
        doit(1);
        doit(2);
    }

    {
        QPen pen{Qt::red};
        pen.setWidth(3);
        painter.setPen(pen);
    }

    painter.drawLine(width()/2, 0, width()/2, height());

    painter.end();
}

void ScratchWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_scratching = true;
        m_mouseX = event->x();
        m_timestamp = QDateTime::currentDateTime();
        setMouseTracking(true);
        emit scratchBegin();
        emit scratchSpeed(0.f);
    }
}

void ScratchWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_timer.stop();
        emit scratchSpeed(1.f);
        m_scratching = false;
        setMouseTracking(false);
        emit scratchEnd();
    }
}

void ScratchWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_scratching)
    {
        const auto now = QDateTime::currentDateTime();

        int dx = m_mouseX - event->x();
        int dt = m_timestamp.msecsTo(now);

        emit scratchSpeed(float(dx) / dt * m_framesPerBeat / m_beatWidth / 50);

        m_mouseX = event->x();
        m_timestamp = now;
        m_timer.start();
    }
}

void ScratchWidget::timeout()
{
    emit scratchSpeed(0.f);
}

QPixmap ScratchWidget::getPixmap(int index)
{
    {
        const auto pixmap = m_graphCache[index];
        if (pixmap)
            return *pixmap;
    }

    if (!m_buffer.isValid() || index < 0 || index >= m_buffer.frameCount()/m_framesPerBeat)
    {
        qWarning() << index;
        return {};
    }

    const auto *begin = m_buffer.constData<frame_t>() + (index*m_framesPerBeat);

    const auto pixmap = new QPixmap{QSize{m_beatWidth, height()}};

    {
        QPainter painter;
        painter.begin(pixmap);

        painter.setPen(Qt::blue);
        painter.setBrush(palette().base());
        painter.drawRect(pixmap->rect());

        painter.setPen(QPen{palette().color(QPalette::Text)});
        painter.setBrush(palette().text());
        GraphRenderer::render(pixmap->rect(), begin, begin+m_framesPerBeat, painter);
    }

    m_graphCache.insert(index, pixmap);

    return *pixmap;
}
