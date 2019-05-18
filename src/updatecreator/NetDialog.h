#pragma once
#include <QDialog>

class ProjectSettings;
class QNetworkAccessManager;
class QNetworkReply;
class QFile;
class QLineEdit;
class QProgressBar;
class QDialogButtonBox;

class NetDialog : public QDialog {
    Q_OBJECT
public:
    enum class FileType {
        Manifest,
        Update,
        Installer
    };

    enum class DialogType {
        Upload,
        Download
    };

    explicit NetDialog(DialogType dialogType, FileType fileType, const QString& filePath, ProjectSettings* projectSettings, QWidget* parent = nullptr);

signals:
    void fileDownloaded();

public slots:
    void accept() override;

    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    void uploadFinished(QNetworkReply* reply);
    void downloadFinished(QNetworkReply* reply);

    DialogType dialogType;
    FileType fileType;
    QString filePath;
    ProjectSettings* projectSettings;

    QNetworkAccessManager* manager = nullptr;
    QFile* file = nullptr;

    QLineEdit* urlLineEdit = nullptr;
    QLineEdit* loginLineEdit = nullptr;
    QLineEdit* passwordLineEdit = nullptr;
    QProgressBar* progressBar = nullptr;
    QDialogButtonBox* buttonBox = nullptr;
};
