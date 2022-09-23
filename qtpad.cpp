#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileSystemModel>

#include "qtpad.h"
#include "ui_qtpad.h"

QtPad::QtPad(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::QtPad)
{
    m_ui->setupUi(this);
    connect(m_ui->actionNew, &QAction::triggered, this, &QtPad::newFile);
    connect(m_ui->actionOpen_File, &QAction::triggered, this, &QtPad::openFile);
    connect(m_ui->actionOpen_Folder, &QAction::triggered, this, &QtPad::openFolder);
    //connect(m_ui->treeView, &QAction::triggered, this, &QtPad::openFileFromTree);
    connect(m_ui->actionSave, &QAction::triggered, this, &QtPad::saveFile);
    connect(m_ui->actionSave_as, &QAction::triggered, this, &QtPad::saveFileAs);
    connect(m_ui->actionExit, &QAction::triggered, this, &QtPad::exitApp);
}

QtPad::~QtPad()
{
    delete m_ui;
}
void QtPad::newFile()
{
    m_currentFile.clear();
    m_ui->textEdit->setText(QString());
    qDebug() << "New document (QtPad)";
}
void QtPad::openFile()
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
void QtPad::openFileFromTree()
{

}
void QtPad::openFolder()
{
    m_currentFolder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    QFileSystemModel *fsModel = new QFileSystemModel(this);
    fsModel->setRootPath(m_currentFolder);
    m_ui->treeView->setModel(fsModel);
    m_ui->treeView->setRootIndex(fsModel->index(m_currentFolder));

    m_ui->treeView->hideColumn(1);
    m_ui->treeView->hideColumn(2);
    m_ui->treeView->hideColumn(3);
    qDebug() << "dir: " << m_currentFolder;
}
void QtPad::saveFile()
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
void QtPad::saveFileAs()
{
    m_currentFile = QFileDialog::getSaveFileName(this, "save file name");
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
    qDebug() << "Save document as (QtPad)";
}
void QtPad::exitApp()
{
    qDebug() << "Exit app (QtPad)";
    exit(0);
}
