#include "NoteProperties.h"
#include <QtWidgets>

NoteProperties::NoteProperties(const Note& note) {
    setWindowTitle(tr("Note Properties"));

    auto formLayout = new QFormLayout;
    formLayout->addRow(new QLabel(tr("Name:")), new QLabel(note.title));
    formLayout->addRow(new QLabel(tr("Created at:")), new QLabel(note.createdAt));
    formLayout->addRow(new QLabel(tr("Updated at:")), new QLabel(note.updatedAt));

    setContentLayout(formLayout);
    buttonBox()->setStandardButtons(QDialogButtonBox::Close);
}
