#include "terminal.h"
#include <QDebug>


Terminal::Terminal(QWidget *parent) : QWidget(parent)
{
    QStringList qsl = {"-into", QString::number(winId()),
                       //"-geometry","180x800",
                       "-bg", "black",
                       "-fa", "Monospace",
                       //"-fs", "10",
                       //"-fullscreen",
                       "-e", "resize -s 20 315; zsh;"};

    process.start("/usr/bin/xterm", qsl);
    process.waitForStarted();
    qDebug() << process.state();
}
