#pragma once
#include <QTreeView>
#include "core/Id.h"

class QMenu;
class QAction;
class TreeModel;
class Database;

class NoteTaking : public QTreeView {
    Q_OBJECT
public:
    NoteTaking(Database* database);

    int exportNote(Id parentId, const QString& path) const;
    void setCurrentId(Id id);

public slots:
    void build();
    void clear();

signals:
    void noteChanged(Id id);

protected:
    void mousePressEvent(QMouseEvent* event) override;

protected slots:
    void currentChanged(const QModelIndex& current, const QModelIndex& previous) override;

private slots:
    void onCustomContextMenu(const QPoint& point);
    void addNote();
    void removeNotes();
    void renameNote();
    void moveUp();
    void moveDown();
    void moveTree(const QModelIndex& index);
    void expandTree();
    void showProperties() const;

private:
    void insertChild(const QString& title);

    QScopedPointer<TreeModel> m_model;
    Database* m_database = nullptr;
    bool m_isInited = false;
};
