#include "presetsmodel.h"

#include <iterator>

#include <QFont>
#include <QColor>

#include "presets.h"

enum {
    ColumnId,
    ColumnName,
    ColumnAuthor,
    ColumnOrderBy,
    ColumnVersion,
    ColumnTempo,
    ColumnIcon,
    ColumnPrice,
    ColumnPriceForSession,
    ColumnHasInfo,
    ColumnTags,
    ColumnDELETED,
    ColumnDifficulty,
    ColumnSample,
    ColumnAudioPreview1Name,
    ColumnAudioPreview1URL,
    ColumnAudioPreview2Name,
    ColumnAudioPreview2URL,
    ColumnImagePreview1,
    ColumnVideoPreview,
    ColumnVideoTutorial,
    NumberOfColumns
};

PresetsModel::PresetsModel(const std::map<QString, presets::Preset> &presets, QObject *parent) :
    QAbstractTableModel{parent}
{
    m_presets.reserve(std::size(presets));
    for (const auto &pair : presets)
        m_presets.emplace_back(pair.second);
}

PresetsModel::~PresetsModel() = default;

const presets::Preset &PresetsModel::getPreset(const QModelIndex &index) const
{
    return getPreset(index.row());
}

const presets::Preset &PresetsModel::getPreset(int row) const
{
    Q_ASSERT(row >= 0 && row < std::size(m_presets));
    return m_presets.at(row);
}

int PresetsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return std::size(m_presets);
}

int PresetsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return NumberOfColumns;
}

QVariant PresetsModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::FontRole && role != Qt::ForegroundRole)
        return {};

    if (index.column() < 0)
        return {};
    if (index.column() >= NumberOfColumns)
        return {};
    if (index.row() < 0)
        return {};
    if (index.row() >= std::size(m_presets))
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
    case ColumnId: return handleData(preset.id);
    case ColumnName: return handleData(preset.name);
    case ColumnAuthor: return handleData(preset.author);
    case ColumnOrderBy: return handleData(preset.orderBy);
    case ColumnVersion: return handleData(preset.version);
    case ColumnTempo: return handleData(preset.tempo);
    case ColumnIcon: return handleData(preset.icon);
    case ColumnPrice: return handleData(preset.price);
    case ColumnPriceForSession: return handleData(preset.priceForSession);
    case ColumnHasInfo: return handleData(preset.hasInfo);
    case ColumnTags: return handleStringVectorData(preset.tags);
    case ColumnDELETED: return handleData(preset.DELETED);
    case ColumnDifficulty: return handleData(preset.difficulty);
    case ColumnSample: return handleData(preset.sample);
    case ColumnAudioPreview1Name: return handleData(preset.audioPreview1Name);
    case ColumnAudioPreview1URL: return handleData(preset.audioPreview1URL);
    case ColumnAudioPreview2Name: return handleData(preset.audioPreview2Name);
    case ColumnAudioPreview2URL: return handleData(preset.audioPreview2URL);
    case ColumnImagePreview1: return handleData(preset.imagePreview1);
    case ColumnVideoPreview: return handleData(preset.videoPreview);
    case ColumnVideoTutorial: return handleData(preset.videoTutorial);
    }

    Q_UNREACHABLE();
}

QVariant PresetsModel::headerData(int section, Qt::Orientation orientation, int role) const
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
    case ColumnName: return tr("name");
    case ColumnAuthor: return tr("author");
    case ColumnOrderBy: return tr("orderBy");
    case ColumnVersion: return tr("version");
    case ColumnTempo: return tr("tempo");
    case ColumnIcon: return tr("icon");
    case ColumnPrice: return tr("price");
    case ColumnPriceForSession: return tr("priceForSession");
    case ColumnHasInfo: return tr("hasInfo");
    case ColumnTags: return tr("tags");
    case ColumnDELETED: return tr("DELETED");
    case ColumnDifficulty: return tr("difficulty");
    case ColumnSample: return tr("sample");
    case ColumnAudioPreview1Name: return tr("audioPreview1Name");
    case ColumnAudioPreview1URL: return tr("audioPreview1URL");
    case ColumnAudioPreview2Name: return tr("audioPreview2Name");
    case ColumnAudioPreview2URL: return tr("audioPreview2URL");
    case ColumnImagePreview1: return tr("imagePreview1");
    case ColumnVideoPreview: return tr("videoPreview");
    case ColumnVideoTutorial: return tr("videoTutorial");
    }

    Q_UNREACHABLE();
}
