#ifndef TERMINAL_H
#define TERMINAL_H

#include <QWidget>
#include <QProcess>

class Terminal : public QWidget
{
    Q_OBJECT
public:
    explicit Terminal(QWidget *parent = nullptr);
    QProcess process;
};

#endif // TERMINAL_H
