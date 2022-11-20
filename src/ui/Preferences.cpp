#include "Preferences.h"
#include "core/Constants.h"
#include <QtWidgets>

Preferences::Preferences(QWidget* parent) : QDialog (parent) {
    setWindowTitle(tr("Preferences"));

    auto layout = new QVBoxLayout(this);
    layout->addWidget(createUiGroupBox());
    layout->addWidget(createHotkeyGroupBox());
    layout->addStretch(1);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);

    resize(500, 300);
    readSettings();
}

void Preferences::accept() {
    if (writeSettings()) {
        QMessageBox::information(this, tr("Restart requred"), tr("You must restart application"));
    }

    QDialog::accept();
}

void Preferences::openFontDialog() {
    bool ok;
    QFont currentFont = font();
    currentFont.setFamily(m_fontFamilyLineEdit->text());
    currentFont.setPointSize(m_fontSizeLineEdit->text().toInt());

    QFont font = QFontDialog::getFont(&ok, currentFont, this, tr("Select Font"));
    if (ok) {
        m_fontFamilyLineEdit->setText(font.family());
        m_fontSizeLineEdit->setText(QString::number(font.pointSize()));
    }
}

QGroupBox* Preferences::createUiGroupBox() {
    m_languageComboBox = new QComboBox;
    m_languageComboBox->addItem(tr("<System>"));
    m_languageComboBox->addItem("English", "en");
    m_languageComboBox->addItem("Russian", "ru");

    m_fontFamilyLineEdit = new QLineEdit;
    m_fontFamilyLineEdit->setReadOnly(true);

    m_fontSizeLineEdit = new QLineEdit;
    m_fontSizeLineEdit->setMaximumWidth(50);
    m_fontSizeLineEdit->setReadOnly(true);

    auto fontButton = new QPushButton(tr("Open..."));
    connect(fontButton, &QPushButton::clicked, this, &Preferences::openFontDialog);

    auto fontLayout = new QHBoxLayout;
    fontLayout->addWidget(m_fontFamilyLineEdit);
    fontLayout->addWidget(m_fontSizeLineEdit);
    fontLayout->addWidget(fontButton);

    m_minimizeCheckBox = new QCheckBox(tr("Minimize to tray on startup"));
    m_hideTrayCheckBox = new QCheckBox(tr("Hide tray icon"));

    auto formLayout = new QFormLayout;
    formLayout->addRow(tr("Language:"), m_languageComboBox);
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

void Preferences::readSettings() {
    QSettings settings;

    QString language = settings.value("language").toString();

    if (!language.isEmpty()) {
        int index = m_languageComboBox->findData(language);
        if (index != -1) {
            m_languageComboBox->setCurrentIndex(index);
        }
    }

    m_minimizeCheckBox->setChecked(settings.value("minimizeOnStartup").toBool());
    m_hideTrayCheckBox->setChecked(settings.value("hideTrayIcon").toBool());

    m_fontFamilyLineEdit->setText(settings.value("Editor/fontFamily", font().family()).toString());
    m_fontSizeLineEdit->setText(settings.value("Editor/fontSize", font().pointSize()).toString());

    m_hotkeyLineEdit->setText(settings.value("GlobalHotkey/hotkey", Const::DefaultSettings::GlobalHotkey).toString());
    m_hotkeyGroupBox->setChecked(settings.value("GlobalHotkey/enabled", true).toBool());
}

bool Preferences::writeSettings() {
    bool restartRequre = false;

    QSettings settings;

    QString language = m_languageComboBox->currentData().toString();

    if (language != settings.value("language").toString()) {
        restartRequre = true;
    }

    settings.setValue("language", language);
    settings.setValue("minimizeOnStartup", m_minimizeCheckBox->isChecked());
    settings.setValue("hideTrayIcon", m_hideTrayCheckBox->isChecked());

    settings.setValue("Editor/fontFamily", m_fontFamilyLineEdit->text());
    settings.setValue("Editor/fontSize", m_fontSizeLineEdit->text());

    settings.setValue("GlobalHotkey/hotkey", m_hotkeyLineEdit->text());
    settings.setValue("GlobalHotkey/enabled", m_hotkeyGroupBox->isChecked());

    return restartRequre;
}
