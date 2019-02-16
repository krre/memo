#include "MainWindow.h"
#include "core/Constants.h"
#include "Outliner.h"
#include <QtCore>
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), splitter(new QSplitter) {
    setWindowTitle(Constants::App::NAME);
    setCentralWidget(splitter);
    createActions();
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
    outliner = new Outliner;
    textEdit = new QPlainTextEdit;

    splitter->addWidget(outliner);
    splitter->addWidget(textEdit);

    splitter->setHandleWidth(1);
    splitter->setChildrenCollapsible(false);
    splitter->setSizes(QList<int>() << 120 << 500);
}

void MainWindow::createActions() {
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));

    QAction* newAction = fileMenu->addAction(tr("New..."), this, &MainWindow::newFile);
    newAction->setShortcut(QKeySequence::New);

    QAction* openAction = fileMenu->addAction(tr("Open..."), this, &MainWindow::openFile);
    openAction->setShortcut(QKeySequence::Open);

    fileMenu->addSeparator();

    QAction* exitAction = fileMenu->addAction(tr("Exit"), this, &QMainWindow::close);
    exitAction->setShortcut(QKeySequence::Quit);

    QMenu* helpMenu = menuBar()->addMenu(tr("Help"));

    helpMenu->addAction(tr("About %1...").arg(Constants::App::NAME), this, &MainWindow::about);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    writeSettings();
    event->accept();
}

void MainWindow::newFile() {
    qDebug() << "new";
}

void MainWindow::openFile() {
    qDebug() << "open";
}

void MainWindow::about() {
    qDebug() << "about";
}
