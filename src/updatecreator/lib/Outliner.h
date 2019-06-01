#pragma once
#include <QWidget>

class QListView;
class QPushButton;
class ListModel;

class Outliner : public QWidget {
    Q_OBJECT
public:
    explicit Outliner(ListModel* model, QWidget* parent = nullptr);
    void selectRow(int row);
    int currentRow() const;
    void setAddButtonEnabled(bool enabled);

signals:
    void addClicked();
    void removeClicked(int row);
    void selectionChanged(int selectedRow, int deselectedRow);

private slots:
    void removeUpdate();

private:
    QListView* listView = nullptr;
    QPushButton* addButton = nullptr;
};
