#include "scratchwidget.h"

#include <QPainter>

#include "graphrenderer.h"

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
