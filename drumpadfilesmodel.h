#pragma once

#include <array>

#include <QAbstractTableModel>

#include "drumpadpresets.h"

namespace drumpad_presets { struct Preset; struct File; }

class DrumPadFilesModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    using QAbstractTableModel::QAbstractTableModel;
    ~DrumPadFilesModel() override;

    const drumpad_presets::File &getFile(const QModelIndex &index) const;
    const drumpad_presets::File &getFile(int row) const;

    void setPreset(const drumpad_presets::Preset &preset);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    std::optional<std::array<drumpad_presets::File, 24>> m_files;
};
