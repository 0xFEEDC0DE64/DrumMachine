#pragma once

#include <vector>

#include <QAbstractTableModel>

namespace presets { class Preset; }

class PresetsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    PresetsModel(QObject *parent = nullptr);
    PresetsModel(const std::map<QString, presets::Preset> &presets, QObject *parent = nullptr);
    PresetsModel(std::vector<presets::Preset> &&presets, QObject *parent = nullptr);
    PresetsModel(const std::vector<presets::Preset> &presets, QObject *parent = nullptr);
    ~PresetsModel() override;

    void setPresets(const std::map<QString, presets::Preset> &presets);
    void setPresets(std::vector<presets::Preset> &&presets);
    void setPresets(const std::vector<presets::Preset> &presets);

    const presets::Preset &getPreset(const QModelIndex &index) const;
    const presets::Preset &getPreset(int row) const;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    std::vector<presets::Preset> m_presets;
};
