#include <QDebug>

#include "qtpad.h"
#include "ui_qtpad.h"

QtPad::QtPad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtPad)
{
    ui->setupUi(this);
    connect(ui->actionNew, &QAction::triggered, this, &QtPad::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &QtPad::openDocument);
    connect(ui->actionPrint, &QAction::triggered, this, &QtPad::printDoument);
    connect(ui->actionSave, &QAction::triggered, this, &QtPad::saveDocument);
    connect(ui->actionSave_as, &QAction::triggered, this, &QtPad::saveDocumentAs);
    connect(ui->actionExit, &QAction::triggered, this, &QtPad::exitApp);
}

QtPad::~QtPad()
{
    delete ui;
}
void QtPad::newDocument()
{
    qDebug() << "New document (QtPad)";
}
void QtPad::openDocument()
{
    qDebug() << "Open document (QtPad)";
}
void QtPad::saveDocument()
{
    qDebug() << "Save document (QtPad)";
}
void QtPad::saveDocumentAs()
{
    qDebug() << "Save document as (QtPad)";
}
void QtPad::printDoument()
{
    qDebug() << "Print document (QtPad)";
}
void QtPad::exitApp()
{
    qDebug() << "Exit app (QtPad)";
}
