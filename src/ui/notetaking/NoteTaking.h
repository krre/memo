#pragma once
#include <QTreeView>
#include <core/Globals.h>

class QMenu;
class QAction;
class TreeModel;
class Database;

class NoteTaking : public QTreeView {
    Q_OBJECT
public:
    NoteTaking(Database* database);

    void exportAllNotes(const QString& filePath);

public slots:
    void updateActions();
    void build();
    void clear();

signals:
    void noteChanged(Id id);

protected:
    void mousePressEvent(QMouseEvent* event) override;

protected slots:
    void currentChanged(const QModelIndex& current, const QModelIndex& previous) override;

private slots:
    void onCustomContextMenu(const QPoint& point);
    void addNote();
    void removeNotes();
    void renameNote();
    void moveUp();
    void moveDown();
    void moveTree(const QModelIndex& index);
    void showProperties();

private:
    void createContextMenu();
    void updateContextMenu();
    void insertChild(const QString& title);
    int exportNote(Id parentId, const QString& path);
    void compressDir(const QString& dirPath);

    QMenu* m_contextMenu = nullptr;
    QAction* m_removeAction = nullptr;
    QAction* m_renameAction = nullptr;
    QAction* m_moveUpAction = nullptr;
    QAction* m_moveDownAction = nullptr;
    QAction* m_propertiesAction = nullptr;
    TreeModel* m_model = nullptr;
    Database* m_database = nullptr;
    bool m_isInited = false;
};
