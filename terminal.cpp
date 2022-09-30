#include "terminal.h"
#include <QDebug>


Terminal::Terminal(QWidget *parent) : QWidget(parent)
{
    QStringList qsl = {"-into", QString::number(winId()),
                       "-bg", "black",
                       "-fg", "green"};
    process.start("/usr/bin/xterm", qsl);
    process.waitForStarted();
    qDebug() << process.state();
}
