#include "treetotableproxymodel.h"

void TreeToTableProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    if (m_sourceModel)
    {
        disconnect(m_sourceModel, &QAbstractItemModel::dataChanged, this, &TreeToTableProxyModel::sourceDataChanged);
        disconnect(m_sourceModel, &QAbstractItemModel::headerDataChanged, this, &QAbstractItemModel::headerDataChanged);

        disconnect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeInserted, this, &TreeToTableProxyModel::sourceRowsAboutToBeInserted);
        disconnect(m_sourceModel, &QAbstractItemModel::rowsInserted, this, &TreeToTableProxyModel::sourceRowsInserted);

        disconnect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &TreeToTableProxyModel::sourceRowsAboutToBeRemoved);
        disconnect(m_sourceModel, &QAbstractItemModel::rowsRemoved, this, &TreeToTableProxyModel::sourceRowsRemoved);

        disconnect(m_sourceModel, &QAbstractItemModel::columnsAboutToBeInserted, this, &TreeToTableProxyModel::sourceColumnsAboutToBeInserted);
        disconnect(m_sourceModel, &QAbstractItemModel::columnsInserted, this, &TreeToTableProxyModel::sourceColumnsInserted);

        disconnect(m_sourceModel, &QAbstractItemModel::columnsAboutToBeRemoved, this, &TreeToTableProxyModel::sourceColumnsAboutToBeRemoved);
        disconnect(m_sourceModel, &QAbstractItemModel::columnsRemoved, this, &TreeToTableProxyModel::sourceColumnsRemoved);

        disconnect(m_sourceModel, &QAbstractItemModel::modelAboutToBeReset, this, &QAbstractItemModel::modelAboutToBeReset);
        disconnect(m_sourceModel, &QAbstractItemModel::modelReset, this, &QAbstractItemModel::modelReset);

        disconnect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeMoved, this, &TreeToTableProxyModel::sourceRowsAboutToBeMoved);
        disconnect(m_sourceModel, &QAbstractItemModel::rowsMoved, this, &TreeToTableProxyModel::sourceRowsMoved);

        disconnect(m_sourceModel, &QAbstractItemModel::columnsAboutToBeMoved, this, &TreeToTableProxyModel::sourceColumnsAboutToBeMoved);
        disconnect(m_sourceModel, &QAbstractItemModel::columnsMoved, this, &TreeToTableProxyModel::sourceColumnsMoved);
    }

    beginResetModel();
    m_sourceModel = sourceModel;
    m_rootIndex = {};
    endResetModel();

    if (m_sourceModel)
    {
        connect(m_sourceModel, &QAbstractItemModel::dataChanged, this, &TreeToTableProxyModel::sourceDataChanged);
        connect(m_sourceModel, &QAbstractItemModel::headerDataChanged, this, &QAbstractItemModel::headerDataChanged);

        connect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeInserted, this, &TreeToTableProxyModel::sourceRowsAboutToBeInserted);
        connect(m_sourceModel, &QAbstractItemModel::rowsInserted, this, &TreeToTableProxyModel::sourceRowsInserted);

        connect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &TreeToTableProxyModel::sourceRowsAboutToBeRemoved);
        connect(m_sourceModel, &QAbstractItemModel::rowsRemoved, this, &TreeToTableProxyModel::sourceRowsRemoved);

        connect(m_sourceModel, &QAbstractItemModel::columnsAboutToBeInserted, this, &TreeToTableProxyModel::sourceColumnsAboutToBeInserted);
        connect(m_sourceModel, &QAbstractItemModel::columnsInserted, this, &TreeToTableProxyModel::sourceColumnsInserted);

        connect(m_sourceModel, &QAbstractItemModel::columnsAboutToBeRemoved, this, &TreeToTableProxyModel::sourceColumnsAboutToBeRemoved);
        connect(m_sourceModel, &QAbstractItemModel::columnsRemoved, this, &TreeToTableProxyModel::sourceColumnsRemoved);

        connect(m_sourceModel, &QAbstractItemModel::modelAboutToBeReset, this, &QAbstractItemModel::modelAboutToBeReset);
        connect(m_sourceModel, &QAbstractItemModel::modelReset, this, &QAbstractItemModel::modelReset);

        connect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeMoved, this, &TreeToTableProxyModel::sourceRowsAboutToBeMoved);
        connect(m_sourceModel, &QAbstractItemModel::rowsMoved, this, &TreeToTableProxyModel::sourceRowsMoved);

        connect(m_sourceModel, &QAbstractItemModel::columnsAboutToBeMoved, this, &TreeToTableProxyModel::sourceColumnsAboutToBeMoved);
        connect(m_sourceModel, &QAbstractItemModel::columnsMoved, this, &TreeToTableProxyModel::sourceColumnsMoved);
    }
}

