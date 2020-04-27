#pragma once

class QPixmap;
class QSize;
class QPainter;
class QPalette;

#include "audioformat.h"

namespace GraphRenderer
{
    QPixmap render(const QSize &size, const frame_t *frameBegin, const frame_t *frameEnd, const QPalette &palette);
    void render(const QSize &size, const frame_t *frameBegin, const frame_t *frameEnd, QPainter &painter, const QPalette &palette);
}
