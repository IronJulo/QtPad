#include "qtpad.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtPad w;
    w.show();
    return a.exec();
}
