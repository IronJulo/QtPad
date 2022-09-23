#ifndef QTPAD_H
#define QTPAD_H

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
    void newDocument();
    void openDocument();
    void openFolder();
    void saveDocument();
    void saveDocumentAs();
    void exitApp();

private:
    Ui::QtPad *m_ui;
    QString m_currentFile;
    QString m_currentFolder;
};
#endif // QTPAD_H
