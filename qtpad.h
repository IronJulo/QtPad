#ifndef QTPAD_H
#define QTPAD_H

#include "codeeditor.h"

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
    void s_tabChanged(int index);

private:
    void openFile();
    void saveFile();

private:
    Ui::QtPad *m_ui;
    QFileSystemModel *m_fsModel;
    CodeEditor *m_currentEditor; //TODO nullptr when nonne & security checks
    QString m_currentFolderPath;
};
#endif // QTPAD_H
