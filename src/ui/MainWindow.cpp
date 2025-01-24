#include "MainWindow.h"
#include "RecentFilesMenu.h"
#include "Editor.h"
#include "TrayIcon.h"
#include "Birthdays.h"
#include "core/Application.h"
#include "core/Exception.h"
#include "core/SolidString.h"
#include "core/Exporter.h"
#include "settings/FileSettings.h"
#include "dialog/Preferences.h"
#include "dialog/FindAllNotesDialog.h"
#include "notetaking/NoteTaking.h"
#include "database/Database.h"
#include "hotkey/GlobalHotkey.h"
#include "server/HttpServerManager.h"
#include <QSplitter>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QScreen>
#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(Application::Name);
    setWindowIcon(QIcon(":/images/icon.png"));

    m_fileSettings.reset(new FileSettings);

    m_splitter = new QSplitter;
    setCentralWidget(m_splitter);

    m_database = new Database(this);
    m_serverManager = new HttpServerManager(m_database, this);

    m_globalHotkey = new GlobalHotkey(this);
    connect(m_globalHotkey, &GlobalHotkey::activated, this, &MainWindow::onGlobalActivated);

    m_trayIcon = new TrayIcon(this);

    setupSplitter();
    createActions();

    connect(m_notetaking, &NoteTaking::noteChanged, this, &MainWindow::onNoteChanged);
    connect(m_editor, &Editor::focusLost, this, &MainWindow::onEditorFocusLost);
    connect(m_editor, &Editor::leave, this, [this] {
       m_notetaking->setFocus();
    });

    setCurrentFile("");
    readSettings();
}

MainWindow::~MainWindow() {

}

void MainWindow::quit() {
    writeSettings();
    QCoreApplication::quit();
}

void MainWindow::readSettings() {
    applyHotSettings();

    QByteArray geometry = m_fileSettings->mainWindowGeometry();

    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    } else {
        const QRect availableGeometry = QGuiApplication::screens().constFirst()->availableGeometry();
        resize(availableGeometry.width() / 2, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    }

    restoreState(m_fileSettings->mainWindowState());

    m_splitter->restoreState(m_fileSettings->mainWindowSplitter());

    loadFile(m_fileSettings->applicationFilePath());

    if (!m_fileSettings->applicationMinimizeOnStartup()) {
        show();
    }
}

void MainWindow::writeSettings() {
    m_fileSettings->setMainWindowGeometry(saveGeometry());
    m_fileSettings->setMainWindowState(m_splitter->saveState());
    m_fileSettings->setMainWindowSplitter(m_splitter->saveState());

    m_fileSettings->setApplicationFilePath(m_currentFile);
    m_recentFilesMenu->save();
}

void MainWindow::applyHotSettings() {
    m_trayIcon->setVisible(!m_fileSettings->applicationHideTrayIcon());

    bool globalHotkeyEnabled = m_fileSettings->globalHotkeyEnabled();

    if (globalHotkeyEnabled) {
        m_globalHotkey->setShortcut(m_fileSettings->globalHotkeyValue());
    } else {
        m_globalHotkey->unsetShortcut();
    }

    QString fontFamily = m_fileSettings->editorFontFamily();

    if (!fontFamily.isEmpty()) {
        QFont font;
        font.setFamily(fontFamily);

        int fontSize = m_fileSettings->editorFontSize();

        if (fontSize) {
            font.setPointSize(fontSize);
        }

        m_editor->setFont(font);
    }

    m_serverManager->stop();

    if (!m_fileSettings->serverEnabled()) {
        return;
    }

    QString token = m_fileSettings->serverToken();

    if (token.isEmpty()) {
        qCritical().noquote() << "Server token is empty";
        return;
    }

    int port = m_fileSettings->serverPort();

    if (m_fileSettings->serverSslEnabled()) {
        QString certificate = m_fileSettings->serverCertificate();

        if (certificate.isEmpty()) {
            qCritical().noquote() << "Server SSL certificate path is empty";
            return;
        }

        QString privateKey = m_fileSettings->serverPrivateKey();

        if (privateKey.isEmpty()) {
            qCritical().noquote() << "Server SSL private key is empty";
            return;
        }

        m_serverManager->start(port, SolidString(token), SolidString(certificate), SolidString(privateKey));
    } else {
        m_serverManager->start(port, SolidString(token));
    }
}

