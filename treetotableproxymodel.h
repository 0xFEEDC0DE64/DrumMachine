#pragma once

#include <QAbstractTableModel>

class TreeToTableProxyModel : public QAbstractTableModel
{
public:
    using QAbstractTableModel::QAbstractTableModel;

    void setSourceModel(QAbstractItemModel *sourceModel);
    void setSourceModelAndRootIndex(QAbstractItemModel *sourceModel, const QModelIndex &index);
    void setRootIndex(const QModelIndex &index);

    QModelIndex mapFromSource(const QModelIndex &index) const;
    QModelIndex mapToSource(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void fetchMore(const QModelIndex &parent) override;
    bool canFetchMore(const QModelIndex &parent) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private slots:
    void sourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());

    void sourceRowsAboutToBeInserted(const QModelIndex &parent, int first, int last);
    void sourceRowsInserted(const QModelIndex &parent, int first, int last);

    void sourceRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void sourceRowsRemoved(const QModelIndex &parent, int first, int last);

    void sourceColumnsAboutToBeInserted(const QModelIndex &parent, int first, int last);
    void sourceColumnsInserted(const QModelIndex &parent, int first, int last);

    void sourceColumnsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void sourceColumnsRemoved(const QModelIndex &parent, int first, int last);

    void sourceRowsAboutToBeMoved( const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow);
    void sourceRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);

    void sourceColumnsAboutToBeMoved( const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationColumn);
    void sourceColumnsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int column);

private:
    const QAbstractItemModel *m_sourceModel{};
    QModelIndex m_rootIndex;
};
