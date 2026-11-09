#pragma once
#include <QObject>

class QString;

class Outliner;
class Database;

class Exporter : public QObject {
    Q_OBJECT
public:
    static void exportAll(const QString& filePath, Outliner* outliner, Database* database, QWidget* parent);

private:
    static void exportBirthdays(const QString& dirPath, Database* database);
    static void compressDir(const QString& dirPath);
};
