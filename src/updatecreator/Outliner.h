#pragma once
#include <QWidget>

class QListWidget;

class Outliner : public QWidget {
    Q_OBJECT
public:
    explicit Outliner(QWidget* parent = nullptr);

private slots:
    void addUpdate();
    void removeUpdate();

private:
    QListWidget* listWidget = nullptr;
};
