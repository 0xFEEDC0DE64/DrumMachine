#pragma once

#include <QScrollArea>

#include <memory>

#include "loopstationpresettagsmodel.h"

namespace Ui { class LoopStationPresetDetailWidget; }
namespace loopstation_presets { struct Preset; }

class LoopStationPresetDetailWidget : public QScrollArea
{
    Q_OBJECT

public:
    explicit LoopStationPresetDetailWidget(QWidget *parent = nullptr);
    ~LoopStationPresetDetailWidget() override;

    void setPreset(const loopstation_presets::Preset &preset);

private:
    const std::unique_ptr<Ui::LoopStationPresetDetailWidget> m_ui;

    LoopStationPresetTagsModel m_tagsModel;
};
