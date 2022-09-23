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
    void saveDocument();
    void saveDocumentAs();
    void printDoument();
    void exitApp();

private:
    Ui::QtPad *m_ui;
    QString m_currentFile;
};
#endif // QTPAD_H
