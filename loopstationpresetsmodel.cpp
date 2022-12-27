#include "loopstationpresetsmodel.h"

#include <iterator>

#include <QFont>
#include <QColor>

#include "loopstationpresets.h"

enum {
    ColumnId,
    ColumnTitle,
    ColumnAuthor,
    ColumnBpm,
    ColumnCoverUrl,
    ColumnLoopLength,
    ColumnOrderBy,
    ColumnPremium,
    ColumnTags,
    ColumnAudioPreviewUrl,
    ColumnDELETED,
    NumberOfColumns
};

LoopStationPresetsModel::LoopStationPresetsModel(QObject *parent) :
    QAbstractTableModel{parent}
{
}

LoopStationPresetsModel::LoopStationPresetsModel(const std::map<QString, loopstation_presets::Preset> &presets, QObject *parent) :
    QAbstractTableModel{parent}
{
    m_presets.reserve(std::size(presets));
    for (const auto &pair : presets)
        m_presets.emplace_back(pair.second);
}

LoopStationPresetsModel::LoopStationPresetsModel(std::vector<loopstation_presets::Preset> &&presets, QObject *parent) :
    QAbstractTableModel{parent}
{
    m_presets = std::move(presets);
}

LoopStationPresetsModel::LoopStationPresetsModel(const std::vector<loopstation_presets::Preset> &presets, QObject *parent) :
    QAbstractTableModel{parent}
{
    m_presets = presets;
}

LoopStationPresetsModel::~LoopStationPresetsModel() = default;

void LoopStationPresetsModel::setPresets(const std::map<QString, loopstation_presets::Preset> &presets)
{
    beginResetModel();
    m_presets.clear();
    m_presets.reserve(std::size(presets));
    for (const auto &pair : presets)
        m_presets.emplace_back(pair.second);
    endResetModel();
}

void LoopStationPresetsModel::setPresets(std::vector<loopstation_presets::Preset> &&presets)
{
    beginResetModel();
    m_presets = std::move(presets);
    endResetModel();
}

void LoopStationPresetsModel::setPresets(const std::vector<loopstation_presets::Preset> &presets)
{
    beginResetModel();
    m_presets = presets;
    endResetModel();
}

const loopstation_presets::Preset &LoopStationPresetsModel::getPreset(const QModelIndex &index) const
{
    return getPreset(index.row());
}

const loopstation_presets::Preset &LoopStationPresetsModel::getPreset(int row) const
{
    Q_ASSERT(row >= 0 && row < int(std::size(m_presets)));
    return m_presets.at(row);
}

QModelIndex LoopStationPresetsModel::findPresetById(const QString &id) const
{
    for (auto iter = std::cbegin(m_presets); iter != std::cend(m_presets); iter++)
    {
        if (iter->id != id)
            continue;

        return createIndex(std::distance(std::cbegin(m_presets), iter), 0);
    }

    return {};
}

int LoopStationPresetsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return std::size(m_presets);
}

int LoopStationPresetsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return NumberOfColumns;
}

QVariant LoopStationPresetsModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::FontRole && role != Qt::ForegroundRole)
        return {};

    if (index.column() < 0)
        return {};
    if (index.column() >= NumberOfColumns)
        return {};
    if (index.row() < 0)
        return {};
    if (index.row() >= int(std::size(m_presets)))
        return {};

    const auto &preset = getPreset(index);

    const auto handleData = [&](const auto &val) -> QVariant
    {
        if (!val)
        {
            if (role == Qt::DisplayRole)
                return this->tr("(null)");
            else if (role == Qt::FontRole)
            {
                QFont font;
                font.setItalic(true);
                return font;
            }
            else if (role == Qt::ForegroundRole)
                return QColor{Qt::gray};
            return {};
        }

        if (role == Qt::DisplayRole || role == Qt::EditRole)
            return *val;

        return {};
    };

    const auto handleStringVectorData = [&](const auto &val) -> QVariant
    {
        if (!val)
        {
            if (role == Qt::DisplayRole)
                return this->tr("(null)");
            else if (role == Qt::FontRole)
            {
                QFont font;
                font.setItalic(true);
                return font;
            }
            else if (role == Qt::ForegroundRole)
                return QColor{Qt::gray};
            return {};
        }

        if (role == Qt::DisplayRole || role == Qt::EditRole)
        {
            QString text;
            for (auto iter = std::cbegin(*val); iter != std::cend(*val); iter++)
            {
                if (iter != std::cbegin(*val))
                    text += ", ";
                text += *iter;
            }
            return text;
        }

        return {};
    };

    switch (index.column())
    {
    case ColumnId:
    {
        if (preset.id)
        {
            bool ok;
            if (auto id = preset.id->toInt(&ok); ok)
                return id;
        }

        return handleData(preset.id);
    }
    case ColumnTitle: return handleData(preset.title);
    case ColumnAuthor: return handleData(preset.author);
    case ColumnBpm: return handleData(preset.bpm);
    case ColumnCoverUrl: return handleData(preset.coverUrl);
    case ColumnLoopLength: return handleData(preset.loopLength);
    case ColumnOrderBy: return handleData(preset.orderBy);
    case ColumnPremium: return handleData(preset.premium);
    case ColumnTags: return handleStringVectorData(preset.tags);
    case ColumnAudioPreviewUrl: return handleData(preset.audioPreviewUrl);
    case ColumnDELETED: return handleData(preset.DELETED);
    }

    Q_UNREACHABLE();
}

QVariant LoopStationPresetsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return {};

    if (orientation != Qt::Horizontal)
        return {};

    if (section < 0)
        return {};
    if (section >= NumberOfColumns)
        return {};

    switch (section)
    {
    case ColumnId: return tr("id");
    case ColumnTitle: return tr("title");
    case ColumnAuthor: return tr("author");
    case ColumnBpm: return tr("bpm");
    case ColumnCoverUrl: return tr("coverUrl");
    case ColumnLoopLength: return tr("loopLength");
    case ColumnOrderBy: return tr("orderBy");
    case ColumnPremium: return tr("premium");
    case ColumnTags: return tr("tags");
    case ColumnAudioPreviewUrl: return tr("audioPreviewUrl");
    case ColumnDELETED: return tr("DELETED");
    }

    Q_UNREACHABLE();
}