void MainWindow::setupSplitter() {
    m_notetaking = new NoteTaking(m_database);
    m_editor = new Editor;

    m_splitter->addWidget(m_notetaking);
    m_splitter->addWidget(m_editor);

    m_splitter->setHandleWidth(1);
    m_splitter->setChildrenCollapsible(false);
    m_splitter->setSizes({ 120, 500 });
}

void MainWindow::createActions() {
    auto fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(tr("New..."), Qt::CTRL | Qt::Key_N, this, &MainWindow::createFile);
    fileMenu->addAction(tr("Open..."), Qt::CTRL | Qt::Key_O, this, &MainWindow::open);

    m_recentFilesMenu = new RecentFilesMenu(m_fileSettings.data(), this);
    connect(m_recentFilesMenu, &RecentFilesMenu::activated, this, &MainWindow::loadFile);
    fileMenu->addAction(m_recentFilesMenu->menuAction());

    auto exportAction = fileMenu->addAction(tr("Export All..."), Qt::CTRL | Qt::Key_E, this, &MainWindow::exportAll);
    auto createBackupAction = fileMenu->addAction(tr("Create Backup..."), this, &MainWindow::backup);
    auto closeAction = fileMenu->addAction(tr("Close"), Qt::CTRL | Qt::Key_W, this, &MainWindow::closeFile);

    exportAction->setEnabled(false);
    createBackupAction->setEnabled(false);
    closeAction->setEnabled(false);

    connect(this, &MainWindow::isOpened, exportAction, &QAction::setEnabled);
    connect(this, &MainWindow::isOpened, createBackupAction, &QAction::setEnabled);
    connect(this, &MainWindow::isOpened, closeAction, &QAction::setEnabled);

    fileMenu->addSeparator();
    fileMenu->addAction(tr("Preferences..."), this, &MainWindow::showPreferences);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Hide"), Qt::Key_Escape, this, &MainWindow::hide);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Exit"), Qt::CTRL | Qt::Key_Q, this, &MainWindow::quit);

    m_editMenu = menuBar()->addMenu(tr("Edit"));
    auto undoAction = m_editMenu->addAction(tr("Undo"), QKeySequence::Undo, m_editor, &Editor::undo);
    auto redoAction = m_editMenu->addAction(tr("Redo"), QKeySequence::Redo, m_editor, &Editor::redo);
    m_editMenu->addSeparator();
    auto cutAction = m_editMenu->addAction(tr("Cut"), QKeySequence::Cut, m_editor, &Editor::cut);
    auto copyAction = m_editMenu->addAction(tr("Copy"), QKeySequence::Copy, m_editor, &Editor::copy);
    auto pasteAction = m_editMenu->addAction(tr("Paste"), QKeySequence::Paste, m_editor, &Editor::paste);
    m_editMenu->addSeparator();
    auto selectAllAction = m_editMenu->addAction(tr("Select All"), QKeySequence::SelectAll, m_editor, &Editor::selectAll);
    m_editMenu->addSeparator();
    auto findAction = m_editMenu->addAction(tr("Find..."), QKeySequence::Find, this, &MainWindow::find);
    auto findAllAction = m_editMenu->addAction(tr("Find in All Notes..."), QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F), this, &MainWindow::findInAllNotes);
    m_findNextAction = m_editMenu->addAction(tr("Find Next"), QKeySequence::FindNext, this, &MainWindow::findNext);
    m_findPreviousAction = m_editMenu->addAction(tr("Find Previous"), QKeySequence::FindPrevious, this, &MainWindow::findPrevious);

    undoAction->setEnabled(false);
    redoAction->setEnabled(false);
    cutAction->setEnabled(false);
    copyAction->setEnabled(false);
    selectAllAction->setEnabled(false);
    findAction->setEnabled(false);
    findAllAction->setEnabled(false);
    m_findNextAction->setEnabled(false);
    m_findPreviousAction->setEnabled(false);

    connect(m_editor, &QTextEdit::undoAvailable, undoAction, &QAction::setEnabled);
    connect(m_editor, &QTextEdit::redoAvailable, redoAction, &QAction::setEnabled);
    connect(m_editor, &QTextEdit::copyAvailable, cutAction, &QAction::setEnabled);
    connect(m_editor, &QTextEdit::copyAvailable, copyAction, &QAction::setEnabled);
    connect(m_editor, &QTextEdit::textChanged, this, [=, this] {
        selectAllAction->setEnabled(!m_editor->document()->isEmpty());
    });
    connect(this, &MainWindow::isOpened, pasteAction, &QAction::setEnabled);
    connect(this, &MainWindow::isOpened, findAction, &QAction::setEnabled);
    connect(this, &MainWindow::isOpened, findAllAction, &QAction::setEnabled);

    m_eventsMenu = menuBar()->addMenu(tr("Events"));
    m_eventsMenu->addAction(tr("Birthdays..."), this, &MainWindow::showBirthdays);

    auto helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(tr("Open download page"), [] {
        QDesktopServices::openUrl(QUrl(Application::ReleasesUrl));
    });
    helpMenu->addAction(tr("About %1...").arg(Application::Name), this, &MainWindow::about);
}

