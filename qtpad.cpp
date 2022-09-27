#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileSystemModel>

#include "codeeditor.h"
//#include "mytabpage.h"
#include "qtpad.h"
#include "ui_qtpad.h"

QtPad::QtPad(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::QtPad)
{
    m_ui->setupUi(this);
    m_ui->tabWidget->setTabsClosable(true);

    m_ui->tabWidget->removeTab(0);
    m_ui->tabWidget->removeTab(0);

    m_ui-> tabWidget ->addTab(new CodeEditor, tr("name2"));
    m_ui-> tabWidget ->addTab(new CodeEditor, tr("name1"));

    connect(m_ui->actionNew, &QAction::triggered, this, &QtPad::s_newFile);
    connect(m_ui->actionOpen_File, &QAction::triggered, this, &QtPad::s_openFile);
    connect(m_ui->actionOpen_Folder, &QAction::triggered, this, &QtPad::s_openFolder);
    connect(m_ui->actionSave, &QAction::triggered, this, &QtPad::s_saveFile);
    connect(m_ui->actionSave_as, &QAction::triggered, this, &QtPad::s_saveFileAs);
    connect(m_ui->actionExit, &QAction::triggered, this, &QtPad::s_exitApp);

    connect(m_ui->treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(s_treeFileClicked(QModelIndex)));
    connect(m_ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(s_tabChanged(int)));
}

QtPad::~QtPad()
{
    delete m_ui;
}
void QtPad::s_newFile()
{
    /*m_currentFileName = "new";
    m_currentFilePath.clear();
    m_ui->plainTextEdit->clear();
    qDebug() << "New document (QtPad)";*/
}

void QtPad::s_openFile()
{
    /*m_currentFilePath = QFileDialog::getOpenFileName(this, "Choose your file");
    m_currentFileName = m_currentFilePath.split("/").back();
    openFile();*/
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
    if (m_currentEditor->getFileName().isEmpty()){
        m_currentEditor->setFilePath(QFileDialog::getSaveFileName(this, "save file name"));
    }
    //m_currentFileName = m_currentFilePath.split("/").back();

    QFile file(m_currentEditor->getFileName());

    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    //setWindowTitle(m_currentFileName); //todo

    QTextStream out(&file);
    QString  text = m_ui->plainTextEdit->toPlainText();
    out << text;
    file.close();
    //qDebug() << "Save document (save as)" << m_currentFilePath;
}

void QtPad::s_saveFileAs()
{
    m_currentEditor->setFilePath(QFileDialog::getSaveFileName(this, "save file name"));
    saveFile();
}

void QtPad::s_exitApp()
{
    qDebug() << "Exit app (QtPad)";
    exit(0);
}

void QtPad::s_treeFileClicked(const QModelIndex &index) {
    if (!m_fsModel->isDir(index))//TODO open new tab
    {
        /*m_currentFilePath = m_fsModel->filePath(index);
        m_currentFileName = m_fsModel->fileName(index);

        qDebug() << "Selected document m_currentFilePath: " << m_currentFilePath;
        qDebug() << "Selected document m_currentFileName: " << m_currentFileName;
        openFile();*/
    }
}
void QtPad::s_tabChanged(int index)
{
    m_currentEditor = (CodeEditor*) m_ui->tabWidget->widget(index); //TODO security checks
    qDebug() << "Selected tab: " << index;
}

void QtPad::saveFile() //TODO security checks
{
    QFile file(m_currentEditor->getFilePath());

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

void QtPad::openFile()//TODO security checks
{
    QFile file(m_currentEditor->getFilePath());

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }

    setWindowTitle(m_currentEditor->getFileName()); //todo
    QTextStream in(&file);
    QString text = in.readAll();
    m_ui->plainTextEdit->setPlainText(text); //todo
    file.close();
}

