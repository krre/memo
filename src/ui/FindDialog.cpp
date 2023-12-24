#include "FindDialog.h"
#include <QtWidgets>

FindDialog::FindDialog() {
    setWindowTitle(tr("Find"));

    m_lineEdit = new QLineEdit;

    auto formLayout = new QFormLayout;
    formLayout->addRow(new QLabel(tr("Text:")), m_lineEdit);

    setContentLayout(formLayout);
    resizeToWidth(250);

    buttonBox()->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(m_lineEdit, &QLineEdit::textChanged, this, [this] (const QString& text) {
        buttonBox()->button(QDialogButtonBox::Ok)->setEnabled(!text.isEmpty());
    });

    m_lineEdit->setFocus();
}

QString FindDialog::text() const {
    return m_lineEdit->text();
}
