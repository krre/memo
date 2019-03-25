#include "Form.h"
#include <QtWidgets>

Form::Form(QWidget* parent) : QWidget(parent) {
    setMinimumWidth(100);

    auto layout = new QVBoxLayout;
    setLayout(layout);

    auto generalGroupBox = new QGroupBox(tr("General"));

    auto generalGridLayout = new QGridLayout;
    generalGroupBox->setLayout(generalGridLayout);

    generalGridLayout->addWidget(new QLabel(tr("URL:")), 0, 0);
    urlLineEdit = new QLineEdit;
    connect(urlLineEdit, &QLineEdit::textEdited, this, &Form::edited);
    generalGridLayout->addWidget(urlLineEdit, 0, 1);

    layout->addWidget(generalGroupBox);

    auto updateGroupBox = new QGroupBox(tr("Update"));

    auto updateGridLayout = new QGridLayout;
    updateGroupBox->setLayout(updateGridLayout);

    updateGridLayout->addWidget(new QLabel(tr("Version:")), 0, 0);
    versionLineEdit = new QLineEdit;
    connect(versionLineEdit, &QLineEdit::textEdited, this, &Form::edited);
    updateGridLayout->addWidget(versionLineEdit, 0, 1);

    updateGridLayout->addWidget(new QLabel(tr("Date:")), 1, 0);
    dateLineEdit = new QLineEdit;
    connect(dateLineEdit, &QLineEdit::textEdited, this, &Form::edited);
    updateGridLayout->addWidget(dateLineEdit, 1, 1);

    updateGridLayout->addWidget(new QLabel(tr("OS:")), 2, 0);

    auto osLayout = new QHBoxLayout;
    windowsCheckBox = new QCheckBox("Windows");
    connect(windowsCheckBox, &QCheckBox::stateChanged, this, &Form::edited);
    osLayout->addWidget(windowsCheckBox);

    linuxCheckBox = new QCheckBox("Linux");
    connect(linuxCheckBox, &QCheckBox::stateChanged, this, &Form::edited);
    osLayout->addWidget(linuxCheckBox);

    macosCheckBox = new QCheckBox("MacOS");
    connect(macosCheckBox, &QCheckBox::stateChanged, this, &Form::edited);
    osLayout->addWidget(macosCheckBox);

    updateGridLayout->addLayout(osLayout, 2, 1, Qt::AlignLeft);

    updateGridLayout->addWidget(new QLabel("Channel:"), 3, 0);
    channelComboBox = new QComboBox;
    connect(channelComboBox, &QComboBox::currentTextChanged, this, &Form::edited);
    channelComboBox->addItem("release");
    channelComboBox->addItem("beta");
    updateGridLayout->addWidget(channelComboBox, 3, 1, Qt::AlignLeft);

    updateGridLayout->addWidget(new QLabel(tr("Size:")), 4, 0);
    sizeLabel = new QLabel;
    updateGridLayout->addWidget(sizeLabel, 4, 1);

    updateGridLayout->addWidget(new QLabel(tr("Description:")), 5, 0);
    descriptionTextEdit = new QPlainTextEdit;
    connect(descriptionTextEdit, &QPlainTextEdit::textChanged, this, &Form::edited);
    updateGridLayout->addWidget(descriptionTextEdit, 5, 1);

    updateGridLayout->setRowStretch(2, 0);

    layout->addWidget(updateGroupBox);
    layout->setStretch(1, 0);
}

void Form::populateUpdate(const ListModel::Update& update) {
    versionLineEdit->setText(update.version);
    dateLineEdit->setText(update.date);

    windowsCheckBox->setChecked(update.os.contains("windows"));
    linuxCheckBox->setChecked(update.os.contains("linux"));
    macosCheckBox->setChecked(update.os.contains("macos"));

    int channelIndex = channelComboBox->findText(update.channel);
    channelComboBox->setCurrentIndex(channelIndex);

    sizeLabel->setText(QString::number(update.size));
    fileSize = update.size;
    descriptionTextEdit->setPlainText(update.description);
}

ListModel::Update Form::getUpdate() const {
    ListModel::Update update;
    update.version = versionLineEdit->text();
    update.date = dateLineEdit->text();

    if (windowsCheckBox->isChecked()) {
        update.os.append("windows");
    }

    if (linuxCheckBox->isChecked()) {
        update.os.append("linux");
    }

    if (macosCheckBox->isChecked()) {
        update.os.append("macos");
    }

    update.channel = channelComboBox->currentText();
    update.size = fileSize;
    update.description = descriptionTextEdit->document()->toPlainText();

    return update;
}

void Form::setUrl(const QString& url) {
    urlLineEdit->setText(url);
}

QString Form::getUrl() const {
    return urlLineEdit->text();
}
