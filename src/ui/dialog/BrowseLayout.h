#pragma once
#include <QHBoxLayout>

class QLineEdit;

class BrowseLayout : public QHBoxLayout {
    Q_OBJECT
public:
    enum class Mode {
        Directory,
        File
    };

    BrowseLayout(Mode mode, const QString& text = {});

    QString text() const;
    void setText(const QString& text);

private slots:
    void onClicked();

private:
    Mode m_mode;
    QLineEdit* m_lineEdit = nullptr;
};
