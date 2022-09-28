#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);
    CodeEditor(QString openedFileName, QString openedFilePath, QWidget *parent = nullptr);
    ~CodeEditor();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    QString getFilePath() const;
    QString getFileName() const;
    void setFilePath(QString path);
    void setFileName(QString name);

private:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *m_lineNumberArea;
    QString m_filePath;
    QString m_fileName;
};

#endif // CODEEDITOR_H
