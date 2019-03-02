#include "Options.h"
#include "core/Constants.h"
#include <QtWidgets>

Options::Options(QWidget* parent) : QDialog (parent) {
    setWindowTitle(tr("Options"));
    QVBoxLayout* layout = new QVBoxLayout;

    QGroupBox* groupBoxUi = new QGroupBox(tr("User Interface"));

    QGridLayout* gridLayoutUi = new QGridLayout;
    gridLayoutUi->setColumnStretch(1, 1);

    gridLayoutUi->addWidget(new QLabel(tr("Language:")), 0, 0);

    languageComboBox = new QComboBox;
    languageComboBox->addItem(tr("<System>"));
    languageComboBox->addItem("English", "en");
    languageComboBox->addItem("Russian", "ru");

    gridLayoutUi->addWidget(languageComboBox, 0, 1, Qt::AlignLeft);

    minimizeCheckBox = new QCheckBox(tr("Minimize to tray on startup"));
    gridLayoutUi->addWidget(minimizeCheckBox, 1, 0, 1, -1);

    hideTrayCheckBox = new QCheckBox(tr("Hide tray icon"));
    gridLayoutUi->addWidget(hideTrayCheckBox, 2, 0, 1, -1);

    groupBoxUi->setLayout(gridLayoutUi);

    layout->addWidget(groupBoxUi);

    groupBoxHotkey = new QGroupBox(tr("Global Hotkey"));
    groupBoxHotkey->setCheckable(true);
    QVBoxLayout* layoutHotkey = new QVBoxLayout;
    hotkeyLineEdit = new QLineEdit(groupBoxHotkey);
    layoutHotkey->addWidget(hotkeyLineEdit);
    groupBoxHotkey->setLayout(layoutHotkey);

    layout->addWidget(groupBoxHotkey);

    layout->addStretch(1);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setLayout(layout);

    resize(500, 300);

    readSettings();
}

void Options::accept() {
    if (writeSettings()) {
        QMessageBox::information(this, tr("Restart requred"), tr("You must restart application"));
    }

    QDialog::accept();
}

void Options::readSettings() {
    QSettings settings;
    QString language = settings.value("language").toString();

    if (!language.isEmpty()) {
        int index = languageComboBox->findData(language);
        if (index != -1) {
            languageComboBox->setCurrentIndex(index);
        }
    }

    minimizeCheckBox->setChecked(settings.value("minimizeOnStartup").toBool());
    hideTrayCheckBox->setChecked(settings.value("hideTrayIcon").toBool());

    hotkeyLineEdit->setText(settings.value("GlobalHotkey/hotkey", Constants::Window::DefaultGlobalHotkey).toString());
    groupBoxHotkey->setChecked(settings.value("GlobalHotkey/enabled", true).toBool());
}

bool Options::writeSettings() {
    bool restartRequre = false;

    QSettings settings;

    QString language = languageComboBox->currentData().toString();
    if (language != settings.value("language").toString()) {
        restartRequre = true;
    }

    settings.setValue("language", language);
    settings.setValue("minimizeOnStartup", minimizeCheckBox->isChecked());
    settings.setValue("hideTrayIcon", hideTrayCheckBox->isChecked());

    settings.setValue("GlobalHotkey/hotkey", hotkeyLineEdit->text());
    settings.setValue("GlobalHotkey/enabled", groupBoxHotkey->isChecked());

    return restartRequre;
}
