#include "scratchwidget.h"

#include <QPainter>
#include <QRect>
#include <QDebug>

#include "graphrenderer.h"

constexpr auto theWidth = 100;

ScratchWidget::ScratchWidget(QWidget *parent) :
    QWidget{parent}
{
}

void ScratchWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);

    painter.setPen({});
    painter.setBrush(palette().window());
    painter.drawRect(rect());

    if (m_buffer.isValid() && m_position < m_buffer.frameCount() - sampleRate)
    {
        {
            QPen pen{Qt::blue};
            pen.setWidth(3);
            painter.setPen(pen);
        }

        const auto doit = [&](int offset)
        {
            int x = ((width()/2)-(float(m_position % sampleRate) / sampleRate * theWidth)) + (theWidth*offset);
            const auto pixmap = getPixmap((m_position/sampleRate)+offset);
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

}

void ScratchWidget::mouseReleaseEvent(QMouseEvent *event)
{

}

void ScratchWidget::mouseMoveEvent(QMouseEvent *event)
{

}

QPixmap ScratchWidget::getPixmap(int index)
{
    {
        const auto pixmap = m_graphCache[index];
        if (pixmap)
            return *pixmap;
    }

    if (!m_buffer.isValid() || index < 0 || index >= m_buffer.frameCount()/sampleRate)
    {
        qWarning() << index;
        return {};
    }

    const auto *begin = m_buffer.constData<frame_t>() + (index*sampleRate);
    const auto pixmap = GraphRenderer::render(QSize{theWidth, height()}, begin, begin+sampleRate, palette());

    m_graphCache.insert(index, new QPixmap{pixmap});

    return pixmap;
}
