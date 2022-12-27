#include "presetdetailwidget.h"
#include "ui_presetdetailwidget.h"

PresetDetailWidget::PresetDetailWidget(QWidget *parent) :
    QScrollArea{parent},
    m_ui{std::make_unique<Ui::PresetDetailWidget>()}
{
    m_ui->setupUi(this);
}

PresetDetailWidget::~PresetDetailWidget() = default;

void PresetDetailWidget::setPreset(const presets::Preset &preset)
{
    // TODO
}
