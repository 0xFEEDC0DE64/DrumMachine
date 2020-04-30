#pragma once

class QPixmap;
class QSize;
class QPainter;
class QPalette;

#include "audioformat.h"

namespace GraphRenderer
{
    QPixmap render(const QSize &size, const frame_t *begin, const frame_t *end, const QPalette &palette);
    void render(const QRect &rect, const frame_t *begin, const frame_t *end, QPainter &painter);
    void render(const QPoint &pos, int height, std::pair<frame_t, frame_t> *begin, std::pair<frame_t, frame_t> *end, QPainter &painter);
    void reduceSamples(const frame_t *inputBegin, const frame_t *inputEnd, std::pair<frame_t, frame_t> *outputBegin, std::pair<frame_t, frame_t> *outputEnd);
}
