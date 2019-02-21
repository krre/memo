#pragma once
#include <QTreeView>

class QMenu;
class QAction;
class TreeModel;
class Database;

class Outliner : public QTreeView {
    Q_OBJECT

public:
    Outliner(Database* database);

public slots:
    void updateActions();
    void build();
    void clear();

signals:
    void noteChanged(int id);

protected:
    void mousePressEvent(QMouseEvent* event) override;

protected slots:
    void currentChanged(const QModelIndex& current, const QModelIndex& previous) override;

private slots:
    void onCustomContextMenu(const QPoint& point);
    void addNote();
    void removeNotes();
    void renameNote();

private:
    void createContextMenu();
    void updateContextMenu();
    void insertChild(const QString& title);

    QMenu* contextMenu;
    QAction* removeAction;
    QAction* renameAction;
    TreeModel* model;
    Database* database;
};
