#pragma once

#include <memory>

#include <QScrollArea>

namespace Ui { class PresetDetailWidget; }
namespace presets { class Preset; }

class PresetDetailWidget : public QScrollArea
{
    Q_OBJECT

public:
    explicit PresetDetailWidget(QWidget *parent = nullptr);
    ~PresetDetailWidget() override;

    void setPreset(const presets::Preset &preset);

private:
    const std::unique_ptr<Ui::PresetDetailWidget> m_ui;
};
