#pragma once
#include <QDialog>

class QLineEdit;

class FindText : public QDialog {
    Q_OBJECT
public:
    FindText();
    QString text() const;

private:
    QLineEdit* m_lineEdit = nullptr;
};
