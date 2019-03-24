#include "MainWindow.h"
#include "Constants.h"
#include "Outliner.h"
#include "ListModel.h"
#include "Form.h"
#include <QtWidgets>

constexpr auto FILE_DIALOG_FILTER = "JSON Files (*.json);;All Files (*)";

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
    if (wantQuit()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile() {
    newManifest();
}

void MainWindow::openFile() {
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                tr(FILE_DIALOG_FILTER), &selectedFilter);

    if (fileName.isEmpty()) return;

    filePath = fileName;
    openManifest();
}

bool MainWindow::saveFile() {
    if (filePath.isEmpty()) {
        QString selectedFilter;
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "manifest.json",
                                    tr(FILE_DIALOG_FILTER), &selectedFilter);
        if (fileName.isEmpty()) return false;

        filePath = fileName;
    }

    saveManifest();

    return true;
}

void MainWindow::closeFile() {
    closeManifest();
}

void MainWindow::quit() {
    if (wantQuit()) {
        writeSettings();
        QCoreApplication::quit();
    }
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

    if (listModel->rowCount()) {
        QVersionNumber lastVersion = QVersionNumber::fromString(listModel->getUpdate(0).version);
        QVersionNumber newVersion(lastVersion.majorVersion(), lastVersion.minorVersion(), lastVersion.microVersion() + 1);
        update.version = newVersion.toString();
    } else {
        update.version = "1.0.0";
    }

    update.date = QDate::currentDate().toString("dd.MM.yyyy");
    listModel->addUpdate(update);
    outliner->selectRow(0);
    dirty = true;
}

void MainWindow::removeUpdate(int row) {
    int result = QMessageBox::question(this, tr("Remove Update"), tr("Are you sure want remove update?"));
    if (result == QMessageBox::Yes) {
        listModel->removeUpdate(row);
        dirty = true;
    }
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

void MainWindow::newManifest() {
    closeManifest();
    addUpdate();
    dirty = true;
}

void MainWindow::saveManifest() {
    qDebug() << "save manifest" << filePath;

    dirty = false;
}

void MainWindow::openManifest() {
    closeManifest();
    qDebug() << "open manifest" << filePath;
}

void MainWindow::closeManifest() {
    int count = listModel->rowCount();
    for (int i = 0; i < count; i++) {
        listModel->removeUpdate(0);
    }

    dirty = false;
    filePath = QString();
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
    fileMenu->addAction(tr("Close"), this, &MainWindow::closeFile, QKeySequence("Ctrl+W"));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Exit"), this, &MainWindow::quit, QKeySequence("Ctrl+Q"));

    QMenu* helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(tr("About %1...").arg(Constants::WindowTitle), this, &MainWindow::about);
}

bool MainWindow::wantQuit() {
    if (!dirty) return true;

    int result = QMessageBox::question(this, tr("Save Changes"), tr("Are you want save manifest?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (result == QMessageBox::Yes) {
        return saveFile();
    } else if (result == QMessageBox::No) {
        return true;
    }

    return false;
}
