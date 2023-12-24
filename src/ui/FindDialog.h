#pragma once
#include "StandardDialog.h"

class QLineEdit;

class FindDialog : public StandardDialog {
    Q_OBJECT
public:
    FindDialog();
    QString text() const;

private:
    QLineEdit* m_lineEdit = nullptr;
};
