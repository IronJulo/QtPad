#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include "qtpad.h"
#include "ui_qtpad.h"

QtPad::QtPad(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::QtPad)
{
    m_ui->setupUi(this);
    connect(m_ui->actionNew, &QAction::triggered, this, &QtPad::newDocument);
    connect(m_ui->actionOpen, &QAction::triggered, this, &QtPad::openDocument);
    connect(m_ui->actionPrint, &QAction::triggered, this, &QtPad::printDoument);
    connect(m_ui->actionSave, &QAction::triggered, this, &QtPad::saveDocument);
    connect(m_ui->actionSave_as, &QAction::triggered, this, &QtPad::saveDocumentAs);
    connect(m_ui->actionExit, &QAction::triggered, this, &QtPad::exitApp);
}

QtPad::~QtPad()
{
    delete m_ui;
}
void QtPad::newDocument()
{
    m_currentFile.clear();
    m_ui->textEdit->setText(QString());
    qDebug() << "New document (QtPad)";
}
void QtPad::openDocument()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Choose your file");
    QFile file(fileName);
    m_currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    m_ui->textEdit->setText(text);
    file.close();

    qDebug() << "Open document (QtPad)";
}
void QtPad::saveDocument()
{
    if (m_currentFile.isEmpty()){
        m_currentFile = QFileDialog::getSaveFileName(this, "save file name");
    }

    QFile file(m_currentFile);

    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    setWindowTitle(m_currentFile);

    QTextStream out(&file);
    QString  text = m_ui->textEdit->toPlainText();
    out << text;
    file.close();
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
