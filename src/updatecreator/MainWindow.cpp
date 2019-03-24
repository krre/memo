#include "MainWindow.h"
#include "Constants.h"
#include "Outliner.h"
#include "ListModel.h"
#include "Form.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(Constants::WindowTitle);

    splitter = new QSplitter;
    setCentralWidget(splitter);

    listModel = new ListModel(this);

    createActions();
    setupSplitter();
    readSettings();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    writeSettings();
    event->accept();
}

void MainWindow::newFile() {

}

void MainWindow::openFile() {

}

void MainWindow::saveFile() {

}

void MainWindow::quit() {
    writeSettings();
    QCoreApplication::quit();
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About %1").arg(Constants::ApplicationName),
        tr("<h3>%1 %2</h3>\
           Creator of updates for Memo<br><br> \
           Based on Qt %3<br> \
           Build on %4 %5<br><br> \
           <a href=%6>%6</a><br><br>Copyright © %7, Vladimir Zarypov")
            .arg(Constants::WindowTitle,
                 Constants::Version,
                 QT_VERSION_STR,
                 __DATE__,
                 __TIME__,
                 Constants::URL,
                 Constants::CopyrightYear));
}

void MainWindow::addUpdate() {
    ListModel::Update update;
    update.version = "1.0.0";
    update.date = "24.03.2019";
    listModel->addUpdate(update);
    outliner->selectRow(0);
}

void MainWindow::removeUpdate() {

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
    outliner = new Outliner(listModel);
    connect(outliner, &Outliner::addClicked, this, &MainWindow::addUpdate);
    connect(outliner, &Outliner::removeClicked, this, &MainWindow::removeUpdate);

    form = new Form;

    splitter->addWidget(outliner);
    splitter->addWidget(form);

    splitter->setHandleWidth(1);
    splitter->setChildrenCollapsible(false);
    splitter->setSizes(QList<int>() << 120 << 500);
}

void MainWindow::createActions() {
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(tr("New..."), this, &MainWindow::newFile, QKeySequence("Ctrl+N"));
    fileMenu->addAction(tr("Open..."), this, &MainWindow::openFile, QKeySequence("Ctrl+O"));
    fileMenu->addAction(tr("Save"), this, &MainWindow::saveFile, QKeySequence("Ctrl+S"));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Exit"), this, &MainWindow::quit, QKeySequence("Ctrl+Q"));

    QMenu* helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(tr("About %1...").arg(Constants::WindowTitle), this, &MainWindow::about);
}
