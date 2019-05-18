#include "MainWindow.h"
#include "Constants.h"
#include "ProjectSettings.h"
#include "NewProjectDialog.h"
#include "Outliner.h"
#include "ListModel.h"
#include "Manifest.h"
#include "Builder.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(Constants::WindowTitle);

    splitter = new QSplitter;
    setCentralWidget(splitter);

    listModel = new ListModel(this);
    projectSettings = new ProjectSettings(this);

    createActions();
    setupSplitter();
    readSettings();
    updateActions();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    saveProject();
    writeSettings();
    event->accept();
}

void MainWindow::newProject() {
    NewProjectDialog newProjectDialog;

    if (newProjectDialog.exec() == QDialog::Accepted) {
        if (!projectPath.isEmpty()) {
            closeProject();
        }

        QString projectDir = newProjectDialog.projectDir();
        projectSettings->create(projectDir + "/" + Constants::ProjectName);
        setProjectPath(projectDir);
        manifest->setManifestPath(manifestPath);
        manifest->setFileTemplate(Constants::FileTemplate);
        addUpdate();
        saveManifest();
        addRecentProject(projectDir);
        tabWidget->setCurrentIndex(0);
    }
}

void MainWindow::openProject() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Project"));

    if (path.isEmpty()) return;

    if (!QFile::exists(path + "/" + Constants::ProjectName)) {
        QMessageBox::critical(this, tr("Error"), tr("Wrong project directory"));
        return;
    }

    loadProject(path);
}

void MainWindow::saveProject() {
    projectSettings->save();
    saveManifest();
}

void MainWindow::closeProject() {
    projectSettings->close();
    builder->clear();
    closeManifest();
    setProjectPath(QString());
}

void MainWindow::clearMenuRecentProjects() {
    for (int i = recentProjectsMenu->actions().size() - Constants::SystemRecentProjectsActions - 1; i >= 0; i--) {
        recentProjectsMenu->removeAction(recentProjectsMenu->actions().at(i));
    }

    updateActions();
}

void MainWindow::quit() {
    saveProject();
    writeSettings();
    QCoreApplication::quit();
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About %1").arg(Constants::WindowTitle),
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
    update.channel = "release";

    listModel->addUpdate(update);
    builder->createSnapshot(update.version);
    outliner->selectRow(0);
    saveManifest();
}

void MainWindow::removeUpdate(int row) {
    int result = QMessageBox::question(this, tr("Remove Update"), tr("Are you sure want remove update?"));

    if (result == QMessageBox::Yes) {
        QString version = listModel->getUpdate(row).version;
        builder->removeSnapshot(version);
        manifest->clearUpdate();
        listModel->removeUpdate(row);
        saveManifest();
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
    tabWidget->setCurrentIndex(settings.value("tab").toInt());

    int size = settings.beginReadArray("RecentProjects");

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        addRecentProject(settings.value("path").toString());
    }

    settings.endArray();

    QString projectPath = settings.value("projectPath").toString();

    if (!projectPath.isEmpty() && QFile::exists(projectPath)) {
        loadProject(projectPath);
    }
}

void MainWindow::writeSettings() {
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("splitter", splitter->saveState());
    settings.setValue("projectPath", projectPath);
    settings.setValue("tab", tabWidget->currentIndex());

    settings.beginWriteArray("RecentProjects");

    for (int i = 0; i < recentProjectsMenu->actions().size() - Constants::SystemRecentProjectsActions; ++i) {
        settings.setArrayIndex(i);
        settings.setValue("path", recentProjectsMenu->actions().at(i)->text());
    }

    settings.endArray();
}

void MainWindow::saveManifest() {
    if (manifestPath.isEmpty()) return;

    listModel->setUpdate(outliner->currentRow(), manifest->getUpdate());

    QJsonObject manifestData;
    manifestData["template"] = manifest->getFileTemplate();
    manifestData["updates"] = listModel->toJson();

    QFile file(manifestPath);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Error opening manifest file. Path:" << manifestPath;
        return;
    }

    file.write(QJsonDocument(manifestData).toJson(QJsonDocument::Indented));
    file.close();
}

