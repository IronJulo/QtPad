#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileSystemModel>

#include "codeeditor.h"
#include "qtpad.h"
#include "ui_qtpad.h"
#include "terminal.h"

QtPad::QtPad(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::QtPad)
{
    m_ui->setupUi(this);
    m_ui->tabWidget->setTabsClosable(true);

    connect(m_ui->actionNew, &QAction::triggered, this, &QtPad::s_newFile);
    connect(m_ui->actionOpen_File, &QAction::triggered, this, &QtPad::s_openFile);
    connect(m_ui->actionOpen_Folder, &QAction::triggered, this, &QtPad::s_openFolder);
    connect(m_ui->actionSave, &QAction::triggered, this, &QtPad::s_saveFile);
    connect(m_ui->actionSave_as, &QAction::triggered, this, &QtPad::s_saveFileAs);
    connect(m_ui->actionExit, &QAction::triggered, this, &QtPad::s_exitApp);
    connect(m_ui->tabWidget, &QTabWidget::tabCloseRequested, this, &QtPad::s_closeTab);

    //connect(m_ui->treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(s_treeFileClicked(QModelIndex))); ddduhhhh
    connect(m_ui->treeView, &QTreeView::doubleClicked, this, &QtPad::s_treeFileClicked);
    //connect(m_ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(s_tabChanged(int)));
    connect(m_ui->tabWidget, &QTabWidget::currentChanged, this, &QtPad::s_tabChanged);

    Terminal *term = new Terminal;
    /*term->setMinimumWidth(500);
    term->setMaximumHeight(50);*/

    m_ui->splitter_2->addWidget(term);
}

QtPad::~QtPad()
{
    delete m_ui;
}
void QtPad::s_newFile()
{
    addEditor("new", "");
    qDebug() << "New document (QtPad)";
}

void QtPad::s_openFile()
{
    QString openedFilePath =  QFileDialog::getOpenFileName(this, "Choose your file");
    QString oppenedFileName = openedFilePath.split("/").back();
    addEditor(oppenedFileName, openedFilePath);
    openFile();
    m_currentEditor->save();
}

void QtPad::s_openFolder()
{
    m_currentFolderPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/",
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
    if (m_currentEditor->isSaved()){
        return;
    }

    if (m_currentEditor->getFilePath().isEmpty()){
        m_currentEditor->setFilePath(QFileDialog::getSaveFileName(this, "save file name"));
    }
    saveFile();
    setWindowTitle(m_currentEditor->getFileName()); //todo
    m_ui->tabWidget->setTabText(m_ui->tabWidget->currentIndex(), m_currentEditor->getFileName());
    m_currentEditor->save();
    qDebug() << "Saved document: " << m_currentEditor->getFileName();
}

void QtPad::s_saveFileAs()
{
    m_currentEditor->setFilePath(QFileDialog::getSaveFileName(this, "save file name"));
    saveFile();
    setWindowTitle(m_currentEditor->getFileName()); //todo
    m_ui->tabWidget->setTabText(m_ui->tabWidget->currentIndex(), m_currentEditor->getFileName());
    m_currentEditor->save();
    qDebug() << "Saved document as: " << m_currentEditor->getFileName();
}

void QtPad::s_exitApp()
{
    qDebug() << "Exit app (QtPad)";
    exit(0);
}

void QtPad::s_treeFileClicked(const QModelIndex &index) {
    qDebug() << "open file";
    if (!m_fsModel->isDir(index))//TODO open new tab
    {
        QString openedFilePath = m_fsModel->filePath(index);
        QString openedFileName = m_fsModel->fileName(index);

        qDebug() << "Selected document m_currentFilePath: " << openedFilePath;
        qDebug() << "Selected document m_currentFileName: " << openedFileName;
        addEditor(openedFileName, openedFilePath);
        openFile();
        m_currentEditor->save();
    }
}
void QtPad::s_tabChanged(int index)
{
    if (index < 0)
    {
        return;
    }
    m_currentEditor = (CodeEditor*) m_ui->tabWidget->widget(index); //TODO security checks
    setWindowTitle(m_currentEditor->getFileName()); //todo
    //m_ui->tabWidget->setTabText(index, m_currentEditor->getFileName());
    qDebug() << "Selected tab: " << index;
}

void QtPad::s_closeTab(int index)
{
    m_ui->tabWidget->setCurrentIndex(index);
    m_currentEditor = (CodeEditor*) m_ui->tabWidget->widget(index);
    if (!m_currentEditor->isSaved()){
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
        msgBox.setDefaultButton(QMessageBox::Save);

        int ret = msgBox.exec();
        if (ret == QMessageBox::Save){
            if (m_currentEditor->getFilePath().isEmpty()){
                m_currentEditor->setFilePath(QFileDialog::getSaveFileName(this, "save file name"));
            }
            saveFile();
        }
    }

    m_ui->tabWidget->removeTab(index);
    if(m_ui->tabWidget->count() != 0){
        m_currentEditor = (CodeEditor*) m_ui->tabWidget->currentWidget();
    }else {
        m_currentEditor = nullptr;
    }
}

void QtPad::saveFile() //TODO security checks
{
    if (m_currentEditor->isSaved()){
        return;
    }
    QFile file(m_currentEditor->getFilePath());
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    QTextStream out(&file);
    QString  text = m_currentEditor->toPlainText();
    out << text;
    file.close();
    m_currentEditor->save();
}

void QtPad::openFile()//TODO security checks
{
    QFile file(m_currentEditor->getFilePath());

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    m_currentEditor->setPlainText(text);
    file.close();
}

void QtPad::addEditor(QString openedFileName, QString openedFilePath)
{
    CodeEditor *editor = new CodeEditor(openedFileName, openedFilePath, this);
    m_currentEditor = editor;
    m_ui->tabWidget->addTab(editor,  openedFileName);
    m_ui->tabWidget->setCurrentIndex(m_ui->tabWidget->count()-1);

}
