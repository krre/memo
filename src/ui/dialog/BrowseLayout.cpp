#include "BrowseLayout.h"
#include <QtWidgets>

BrowseLayout::BrowseLayout(Mode mode, const QString& text) : m_mode(mode) {
    m_lineEdit = new QLineEdit(text);

    auto pushButton = new QPushButton(tr("Browse..."));
    connect(pushButton, &QPushButton::clicked, this, &BrowseLayout::onClicked);

    addWidget(m_lineEdit);
    addWidget(pushButton);
}

QString BrowseLayout::text() const {
    return m_lineEdit->text();
}

void BrowseLayout::setText(const QString& text) {
    m_lineEdit->setText(text);
}

void BrowseLayout::onClicked() {
    QString path;
    QString dir = QDir(m_lineEdit->text()).absolutePath();

    if (m_mode == Mode::Directory) {
        path = QFileDialog::getExistingDirectory(nullptr, QString(), dir);
    } else {
        path = QFileDialog::getOpenFileName(nullptr, QString(), dir);
    }

    if (!path.isEmpty()) {
        m_lineEdit->setText(path);
    }
}