void MainWindow::openManifest() {
    QFile file(manifestPath);

    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "Error opening manifest file. Path:" << manifestPath;
        return;
    }

    QJsonParseError error{};
    QJsonObject manifestData = QJsonDocument::fromJson(file.readAll(), &error).object();

    if (error.error != QJsonParseError::NoError) {
        qCritical() << "Error parsing manifest:" << error.errorString() << "Offset:" << error.offset;
        return;
    }

    manifest->setManifestPath(manifestPath);
    manifest->setFileTemplate(manifestData["template"].toString());
    listModel->fromJson(manifestData["updates"].toArray());
    outliner->selectRow(0);
    updateActions();
}

void MainWindow::closeManifest() {
    saveManifest();

    int count = listModel->rowCount();

    for (int i = 0; i < count; i++) {
        listModel->removeUpdate(0);
    }

    manifest->clear();
    manifestPath = QString();
    manifest->setManifestPath(manifestPath);
    updateActions();
}

void MainWindow::setupSplitter() {
    outliner = new Outliner(listModel);
    connect(outliner, &Outliner::addClicked, this, &MainWindow::addUpdate);
    connect(outliner, &Outliner::removeClicked, this, &MainWindow::removeUpdate);
    connect(outliner, &Outliner::selectionChanged, [this] (int selectedRow, int deselectedRow) {
        if (deselectedRow >= 0) {
            listModel->setUpdate(deselectedRow, manifest->getUpdate());
        }

        if (selectedRow >= 0) {
            const ListModel::Update& update = listModel->getUpdate(selectedRow);
            manifest->populateUpdate(update);
            builder->setVersion(update.version);
        }
    });

    splitter->addWidget(outliner);

    tabWidget = new QTabWidget;

    manifest = new Manifest(projectSettings);

    connect(manifest, &Manifest::lostFocus, [this] {
        saveManifest();
    });

    tabWidget->addTab(manifest, tr("Manifest"));
    splitter->addWidget(tabWidget);

    builder = new Builder(projectSettings, manifest);
    tabWidget->addTab(builder, tr("Builder"));

    splitter->setHandleWidth(1);
    splitter->setChildrenCollapsible(false);
    splitter->setSizes(QList<int>() << 120 << 500);
}

void MainWindow::createActions() {
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(tr("New..."), this, &MainWindow::newProject, QKeySequence("Ctrl+N"));
    fileMenu->addAction(tr("Open..."), this, &MainWindow::openProject, QKeySequence("Ctrl+O"));

    recentProjectsMenu = new QMenu(tr("Recent Projects"), this);
    recentProjectsMenu->addSeparator();
    recentProjectsMenu->addAction(tr("Clear"), this, &MainWindow::clearMenuRecentProjects);
    fileMenu->addAction(recentProjectsMenu->menuAction());

    closeAction = fileMenu->addAction(tr("Close"), this, &MainWindow::closeProject, QKeySequence("Ctrl+W"));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Exit"), this, &MainWindow::quit, QKeySequence("Ctrl+Q"));

    QMenu* helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(tr("About %1...").arg(Constants::WindowTitle), this, &MainWindow::about);
}

void MainWindow::updateActions() {
    bool enabled = !manifestPath.isEmpty();
    closeAction->setEnabled(enabled);

    for (int i = 0; i < tabWidget->count(); i++) {
        tabWidget->widget(i)->setEnabled(enabled);
    }

    outliner->setAddButtonEnabled(enabled);
}

void MainWindow::setProjectPath(const QString& path) {
    projectPath = path;

    if (!path.isEmpty()) {
        manifestPath = path + "/" + Constants::ManifestName;
    } else {
        manifestPath = QString();
    }
}

void MainWindow::addRecentProject(const QString& path) {
    if (!QFile::exists(path)) return;

    for (QAction* action : recentProjectsMenu->actions()) {
        if (action->text() == path) {
            recentProjectsMenu->removeAction(action);
        }
    }

    auto fileAction = new QAction(path);
    connect(fileAction, &QAction::triggered, [=] {
        loadProject(path);
    });

    recentProjectsMenu->insertAction(recentProjectsMenu->actions().first(), fileAction);

    if (recentProjectsMenu->actions().size() > Constants::MaxRecentProjects + Constants::SystemRecentProjectsActions) {
        recentProjectsMenu->removeAction(recentProjectsMenu->actions().at(recentProjectsMenu->actions().size() - Constants::SystemRecentProjectsActions - 1));
    }

    updateActions();
}

void MainWindow::loadProject(const QString& path) {
    if (!projectPath.isEmpty()) {
        closeProject();
    }

    setProjectPath(path);
    projectSettings->open(path + "/" + Constants::ProjectName);
    builder->load();
    openManifest();
    addRecentProject(path);
}
