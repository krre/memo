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
    void leave();

protected:
    void focusOutEvent(QFocusEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    int noteId = 0;
};
