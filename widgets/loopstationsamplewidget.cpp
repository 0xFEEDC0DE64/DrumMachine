#include "loopstationsamplewidget.h"
#include "ui_loopstationsamplewidget.h"

LoopStationSampleWidget::LoopStationSampleWidget(QWidget *parent) :
    QFrame{parent},
    m_ui{std::make_unique<Ui::LoopStationSampleWidget>()}
{
    m_ui->setupUi(this);
}

LoopStationSampleWidget::~LoopStationSampleWidget() = default;
