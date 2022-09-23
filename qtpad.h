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

private:
    Ui::QtPad *ui;
};
#endif // QTPAD_H
