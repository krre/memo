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

    void exportAllNotes(const QString& directory);

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
    void moveUp();
    void moveDown();
    void moveTree(const QModelIndex& index);
    void showProperties();

private:
    void createContextMenu();
    void updateContextMenu();
    void insertChild(const QString& title);
    int exportNote(int parentId, const QString& path);

    QMenu* contextMenu;
    QAction* removeAction;
    QAction* renameAction;
    QAction* moveUpAction;
    QAction* moveDownAction;
    QAction* propertiesAction;
    TreeModel* model = nullptr;
    Database* database = nullptr;
    bool isInited = false;
};
