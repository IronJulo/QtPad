#include "codeeditor.h"
#include "qlayout.h"
#include "qtpad.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtPad w;

    //w.layout()->addWidget(&codeEditor);
    w.show();
    return a.exec();
}
