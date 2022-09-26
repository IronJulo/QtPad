#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileSystemModel>

#include "codeeditor.h"
#include "qtpad.h"
#include "ui_qtpad.h"

QtPad::QtPad(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::QtPad)
{
    m_ui->setupUi(this);
    connect(m_ui->actionNew, &QAction::triggered, this, &QtPad::s_newFile);
    connect(m_ui->actionOpen_File, &QAction::triggered, this, &QtPad::s_openFile);
    connect(m_ui->actionOpen_Folder, &QAction::triggered, this, &QtPad::s_openFolder);
    connect(m_ui->actionSave, &QAction::triggered, this, &QtPad::s_saveFile);
    connect(m_ui->actionSave_as, &QAction::triggered, this, &QtPad::s_saveFileAs);
    connect(m_ui->actionExit, &QAction::triggered, this, &QtPad::s_exitApp);

    connect(m_ui->treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(s_treeFileClicked(QModelIndex)));
}

QtPad::~QtPad()
{
    delete m_ui;
}
void QtPad::s_newFile()
{
    m_currentFileName = "new";
    m_currentFilePath.clear();
    m_ui->plainTextEdit->clear();
    qDebug() << "New document (QtPad)";
}

void QtPad::s_openFile()
{
    m_currentFilePath = QFileDialog::getOpenFileName(this, "Choose your file");
    m_currentFileName = m_currentFilePath.split("/").back();
    openFile();
}

void QtPad::s_openFolder()
{
    m_currentFolderPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    m_fsModel = new QFileSystemModel(this);
    m_fsModel->setRootPath(m_currentFolderPath);
    m_ui->treeView->setModel(m_fsModel);
    m_ui->treeView->setRootIndex(m_fsModel->index(m_currentFolderPath));

    m_ui->treeView->hideColumn(1);
    m_ui->treeView->hideColumn(2);
    m_ui->treeView->hideColumn(3);
    m_ui->treeView->header()->hide();

    qDebug() << "dir: " << m_currentFolderPath;
}

void QtPad::s_saveFile()
{
    if (m_currentFilePath.isEmpty()){
        m_currentFilePath = QFileDialog::getSaveFileName(this, "save file name");
    }
    m_currentFileName = m_currentFilePath.split("/").back();

    QFile file(m_currentFilePath);

    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    setWindowTitle(m_currentFileName); //todo

    QTextStream out(&file);
    QString  text = m_ui->plainTextEdit->toPlainText();
    out << text;
    file.close();
    qDebug() << "Save document (save as)" << m_currentFilePath;
}

void QtPad::s_saveFileAs()
{
    m_currentFilePath = QFileDialog::getSaveFileName(this, "save file name");
    saveFile();
}

void QtPad::s_exitApp()
{
    qDebug() << "Exit app (QtPad)";
    exit(0);
}

void QtPad::s_treeFileClicked(const QModelIndex &index) {
    if (!m_fsModel->isDir(index))
    {
        m_currentFilePath = m_fsModel->filePath(index);
        m_currentFileName = m_fsModel->fileName(index);

        qDebug() << "Selected document m_currentFilePath: " << m_currentFilePath;
        qDebug() << "Selected document m_currentFileName: " << m_currentFileName;
        openFile();
    }
}


void QtPad::saveFile()
{
    QFile file(m_currentFilePath);

    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    QString  text = m_ui->plainTextEdit->toPlainText();
    out << text;
    file.close();
    qDebug() << "Saved document";
}

void QtPad::openFile()
{
    QFile file(m_currentFilePath);

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }

    setWindowTitle(m_currentFileName); //todo
    QTextStream in(&file);
    QString text = in.readAll();
    m_ui->plainTextEdit->setPlainText(text);
    file.close();

    qDebug() << "Open document m_currentFilePath: " << m_currentFilePath;
    qDebug() << "Open document m_currentFileName: " << m_currentFileName;
}

