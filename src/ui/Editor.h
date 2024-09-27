#pragma once
#include "core/Globals.h"
#include <QTextEdit>

class Editor : public QTextEdit {
    Q_OBJECT
public:
    enum class Mode {
        Plain,
        Markdown
    };

    explicit Editor(QWidget* parent = nullptr);

    void setId(Id id);
    Id id() const;

    void setMode(Mode mode);
    Mode mode() const;

    void setNote(const QString& note);
    QString note() const;

signals:
    void focusLost();
    void leave();

protected:
    void focusOutEvent(QFocusEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    Id m_id = 0;
    Mode m_mode = Mode::Plain;
};
