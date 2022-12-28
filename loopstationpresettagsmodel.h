#pragma once

#include <QAbstractListModel>

namespace loopstation_presets { struct Preset; }

class LoopStationPresetTagsModel : public QAbstractListModel
{
public:
    LoopStationPresetTagsModel(QObject *parent = nullptr);

    void setPreset(const loopstation_presets::Preset &preset);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    std::vector<QString> m_tags;
};
