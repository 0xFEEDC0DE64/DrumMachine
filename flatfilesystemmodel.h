#pragma once

#include <QFileSystemModel>

class FlatFileSystemModel : public QFileSystemModel
{
public:
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override { return (parent == m_root_path) ? QFileSystemModel::hasChildren(parent) : false; };
    bool canFetchMore(const QModelIndex &parent) const override { return (parent == m_root_path) ? QFileSystemModel::canFetchMore(parent) : false; };
    void fetchMore(const QModelIndex &parent) override { if ((parent == m_root_path)) QFileSystemModel::fetchMore(parent); };

public:
    QModelIndex m_root_path;
};
