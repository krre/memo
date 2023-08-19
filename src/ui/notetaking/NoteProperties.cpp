#include "NoteProperties.h"
#include <QtWidgets>

NoteProperties::NoteProperties(const Data& data) {
    setWindowTitle(tr("Note Properties"));

    auto formLayout = new QFormLayout;
    formLayout->addRow(new QLabel(tr("Name:")), new QLabel(data.title));
    formLayout->addRow(new QLabel(tr("Created at:")), new QLabel(data.createdAt));
    formLayout->addRow(new QLabel(tr("Updated at:")), new QLabel(data.updatedAt));

    setContentLayout(formLayout);
    buttonBox()->setStandardButtons(QDialogButtonBox::Close);
}
