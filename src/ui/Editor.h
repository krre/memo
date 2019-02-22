#pragma once
#include <QPlainTextEdit>

class Editor : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit Editor(QWidget* parent = nullptr);

    void setId(int id);
    int id() const;

private:
    int noteId = 0;
};
