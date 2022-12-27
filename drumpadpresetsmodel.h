#pragma once

#include <vector>

#include <QAbstractTableModel>

namespace drumpad_presets { class Preset; }

class DrumPadPresetsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    DrumPadPresetsModel(QObject *parent = nullptr);
    DrumPadPresetsModel(const std::map<QString, drumpad_presets::Preset> &presets, QObject *parent = nullptr);
    DrumPadPresetsModel(std::vector<drumpad_presets::Preset> &&presets, QObject *parent = nullptr);
    DrumPadPresetsModel(const std::vector<drumpad_presets::Preset> &presets, QObject *parent = nullptr);
    ~DrumPadPresetsModel() override;

    void setPresets(const std::map<QString, drumpad_presets::Preset> &presets);
    void setPresets(std::vector<drumpad_presets::Preset> &&presets);
    void setPresets(const std::vector<drumpad_presets::Preset> &presets);

    const drumpad_presets::Preset &getPreset(const QModelIndex &index) const;
    const drumpad_presets::Preset &getPreset(int row) const;

    QModelIndex findPresetById(const QString &id) const;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    std::vector<drumpad_presets::Preset> m_presets;
};
