#pragma once
#include <QTreeView>

class QMenu;

class Outliner : public QTreeView {
    Q_OBJECT

public:
    Outliner();

private slots:
    void onCustomContextMenu(const QPoint& point);
    void addNote();

private:
    void createContextMenu();

    QMenu* contextMenu;
};
