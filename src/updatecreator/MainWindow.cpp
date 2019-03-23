#include "MainWindow.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Memo Update Creator");

    splitter = new QSplitter;
    setCentralWidget(splitter);

    setupSplitter();
    readSettings();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    writeSettings();
    event->accept();
}

void MainWindow::readSettings() {
    QSettings settings;

    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();

    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 2, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }

    splitter->restoreState(settings.value("splitter").toByteArray());
}

void MainWindow::writeSettings() {
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("splitter", splitter->saveState());
}

void MainWindow::setupSplitter() {
    splitter->addWidget(new QListView);
    splitter->addWidget(new QTextEdit);

    splitter->setHandleWidth(1);
    splitter->setChildrenCollapsible(false);
    splitter->setSizes(QList<int>() << 120 << 500);
}
