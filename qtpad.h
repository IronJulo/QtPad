#ifndef QTPAD_H
#define QTPAD_H

#include <QFileSystemModel>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class QtPad; }
QT_END_NAMESPACE

class QtPad : public QMainWindow
{
    Q_OBJECT

public:
    QtPad(QWidget *parent = nullptr);
    ~QtPad();

private slots:
    void s_newFile();
    void s_openFile();
    void s_openFolder();
    void s_saveFile();
    void s_saveFileAs();
    void s_exitApp();

    void s_treeFileClicked(const QModelIndex &index);

private:
    void openFile();

private:
    Ui::QtPad *m_ui;
    QFileSystemModel *m_fsModel;
    QString m_currentFilePath;
    QString m_currentFileName;
    QString m_currentFolderPath;
};
#endif // QTPAD_H
