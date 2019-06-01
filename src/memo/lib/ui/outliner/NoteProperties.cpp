#include "NoteProperties.h"
#include <QtWidgets>

NoteProperties::NoteProperties(const Data& data) {
    setWindowTitle(tr("Note Properties"));

    auto layout = new QVBoxLayout;
    setLayout(layout);

    auto gridLayout = new QGridLayout;
    gridLayout->setColumnStretch(1, 1);

    gridLayout->addWidget(new QLabel(tr("Name:")), 0, 0);
    gridLayout->addWidget(new QLabel(data.title), 0, 1);

    gridLayout->addWidget(new QLabel(tr("Created at:")), 1, 0);
    gridLayout->addWidget(new QLabel(data.createdAt), 1, 1);

    gridLayout->addWidget(new QLabel(tr("Updated at:")), 2, 0);
    gridLayout->addWidget(new QLabel(data.updatedAt), 2, 1);

    layout->addLayout(gridLayout);
    layout->addStretch(1);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
