#include "NoteProperties.h"
#include <QLabel>
#include <QDialogButtonBox>
#include <QFormLayout>

NoteProperties::NoteProperties(const Note& note) {
    setWindowTitle(tr("Note Properties"));

    auto formLayout = new QFormLayout;
    formLayout->addRow(new QLabel(tr("ID:")), new QLabel(note.id.toString()));
    formLayout->addRow(new QLabel(tr("Name:")), new QLabel(note.title));
    formLayout->addRow(new QLabel(tr("Created at:")), new QLabel(note.createdAt));
    formLayout->addRow(new QLabel(tr("Updated at:")), new QLabel(note.updatedAt));

    setContentLayout(formLayout);
    buttonBox()->setStandardButtons(QDialogButtonBox::Close);
}
