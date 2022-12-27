#include "drumpadpresettagsmodel.h"

DrumPadPresetTagsModel::DrumPadPresetTagsModel(QObject *parent) :
    QAbstractListModel{parent}
{
}

void DrumPadPresetTagsModel::setPreset(const drumpad_presets::Preset &preset)
{
    beginResetModel();
    if (preset.tags)
        m_tags = *preset.tags;
    else
        m_tags.clear();
    endResetModel();
}

int DrumPadPresetTagsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_tags.size();
}

QVariant DrumPadPresetTagsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    if (index.row() < 0 || index.row() >= int(m_tags.size()))
        return {};

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return m_tags[index.row()];
    }

    return {};
}
