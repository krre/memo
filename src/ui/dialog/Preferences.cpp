#include "Preferences.h"
#include "BrowseLayout.h"
#include "settings/Settings.h"
#include "core/Application.h"
#include <QtWidgets>
#include <QtNetwork>

Preferences::Preferences(Settings* settings, QWidget* parent) : StandardDialog(parent), m_settings(settings) {
    setWindowTitle(tr("Preferences"));

    auto layout = new QVBoxLayout;
    layout->addWidget(createUiGroupBox());
    layout->addWidget(createHotkeyGroupBox());
    layout->addWidget(createBackupsGroupBox());
    layout->addWidget(createServerGroupBox());
    layout->addStretch(1);

    setContentLayout(layout);
    resizeToWidth(600);
}

void Preferences::accept() {
    Settings::General general = m_settings->general();
    QString language = m_languageComboBox->currentData().toString();

    if (m_settings->general().language != language) {
        general.language = m_languageComboBox->currentData().toString();
        QMessageBox::information(this, Application::Name, tr("You must restart application"));
    }

    general.minimizeOnStartup = m_minimizeCheckBox->isChecked();
    general.hideTrayIcon = m_hideTrayCheckBox->isChecked();
    m_settings->setGeneral(general);

    Settings::Backups backups;
    backups.directory = m_backupsBrowseLayout->text();
    m_settings->setBackups(backups);

    Settings::Editor editor;
    editor.fontFamily = m_fontFamilyLineEdit->text();
    editor.fontSize = m_fontSizeLineEdit->text().toInt();
    m_settings->setEditor(editor);

    Settings::GlobalHotkey globalHotkey;
    globalHotkey.hotkey = m_hotkeyLineEdit->text();
    globalHotkey.enabled = m_hotkeyGroupBox->isChecked();
    m_settings->setGlobalHotkey(globalHotkey);

    Settings::Server server;
    server.enabled = m_serverGroupBox->isChecked();
    server.port = m_portLineEdit->text().toInt();
    server.token = m_tokenLineEdit->text();
    server.sslEnabled = m_sslGroupBox->isChecked();
    server.certificate = m_certificateBrowseLayout->text();
    server.privateKey = m_privateKeyBrowseLayout->text();
    m_settings->setServer(server);

    QDialog::accept();
}

void Preferences::onFontButtonClicked() {
    QFont currentFont = font();
    currentFont.setFamily(m_fontFamilyLineEdit->text());
    currentFont.setPointSize(m_fontSizeLineEdit->text().toInt());

    bool ok;
    QFont font = QFontDialog::getFont(&ok, currentFont, this, tr("Select Font"));

    if (ok) {
        m_fontFamilyLineEdit->setText(font.family());
        m_fontSizeLineEdit->setText(QString::number(font.pointSize()));
    }
}

void Preferences::onBackupsBrowseButtonClicked() {
    QString dirPath = QFileDialog::getExistingDirectory(nullptr, tr("Select Directory"), m_backupsBrowseLayout->text());

    if (!dirPath.isEmpty()) {
        m_backupsBrowseLayout->setText(dirPath);
    }
}

QGroupBox* Preferences::createUiGroupBox() {
    m_languageComboBox = new QComboBox;
    m_languageComboBox->addItem(tr("<System>"));
    m_languageComboBox->addItem(tr("English"), "en");
    m_languageComboBox->addItem(tr("Russian"), "ru");

    Settings::General general = m_settings->general();

    if (!general.language.isEmpty()) {
        int index = m_languageComboBox->findData(general.language);

        if (index != -1) {
            m_languageComboBox->setCurrentIndex(index);
        }
    }

    Settings::Editor editor = m_settings->editor();

    m_fontFamilyLineEdit = new QLineEdit;
    m_fontFamilyLineEdit->setReadOnly(true);
    m_fontFamilyLineEdit->setText(editor.fontFamily);

    m_fontSizeLineEdit = new QLineEdit;
    m_fontSizeLineEdit->setMaximumWidth(50);
    m_fontSizeLineEdit->setReadOnly(true);
    m_fontSizeLineEdit->setText(QString::number(editor.fontSize));

    auto fontButton = new QPushButton(tr("Open..."));
    connect(fontButton, &QPushButton::clicked, this, &Preferences::onFontButtonClicked);

    auto fontLayout = new QHBoxLayout;
    fontLayout->addWidget(m_fontFamilyLineEdit);
    fontLayout->addWidget(m_fontSizeLineEdit);
    fontLayout->addWidget(fontButton);

    m_minimizeCheckBox = new QCheckBox(tr("Minimize to tray on startup"));
    m_minimizeCheckBox->setChecked(general.minimizeOnStartup);

    m_hideTrayCheckBox = new QCheckBox(tr("Hide tray icon"));
    m_hideTrayCheckBox->setChecked(general.hideTrayIcon);

    auto formLayout = new QFormLayout;
    formLayout->addRow(tr("Language:"), m_languageComboBox);
    formLayout->addRow(tr("Font:"), fontLayout);

    formLayout->itemAt(formLayout->indexOf(m_languageComboBox))->setAlignment(Qt::AlignLeft);

    auto result = new QGroupBox(tr("User Interface"));
    auto verticalLayout = new QVBoxLayout(result);
    verticalLayout->addLayout(formLayout);
    verticalLayout->addWidget(m_minimizeCheckBox);
    verticalLayout->addWidget(m_hideTrayCheckBox);

    return result;
}