void TreeToTableProxyModel::setSourceModelAndRootIndex(QAbstractItemModel *sourceModel, const QModelIndex &index)
{
    Q_ASSERT(index.model() == sourceModel);

    if (m_sourceModel)
    {
        disconnect(m_sourceModel, &QAbstractItemModel::dataChanged, this, &TreeToTableProxyModel::sourceDataChanged);
        disconnect(m_sourceModel, &QAbstractItemModel::headerDataChanged, this, &QAbstractItemModel::headerDataChanged);

        disconnect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeInserted, this, &TreeToTableProxyModel::sourceRowsAboutToBeInserted);
        disconnect(m_sourceModel, &QAbstractItemModel::rowsInserted, this, &TreeToTableProxyModel::sourceRowsInserted);

        disconnect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &TreeToTableProxyModel::sourceRowsAboutToBeRemoved);
        disconnect(m_sourceModel, &QAbstractItemModel::rowsRemoved, this, &TreeToTableProxyModel::sourceRowsRemoved);

        disconnect(m_sourceModel, &QAbstractItemModel::columnsAboutToBeInserted, this, &TreeToTableProxyModel::sourceColumnsAboutToBeInserted);
        disconnect(m_sourceModel, &QAbstractItemModel::columnsInserted, this, &TreeToTableProxyModel::sourceColumnsInserted);

        disconnect(m_sourceModel, &QAbstractItemModel::columnsAboutToBeRemoved, this, &TreeToTableProxyModel::sourceColumnsAboutToBeRemoved);
        disconnect(m_sourceModel, &QAbstractItemModel::columnsRemoved, this, &TreeToTableProxyModel::sourceColumnsRemoved);

        disconnect(m_sourceModel, &QAbstractItemModel::modelAboutToBeReset, this, &QAbstractItemModel::modelAboutToBeReset);
        disconnect(m_sourceModel, &QAbstractItemModel::modelReset, this, &QAbstractItemModel::modelReset);

        disconnect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeMoved, this, &TreeToTableProxyModel::sourceRowsAboutToBeMoved);
        disconnect(m_sourceModel, &QAbstractItemModel::rowsMoved, this, &TreeToTableProxyModel::sourceRowsMoved);

        disconnect(m_sourceModel, &QAbstractItemModel::columnsAboutToBeMoved, this, &TreeToTableProxyModel::sourceColumnsAboutToBeMoved);
        disconnect(m_sourceModel, &QAbstractItemModel::columnsMoved, this, &TreeToTableProxyModel::sourceColumnsMoved);
    }

    beginResetModel();
    m_sourceModel = sourceModel;
    m_rootIndex = index;
    endResetModel();

    if (m_sourceModel)
    {
        connect(m_sourceModel, &QAbstractItemModel::dataChanged, this, &TreeToTableProxyModel::sourceDataChanged);
        connect(m_sourceModel, &QAbstractItemModel::headerDataChanged, this, &QAbstractItemModel::headerDataChanged);

        connect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeInserted, this, &TreeToTableProxyModel::sourceRowsAboutToBeInserted);
        connect(m_sourceModel, &QAbstractItemModel::rowsInserted, this, &TreeToTableProxyModel::sourceRowsInserted);

        connect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &TreeToTableProxyModel::sourceRowsAboutToBeRemoved);
        connect(m_sourceModel, &QAbstractItemModel::rowsRemoved, this, &TreeToTableProxyModel::sourceRowsRemoved);

        connect(m_sourceModel, &QAbstractItemModel::columnsAboutToBeInserted, this, &TreeToTableProxyModel::sourceColumnsAboutToBeInserted);
        connect(m_sourceModel, &QAbstractItemModel::columnsInserted, this, &TreeToTableProxyModel::sourceColumnsInserted);

        connect(m_sourceModel, &QAbstractItemModel::columnsAboutToBeRemoved, this, &TreeToTableProxyModel::sourceColumnsAboutToBeRemoved);
        connect(m_sourceModel, &QAbstractItemModel::columnsRemoved, this, &TreeToTableProxyModel::sourceColumnsRemoved);

        connect(m_sourceModel, &QAbstractItemModel::modelAboutToBeReset, this, &QAbstractItemModel::modelAboutToBeReset);
        connect(m_sourceModel, &QAbstractItemModel::modelReset, this, &QAbstractItemModel::modelReset);

        connect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeMoved, this, &TreeToTableProxyModel::sourceRowsAboutToBeMoved);
        connect(m_sourceModel, &QAbstractItemModel::rowsMoved, this, &TreeToTableProxyModel::sourceRowsMoved);

        connect(m_sourceModel, &QAbstractItemModel::columnsAboutToBeMoved, this, &TreeToTableProxyModel::sourceColumnsAboutToBeMoved);
        connect(m_sourceModel, &QAbstractItemModel::columnsMoved, this, &TreeToTableProxyModel::sourceColumnsMoved);
    }
}

void TreeToTableProxyModel::setRootIndex(const QModelIndex &index)
{
    if (!index.isValid())
        return;
    Q_ASSERT(index.model() == m_sourceModel);

    beginResetModel();
    m_rootIndex = index;
    endResetModel();
}

