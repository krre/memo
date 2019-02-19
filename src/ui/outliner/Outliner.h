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

signals:
    void noteAdded(int parent, int pos, const QString& title);

protected:
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onCustomContextMenu(const QPoint& point);
    void addNote();
    void removeNote();
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
