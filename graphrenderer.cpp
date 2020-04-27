#include "graphrenderer.h"

#include <iterator>

#include <QPixmap>
#include <QPainter>
#include <QPalette>

QPixmap GraphRenderer::render(const QSize &size, const frame_t *frameBegin, const frame_t *frameEnd, const QPalette &palette)
{
    QPixmap pixmap{size};

    QPainter painter;
    painter.begin(&pixmap);

    painter.fillRect(pixmap.rect(), palette.base());

    painter.setPen(Qt::white);
    painter.setBrush(Qt::black);

    painter.drawRect(QRect({}, size));

    render(size, frameBegin, frameEnd, painter, palette);
    painter.end();
    return pixmap;
}

void GraphRenderer::render(const QSize &size, const frame_t *frameBegin, const frame_t *frameEnd, QPainter &painter, const QPalette &palette)
{
    if (frameEnd == frameBegin)
        return;

    painter.setPen(QPen{palette.color(QPalette::Text)});
    painter.setBrush(palette.text());

    const auto framesPerPixel = std::distance(frameBegin, frameEnd) / size.width();

    for (int x = 0; x < size.width(); x++)
    {
        const frame_t *begin = frameBegin + (x * framesPerPixel);
        const frame_t *end = begin + framesPerPixel;

        frame_t min{1.f, 1.f}, max{-1.f, -1.f};
        for (auto iter = begin; iter != end; iter++)
        {
            if ((*iter)[0] < min[0])
                min[0] = (*iter)[0];
            if ((*iter)[1] < min[1])
                min[1] = (*iter)[1];
            if ((*iter)[0] > max[0])
                max[0] = (*iter)[0];
            if ((*iter)[1] > max[1])
                max[1] = (*iter)[1];
        }

        painter.drawLine(x, (size.height() / 2) - (min[0] * (size.height() / 2)),
                         x, (size.height() / 2) + (max[0] * (size.height() / 2)));
    }
}
