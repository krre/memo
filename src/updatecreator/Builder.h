#pragma once
#include <QWidget>

class Builder : public QWidget {
    Q_OBJECT
public:
    explicit Builder(QWidget* parent = nullptr);
};
