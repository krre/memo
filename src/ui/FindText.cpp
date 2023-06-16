#include "FindText.h"
#include <QtWidgets>

FindText::FindText() {
    setWindowTitle(tr("Find"));

    m_lineEdit = new QLineEdit;

    auto formLayout = new QFormLayout;
    formLayout->addRow(new QLabel(tr("Text:")), m_lineEdit);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(m_lineEdit, &QLineEdit::textChanged, this, [=] (const QString& text) {
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!text.isEmpty());
    });

    auto layout = new QVBoxLayout;
    layout->addLayout(formLayout);
    layout->addStretch(1);
    layout->addWidget(buttonBox);

    setLayout(layout);
    resize(250, sizeHint().height());
}

QString FindText::text() const {
    return m_lineEdit->text();
}
