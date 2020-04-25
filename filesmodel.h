#pragma once

#include <array>

#include <QAbstractTableModel>

#include "presets.h"

namespace presets { class Preset; class File; }

class FilesModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    using QAbstractTableModel::QAbstractTableModel;
    ~FilesModel() override;

    const presets::File &getFile(const QModelIndex &index) const;
    const presets::File &getFile(int row) const;

    void setPreset(const presets::Preset &preset);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    std::optional<std::array<presets::File, 24>> m_files;
};
