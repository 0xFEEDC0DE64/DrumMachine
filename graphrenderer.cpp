#include "graphrenderer.h"

#include <iterator>

#include <QPixmap>
#include <QPainter>
#include <QPalette>
#include <QRect>
#include <QPoint>

#include "audioformat.h"

QPixmap GraphRenderer::render(const QSize &size, const frame_t *begin, const frame_t *end, const QPalette &palette)
{
    QPixmap pixmap{size};

    {
        QPainter painter;
        painter.begin(&pixmap);

        painter.setPen({});
        painter.setBrush(palette.base());
        painter.drawRect(pixmap.rect());

        painter.setPen(QPen{palette.color(QPalette::Text)});
        painter.setBrush(palette.text());
        render(pixmap.rect(), begin, end, painter);

        painter.end();
    }

    return pixmap;
}

void GraphRenderer::render(const QRect &rect, const frame_t *begin, const frame_t *end, QPainter &painter)
{
    if (end == begin)
        return;

    std::pair<frame_t, frame_t> arr[rect.width()];

    reduceSamples(begin, end, arr, arr+rect.width());

    render(rect.topLeft(), rect.height(), arr, arr+rect.width(), painter);
}

void GraphRenderer::render(const QPoint &pos, int height, std::pair<frame_t, frame_t> *begin, std::pair<frame_t, frame_t> *end, QPainter &painter)
{
    int x = pos.x();
    for (auto iter = begin; iter != end; iter++)
    {
        const frame_t &min = iter->first;
        const frame_t &max = iter->second;

        painter.drawLine(x, pos.y() + (height / 2) - (min[0] * (height / 2)),
                         x, pos.y() + (height / 2) + (max[0] * (height / 2)));
        x++;
    }
}

void GraphRenderer::reduceSamples(const frame_t *inputBegin, const frame_t *inputEnd, std::pair<frame_t, frame_t> *outputBegin, std::pair<frame_t, frame_t> *outputEnd)
{
    const auto inputLength = std::distance(inputBegin, inputEnd);
    const auto outputLength = std::distance(outputBegin, outputEnd);

    const auto framesPerPixel = inputLength / outputLength;

    for (int i = 0; i < outputLength; i++)
    {
        frame_t &min = outputBegin[i].first;
        frame_t &max = outputBegin[i].second;

        min = {1.f, 1.f};
        max = {-1.f, -1.f};

        const frame_t *begin = inputBegin + (i * framesPerPixel);
        const frame_t *end = begin + framesPerPixel;

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
    }
}
