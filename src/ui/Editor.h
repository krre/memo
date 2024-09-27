#pragma once
#include "core/Globals.h"
#include <QTextEdit>

class Editor : public QTextEdit {
    Q_OBJECT
public:
    explicit Editor(QWidget* parent = nullptr);

    void setId(Id id);
    Id id() const;

signals:
    void focusLost();
    void leave();

protected:
    void focusOutEvent(QFocusEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    Id m_id = 0;
};
