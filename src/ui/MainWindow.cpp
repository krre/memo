#include "MainWindow.h"
#include "core/Constants.h"
#include <QtCore>
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), splitter(new QSplitter) {
    setWindowTitle(Constants::App::NAME);
    setCentralWidget(splitter);
    setupSplitter();
    readSettings();
}

MainWindow::~MainWindow() {
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
    treeView = new QTreeView;
    textEdit = new QPlainTextEdit;

    splitter->addWidget(treeView);
    splitter->addWidget(textEdit);

    splitter->setHandleWidth(1);
    splitter->setChildrenCollapsible(false);
    splitter->setSizes(QList<int>() << 120 << 500);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    writeSettings();
    event->accept();
}
