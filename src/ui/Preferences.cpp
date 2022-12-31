#include "Preferences.h"
#include "core/ISettings.h"
#include <QtWidgets>
#include <QtNetwork>

Preferences::Preferences(ISettings* settings, QWidget* parent) : QDialog (parent), m_settings(settings) {
    setWindowTitle(tr("Preferences"));

    auto layout = new QVBoxLayout(this);
    layout->addWidget(createUiGroupBox());
    layout->addWidget(createHotkeyGroupBox());
    layout->addWidget(createBackupsGroupBox());
    layout->addWidget(createServerGroupBox());
    layout->addStretch(1);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);

    resize(600, 300);
    readSettings();
}

void Preferences::accept() {
    if (writeSettings()) {
        QMessageBox::information(this, tr("Restart requred"), tr("You must restart application"));
    }

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
    QString dirPath = QFileDialog::getExistingDirectory(nullptr, tr("Select Directory"), m_backupsLineEdit->text());

    if (!dirPath.isEmpty()) {
        m_backupsLineEdit->setText(dirPath);
    }
}

QGroupBox* Preferences::createUiGroupBox() {
    m_languageComboBox = new QComboBox;
    m_languageComboBox->addItem(tr("<System>"));
    m_languageComboBox->addItem(tr("English"), "en");
    m_languageComboBox->addItem(tr("Russian"), "ru");

    m_fontFamilyLineEdit = new QLineEdit;
    m_fontFamilyLineEdit->setReadOnly(true);

    m_fontSizeLineEdit = new QLineEdit;
    m_fontSizeLineEdit->setMaximumWidth(50);
    m_fontSizeLineEdit->setReadOnly(true);

    auto fontButton = new QPushButton(tr("Open..."));
    connect(fontButton, &QPushButton::clicked, this, &Preferences::onFontButtonClicked);

    auto fontLayout = new QHBoxLayout;
    fontLayout->addWidget(m_fontFamilyLineEdit);
    fontLayout->addWidget(m_fontSizeLineEdit);
    fontLayout->addWidget(fontButton);

    m_minimizeCheckBox = new QCheckBox(tr("Minimize to tray on startup"));
    m_hideTrayCheckBox = new QCheckBox(tr("Hide tray icon"));

    auto formLayout = new QFormLayout;
    formLayout->addRow(tr("Language:"), m_languageComboBox);
    formLayout->itemAt(0, QFormLayout::FieldRole)->setAlignment(Qt::AlignLeft);
    formLayout->addRow(tr("Font:"), fontLayout);

    auto result = new QGroupBox(tr("User Interface"));
    auto verticalLayout = new QVBoxLayout(result);
    verticalLayout->addLayout(formLayout);
    verticalLayout->addWidget(m_minimizeCheckBox);
    verticalLayout->addWidget(m_hideTrayCheckBox);

    return result;
}

QGroupBox* Preferences::createHotkeyGroupBox() {
    m_hotkeyGroupBox = new QGroupBox(tr("Global Hotkey"));
    m_hotkeyGroupBox->setCheckable(true);

    m_hotkeyLineEdit = new QLineEdit;

    auto hotkeyLayout = new QVBoxLayout(m_hotkeyGroupBox);
    hotkeyLayout->addWidget(m_hotkeyLineEdit);

    return m_hotkeyGroupBox;
}

QGroupBox* Preferences::createBackupsGroupBox() {
    auto label = new QLabel(tr("Directory:"));
    m_backupsLineEdit = new QLineEdit;

    auto browseButton = new QPushButton(tr("Browse..."));
    connect(browseButton, &QPushButton::clicked, this, &Preferences::onBackupsBrowseButtonClicked);

    auto result = new QGroupBox(tr("Backups"));
    auto horizontalLayout = new QHBoxLayout(result);
    horizontalLayout->addWidget(label);
    horizontalLayout->addWidget(m_backupsLineEdit);
    horizontalLayout->addWidget(browseButton);
    return result;
}

QGroupBox* Preferences::createServerGroupBox() {
    auto addressTextEdit = new QPlainTextEdit;
    addressTextEdit->setReadOnly(true);

    for (const QHostAddress& address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address.isGlobal()) {
            addressTextEdit->appendPlainText(address.toString());
        }
    }

    m_portLineEdit = new QLineEdit;
    m_keyLineEdit = new QLineEdit;

    auto formLayout = new QFormLayout;
    formLayout->addRow(tr("IP Address:"), addressTextEdit);
    formLayout->addRow(tr("Port:"), m_portLineEdit);
    formLayout->addRow(tr("Key:"), m_keyLineEdit);

    m_serverGroupBox = new QGroupBox(tr("Server"));
    m_serverGroupBox->setCheckable(true);
    m_serverGroupBox->setLayout(formLayout);

    return m_serverGroupBox;
}

void Preferences::readSettings() {
    if (!m_settings->general.language.isEmpty()) {
        int index = m_languageComboBox->findData(m_settings->general.language);
        if (index != -1) {
            m_languageComboBox->setCurrentIndex(index);
        }
    }

    m_minimizeCheckBox->setChecked(m_settings->general.minimizeOnStartup);
    m_hideTrayCheckBox->setChecked(m_settings->general.hideTrayIcon);

    m_fontFamilyLineEdit->setText(m_settings->editor.fontFamily);
    m_fontSizeLineEdit->setText(QString::number(m_settings->editor.fontSize));

    m_hotkeyLineEdit->setText(m_settings->globalHotKey.hotKey);
    m_hotkeyGroupBox->setChecked(m_settings->globalHotKey.enabled);

    m_backupsLineEdit->setText(m_settings->backups.directory);

    m_serverGroupBox->setChecked(m_settings->server.enabled);
    m_portLineEdit->setText(QString::number(m_settings->server.port));
    m_keyLineEdit->setText(m_settings->server.key);
}

bool Preferences::writeSettings() {
    bool restartRequre = false;

    QString language = m_languageComboBox->currentData().toString();

    if (language != m_settings->general.language) {
        restartRequre = true;
    }

    m_settings->general.language = language;
    m_settings->general.minimizeOnStartup = m_minimizeCheckBox->isChecked();
    m_settings->general.hideTrayIcon = m_hideTrayCheckBox->isChecked();

    m_settings->editor.fontFamily = m_fontFamilyLineEdit->text();
    m_settings->editor.fontSize = m_fontSizeLineEdit->text().toInt();

    m_settings->globalHotKey.hotKey = m_hotkeyLineEdit->text();
    m_settings->globalHotKey.enabled = m_hotkeyGroupBox->isChecked();

    m_settings->backups.directory = m_backupsLineEdit->text();

    m_settings->server.enabled = m_serverGroupBox->isChecked();
    m_settings->server.port = m_portLineEdit->text().toInt();
    m_settings->server.key = m_keyLineEdit->text();

    return restartRequre;
}
