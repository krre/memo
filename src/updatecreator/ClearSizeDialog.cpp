#include "ClearSizeDialog.h"
#include <memo/Constants.h>
#include <QtWidgets>

ClearSizeDialog::ClearSizeDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle(tr("Clear Size"));

    auto layout = new QVBoxLayout;
    setLayout(layout);

    windowsCheckBox = new QCheckBox("Windows");
    layout->addWidget(windowsCheckBox);

    linuxCheckBox = new QCheckBox("Linux");
    layout->addWidget(linuxCheckBox);

    macOSCheckBox = new QCheckBox("MacOS");
    layout->addWidget(macOSCheckBox);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    adjustSize();
}

void ClearSizeDialog::accept() {
    if (windowsCheckBox->isChecked()) {
        os.append(Memo::Constants::OS::Windows);
    }

    if (linuxCheckBox->isChecked()) {
        os.append(Memo::Constants::OS::Linux);
    }

    if (macOSCheckBox->isChecked()) {
        os.append(Memo::Constants::OS::MacOS);
    }

    QDialog::accept();
}
