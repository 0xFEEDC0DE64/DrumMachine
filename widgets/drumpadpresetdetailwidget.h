#pragma once

#include <memory>

#include <QScrollArea>

#include "drumpadpresettagsmodel.h"

namespace Ui { class DrumPadPresetDetailWidget; }
namespace drumpad_presets { struct Preset; }

class DrumPadPresetDetailWidget : public QScrollArea
{
    Q_OBJECT

public:
    explicit DrumPadPresetDetailWidget(QWidget *parent = nullptr);
    ~DrumPadPresetDetailWidget() override;

    void setPreset(const drumpad_presets::Preset &preset);

private:
    const std::unique_ptr<Ui::DrumPadPresetDetailWidget> m_ui;

    DrumPadPresetTagsModel m_tagsModel;
};
