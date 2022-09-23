#include "qtpad.h"
#include "ui_qtpad.h"

QtPad::QtPad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtPad)
{
    ui->setupUi(this);
}

QtPad::~QtPad()
{
    delete ui;
}

