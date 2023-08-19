#pragma once
#include "StandardDialog.h"

class QLineEdit;

class FindText : public StandardDialog {
    Q_OBJECT
public:
    FindText();
    QString text() const;

private:
    QLineEdit* m_lineEdit = nullptr;
};
