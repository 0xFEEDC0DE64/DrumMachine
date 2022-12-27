#pragma once

#include <QAbstractListModel>

#include "drumpadpresets.h"

namespace drumpad_presets { struct Preset; }

class DrumPadPresetTagsModel : public QAbstractListModel
{
public:
    DrumPadPresetTagsModel(QObject *parent = nullptr);

    void setPreset(const drumpad_presets::Preset &preset);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    std::vector<QString> m_tags;
};
