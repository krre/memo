#pragma once
#include <QWidget>

class QListView;
class ListModel;

class Outliner : public QWidget {
    Q_OBJECT
public:
    explicit Outliner(ListModel* model, QWidget* parent = nullptr);
    void selectRow(int row);

signals:
    void addClicked();
    void removeClicked(int row);
    void selectionChanged(int selectedRow, int deselectedRow);

private slots:
    void removeUpdate();

private:
    QListView* listView = nullptr;
};
