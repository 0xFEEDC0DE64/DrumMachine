#include "filesmodel.h"

#include <iterator>

#include <QFont>
#include <QColor>

enum {
    ColumnFilename,
    ColumnColor,
    ColumnStopOnRelease,
    ColumnLooped,
    ColumnChoke,
    NumberOfColumns
};

FilesModel::~FilesModel() = default;

const presets::File &FilesModel::getFile(const QModelIndex &index) const
{
    return getFile(index.row());
}

const presets::File &FilesModel::getFile(int row) const
{
    return m_files->at(row);
}

void FilesModel::setPreset(const presets::Preset &preset)
{
    beginResetModel();
    m_files = preset.files;
    endResetModel();
}

int FilesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if (!m_files)
        return 0;

    return std::size(*m_files);
}

int FilesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return NumberOfColumns;
}

QVariant FilesModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::FontRole && role != Qt::ForegroundRole)
        return {};

    if (!m_files)
        return {};
    if (index.column() < 0)
        return {};
    if (index.column() >= NumberOfColumns)
        return {};
    if (index.row() < 0)
        return {};
    if (index.row() >= std::size(*m_files))
        return {};

    const auto &file = getFile(index);

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

    switch (index.column())
    {
    case ColumnFilename: return handleData(file.filename);
    case ColumnColor: return handleData(file.color);
    case ColumnStopOnRelease: return handleData(file.stopOnRelease);
    case ColumnLooped: return handleData(file.looped);
    case ColumnChoke: return handleData(file.choke);
    }

    Q_UNREACHABLE();
}

QVariant FilesModel::headerData(int section, Qt::Orientation orientation, int role) const
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
    case ColumnFilename: return tr("filename");
    case ColumnColor: return tr("color");
    case ColumnStopOnRelease: return tr("stopOnRelease");
    case ColumnLooped: return tr("looped");
    case ColumnChoke: return tr("choke");
    }

    Q_UNREACHABLE();
}