void MainWindow::loadFile(const QString& filePath) {
    if (filePath.isEmpty() || !QFile::exists(filePath)) return;

    try {
        m_database->open(filePath);
        m_notetaking->build();
        setCurrentFile(filePath);
        m_recentFilesMenu->addPath(filePath);

        if (m_database->isBirthdayToday()) {
            showBirthdays();
        }
    } catch (const Exception& e) {
        showErrorDialog(e.error());
    }
}

void MainWindow::setCurrentFile(const QString& filePath) {
    QString title = QApplication::applicationName();
    bool isFileOpened = !filePath.isEmpty();

    if (isFileOpened) {
        QFileInfo fi(filePath);
        title = title + " - " + fi.fileName();
    }

    setWindowTitle(title);
    m_currentFile = filePath;
    m_editMenu->menuAction()->setVisible(isFileOpened);
    m_eventsMenu->menuAction()->setVisible(isFileOpened);
    emit isOpened(isFileOpened);
}

void MainWindow::showErrorDialog(const QString& message) {
    QMessageBox::critical(this, Application::Name, message, QMessageBox::Ok);
}

QString MainWindow::dateFileName(const QString& name) {
    QDateTime dateTime = QDateTime::currentDateTime();
    QFileInfo fi(name);
    return fi.baseName() + "-" + dateTime.date().toString("yyyy-MM-dd") + "_" + dateTime.time().toString("HH-mm-ss") + "." + fi.completeSuffix();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    writeSettings();
    event->accept();
}

void MainWindow::createFile() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("New File"), "notes.db",
                                                    tr("All Files (*);;Database Files (*.db)"));

    if (fileName.isEmpty()) return;

    closeFile();

    if (QFile::exists(fileName)) {
        if (!QFile::remove(fileName)) {
            showErrorDialog(tr("Error rewriting old file"));
        }
    }

    try {
        m_database->create(fileName);
        loadFile(fileName);
    } catch (const Exception& e) {
        showErrorDialog(e.error());
    }
}

void MainWindow::open() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                                                    tr("All Files (*);;Database Files (*.db)"));
    if (!fileName.isEmpty()) {
        closeFile();
        loadFile(fileName);
    }
}

void MainWindow::exportAll() {
    QFileInfo fi(m_currentFile);
    QString name = m_fileSettings->backupsDirectory() + "/" + dateFileName(fi.baseName() + ".zip");
    QString filePath = QFileDialog::getSaveFileName(this, tr("Export notes to ZIP archive"), name);

    if (!filePath.isEmpty()) {
        Exporter::exportAll(filePath, m_notetaking, m_database, this);
    }
}