QModelIndex TreeToTableProxyModel::mapFromSource(const QModelIndex &index) const
{
    Q_ASSERT(m_sourceModel);
    Q_ASSERT(m_rootIndex.isValid());
    Q_ASSERT(index.parent() == m_rootIndex);

    return createIndex(index.row(), index.column());
}

QModelIndex TreeToTableProxyModel::mapToSource(const QModelIndex &index) const
{
    Q_ASSERT(m_sourceModel);
    Q_ASSERT(m_rootIndex.isValid());
    Q_ASSERT(index.model() == this);

    return m_sourceModel->index(index.row(), index.column(), m_rootIndex);
}

int TreeToTableProxyModel::rowCount(const QModelIndex &parent) const
{
    Q_ASSERT(!parent.isValid());

    if (m_sourceModel && m_rootIndex.isValid())
        return m_sourceModel->rowCount(m_rootIndex);
    else
        return 0;
}

int TreeToTableProxyModel::columnCount(const QModelIndex &parent) const
{
    Q_ASSERT(!parent.isValid());

    if (m_sourceModel && m_rootIndex.isValid())
        return m_sourceModel->columnCount(m_rootIndex);
    else
        return 0;
}

QVariant TreeToTableProxyModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.model() == this);

    if (m_sourceModel && m_rootIndex.isValid())
        return m_sourceModel->data(mapToSource(index), role);
    else
        return {};
}

QVariant TreeToTableProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (m_sourceModel)
        return m_sourceModel->headerData(section, orientation, role);
    else
        return {};
}

void TreeToTableProxyModel::fetchMore(const QModelIndex &parent)
{
    Q_ASSERT(!parent.isValid());

    if (m_sourceModel && m_rootIndex.isValid())
        m_sourceModel->data(m_rootIndex);
}

bool TreeToTableProxyModel::canFetchMore(const QModelIndex &parent) const
{
    Q_ASSERT(!parent.isValid());

    if (m_sourceModel && m_rootIndex.isValid())
        return m_sourceModel->canFetchMore(m_rootIndex);
    else
        return {};
}

Qt::ItemFlags TreeToTableProxyModel::flags(const QModelIndex &index) const
{
    Q_ASSERT(index.model() == this);

    if (m_sourceModel && m_rootIndex.isValid())
        return m_sourceModel->flags(mapToSource(index));
    else
        return QAbstractTableModel::flags(index);
}

void TreeToTableProxyModel::sourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if (topLeft.parent() != m_rootIndex)
        return;

    emit dataChanged(mapFromSource(topLeft), mapFromSource(bottomRight), roles);
}

void TreeToTableProxyModel::sourceRowsAboutToBeInserted(const QModelIndex &parent, int first, int last)
{
    if (parent != m_rootIndex)
        return;

    beginInsertRows({}, first, last);
}

void TreeToTableProxyModel::sourceRowsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(first)
    Q_UNUSED(last)

    if (parent != m_rootIndex)
        return;

    endInsertRows();
}

void TreeToTableProxyModel::sourceRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    if (parent != m_rootIndex)
        return;

    beginRemoveRows({}, first, last);
}

void TreeToTableProxyModel::sourceRowsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(first)
    Q_UNUSED(last)

    if (parent != m_rootIndex)
        return;

    endRemoveRows();
}

void TreeToTableProxyModel::sourceColumnsAboutToBeInserted(const QModelIndex &parent, int first, int last)
{
    if (parent != m_rootIndex)
        return;

    beginInsertColumns({}, first, last);
}

void TreeToTableProxyModel::sourceColumnsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(first)
    Q_UNUSED(last)

    if (parent != m_rootIndex)
        return;

    endInsertColumns();
}

void TreeToTableProxyModel::sourceColumnsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    if (parent != m_rootIndex)
        return;

    beginRemoveColumns({}, first, last);
}

void TreeToTableProxyModel::sourceColumnsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(first)
    Q_UNUSED(last)

    if (parent != m_rootIndex)
        return;

    endRemoveColumns();
}

void TreeToTableProxyModel::sourceRowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow)
{
    Q_UNUSED(sourceParent)
    Q_UNUSED(sourceStart)
    Q_UNUSED(sourceEnd)
    Q_UNUSED(destinationParent)
    Q_UNUSED(destinationRow)
}

void TreeToTableProxyModel::sourceRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row)
{
    Q_UNUSED(parent)
    Q_UNUSED(start)
    Q_UNUSED(end)
    Q_UNUSED(destination)
    Q_UNUSED(row)
}

void TreeToTableProxyModel::sourceColumnsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationColumn)
{
    Q_UNUSED(sourceParent)
    Q_UNUSED(sourceStart)
    Q_UNUSED(sourceEnd)
    Q_UNUSED(destinationParent)
    Q_UNUSED(destinationColumn)
}

void TreeToTableProxyModel::sourceColumnsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int column)
{
    Q_UNUSED(parent)
    Q_UNUSED(start)
    Q_UNUSED(end)
    Q_UNUSED(destination)
    Q_UNUSED(column)
}
