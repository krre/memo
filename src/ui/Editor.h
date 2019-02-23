#pragma once
#include <QPlainTextEdit>

class Editor : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit Editor(QWidget* parent = nullptr);

    void setId(int id);
    int id() const;

signals:
    void focusLost();

protected:
    void focusOutEvent(QFocusEvent* event) override;

private:
    int noteId = 0;
};