void MainWindow::backup() {
    QFileInfo fi(m_currentFile);
    QString name = m_fileSettings->backupsDirectory() + "/" + dateFileName(fi.fileName());

    QString backupFile = QFileDialog::getSaveFileName(this, tr("Create Backup"), name);

    if (!backupFile.isEmpty()) {
        QFile::copy(m_currentFile, backupFile);
    }
}

void MainWindow::closeFile() {
    m_database->close();
    onNoteChanged(0);
    m_notetaking->clear();
    setCurrentFile();
}

void MainWindow::showPreferences() {
    Preferences preferences(m_fileSettings.data());

    if (preferences.exec() == QDialog::Accepted) {
        applyHotSettings();
    }
}

void MainWindow::find() {
    bool ok;
    m_findText = QInputDialog::getText(this, tr("Find"), tr("Text:"), QLineEdit::Normal, "", &ok);

    if (!ok || m_findText.isEmpty()) {
        return;
    }

    QTextCursor cursor = m_editor->textCursor();
    QTextCursor savedCursor = cursor;

    cursor.movePosition(QTextCursor::Start);\
    m_editor->setTextCursor(cursor);

    if (!m_editor->find(m_findText)) {
        QMessageBox::warning(this, Application::Name, tr("Text not found"));
        m_editor->setTextCursor(savedCursor);
        return;
    }

    m_findNextAction->setEnabled(true);
    m_findPreviousAction->setEnabled(true);
}

void MainWindow::findInAllNotes() {
    FindAllNotesDialog findAllNotesDialog(m_database);

    if (findAllNotesDialog.exec() == QDialog::Accepted) {
        m_notetaking->setCurrentId(findAllNotesDialog.noteId());
    }
}

void MainWindow::findNext() {
    m_editor->find(m_findText);
}

void MainWindow::findPrevious() {
    m_editor->find(m_findText, QTextDocument::FindBackward);
}

void MainWindow::showBirthdays() {
    auto birthdays = new Birthdays(m_database, m_fileSettings.data());
    birthdays->show();
    birthdays->activateWindow();
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About %1").arg(Application::Name),
        tr("<h3>%1 %2</h3>"
           "Note-taking for quick notes<br><br>"
           "Based on Qt %3<br>"
           "Build on %4 %5<br><br>"
           "<a href=%6>%6</a><br><br>Copyright © %7, Vladimir Zarypov")
            .arg(Application::Name, Application::Version, QT_VERSION_STR,
            Application::BuildDate, Application::BuildTime, Application::Url, Application::Years));
}

void MainWindow::onNoteChanged(Id id) {
    m_editor->setId(id);
    m_editor->setEnabled(id > 0);

    m_findNextAction->setEnabled(false);
    m_findPreviousAction->setEnabled(false);

    if (id) {
        bool markdown = m_database->noteValue(id, "markdown").toInt();
        m_editor->setMode(markdown ? Editor::Mode::Markdown : Editor::Mode::Plain);

        QString note = m_database->noteValue(id, "note").toString();
        m_editor->setNote(note);
        m_editor->setFocus();

        int line = m_database->noteValue(id, "line").toInt();
        QTextCursor cursor = m_editor->textCursor();
        cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, line);
        m_editor->setTextCursor(cursor);
    } else {
        m_editor->clear();
    }
}

void MainWindow::onEditorFocusLost() {
    Id lastId = m_editor->id();

    if (!lastId) return;

    if (m_editor->document()->isModified()) {
        m_database->updateNoteValue(lastId, "note", m_editor->note());
    }

    m_database->updateNoteValue(lastId, "line", m_editor->textCursor().blockNumber());
    m_database->updateNoteValue(lastId, "markdown", m_editor->mode() == Editor::Mode::Markdown ? 1 : 0);
}

void MainWindow::onGlobalActivated() {
    show();
    raise();
    activateWindow();
}
