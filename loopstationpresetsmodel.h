#pragma once

#include <vector>

#include <QAbstractTableModel>

namespace loopstation_presets { class Preset; }

class LoopStationPresetsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    LoopStationPresetsModel(QObject *parent = nullptr);
    LoopStationPresetsModel(const std::map<QString, loopstation_presets::Preset> &presets, QObject *parent = nullptr);
    LoopStationPresetsModel(std::vector<loopstation_presets::Preset> &&presets, QObject *parent = nullptr);
    LoopStationPresetsModel(const std::vector<loopstation_presets::Preset> &presets, QObject *parent = nullptr);
    ~LoopStationPresetsModel() override;

    void setPresets(const std::map<QString, loopstation_presets::Preset> &presets);
    void setPresets(std::vector<loopstation_presets::Preset> &&presets);
    void setPresets(const std::vector<loopstation_presets::Preset> &presets);

    const loopstation_presets::Preset &getPreset(const QModelIndex &index) const;
    const loopstation_presets::Preset &getPreset(int row) const;

    QModelIndex findPresetById(const QString &id) const;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    std::vector<loopstation_presets::Preset> m_presets;
};
