#pragma once
#include <QObject>

class Editor;

class FindText : public QObject {
    Q_OBJECT
public:
    FindText(Editor* editor, QWidget* parent);

public slots:
    void find();
    void findNext();
    void findPrevious();
    void reset();

signals:
    void navigationAvailableChanged(bool available);

private:
    void setNavigationAvailable(bool available);

    Editor* m_editor = nullptr;
    QWidget* m_dialogParent = nullptr;
    QString m_text;
    bool m_navigationAvailable = false;
};
