#pragma once
#include "StandardDialog.h"
#include "core/Id.h"

class Database;

class QLineEdit;
class QListWidget;

class FindAllNotesDialog : public StandardDialog {
    Q_OBJECT
public:
    FindAllNotesDialog(Database* database);
    Id noteId() const;

private slots:
    void find();

private:
    Database* m_database = nullptr;

    QLineEdit* m_lineEdit = nullptr;
    QListWidget* m_listWidget = nullptr;
};