QGroupBox* Preferences::createHotkeyGroupBox() {
    Settings::GlobalHotkey globalHotkey = m_settings->globalHotkey();

    m_hotkeyGroupBox = new QGroupBox(tr("Global Hotkey"));
    m_hotkeyGroupBox->setCheckable(true);
    m_hotkeyGroupBox->setChecked(globalHotkey.enabled);

    m_hotkeyLineEdit = new QLineEdit;
    m_hotkeyLineEdit->setText(globalHotkey.hotkey);

    auto hotkeyLayout = new QVBoxLayout(m_hotkeyGroupBox);
    hotkeyLayout->addWidget(m_hotkeyLineEdit);

    return m_hotkeyGroupBox;
}

QGroupBox* Preferences::createBackupsGroupBox() {
    m_backupsBrowseLayout = new BrowseLayout(BrowseLayout::Mode::Directory, m_settings->backups().directory);

    auto formLayout = new QFormLayout;
    formLayout->addRow(tr("Directory:"), m_backupsBrowseLayout);

    auto result = new QGroupBox(tr("Backups"));
    result->setLayout(formLayout);

    return result;
}

QGroupBox* Preferences::createServerGroupBox() {
    QString addresses;

    for (const QHostAddress& address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address.isGlobal()) {
            addresses += address.toString() + ";";
        }
    }

    addresses.chop(1);

    auto addressLineEdit = new QLineEdit(addresses);
    addressLineEdit->setReadOnly(true);

    Settings::Server server = m_settings->server();

    m_portLineEdit = new QLineEdit;
    m_portLineEdit->setText(QString::number(server.port));

    m_tokenLineEdit = new QLineEdit;
    m_tokenLineEdit->setText(server.token);

    m_certificateBrowseLayout = new BrowseLayout(BrowseLayout::Mode::File, server.certificate);
    m_privateKeyBrowseLayout = new BrowseLayout(BrowseLayout::Mode::File, server.privateKey);

    auto sslFormLayout = new QFormLayout;
    sslFormLayout->addRow(tr("Certificate:"), m_certificateBrowseLayout);
    sslFormLayout->addRow(tr("Private key:"), m_privateKeyBrowseLayout);

    m_sslGroupBox = new QGroupBox("SSL");
    m_sslGroupBox->setCheckable(true);
    m_sslGroupBox->setChecked(server.sslEnabled);
    m_sslGroupBox->setLayout(sslFormLayout);

    auto formLayout = new QFormLayout;
    formLayout->addRow(tr("IP address:"), addressLineEdit);
    formLayout->addRow(tr("Port:"), m_portLineEdit);
    formLayout->addRow(tr("Token:"), m_tokenLineEdit);

    auto verticalLayout = new QVBoxLayout;
    verticalLayout->addLayout(formLayout);
    verticalLayout->addWidget(m_sslGroupBox);

    m_serverGroupBox = new QGroupBox(tr("Server"));
    m_serverGroupBox->setCheckable(true);
    m_serverGroupBox->setChecked(server.enabled);
    m_serverGroupBox->setLayout(verticalLayout);

    return m_serverGroupBox;
}
