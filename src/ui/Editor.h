#pragma once
#include "core/Id.h"
#include <QTextEdit>

class Editor : public QTextEdit {
    Q_OBJECT
public:
    enum class Mode {
        Plain,
        Markdown
    };

    Editor(QWidget* parent = nullptr);

    Id id() const;

    void setMode(Mode mode);
    Mode mode() const;

    void setNote(Id id, const QString& note);
    QString note() const;

    void clearNote();

signals:
    void focusLost();
    void leave();

protected:
    void focusOutEvent(QFocusEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
    void insertFromMimeData(const QMimeData* source) override;

private:
    Id m_id = 0;
    Mode m_mode = Mode::Plain;
};
