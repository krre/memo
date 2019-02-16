#pragma once
#include <QTreeView>

class QMenu;
class TreeModel;

class Outliner : public QTreeView {
    Q_OBJECT

public:
    Outliner();

public slots:
    void updateActions();

private slots:
    void onCustomContextMenu(const QPoint& point);
    void addNote();
    void removeNote();
    void renameNote();

private:
    void createContextMenu();
    void insertChild(const QString& title);

    QMenu* contextMenu;
    TreeModel* model;
};
