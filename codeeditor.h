#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QString openedFileName, QString openedFilePath, QWidget *parent = nullptr);
    ~CodeEditor();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    QString getFilePath() const;
    QString getFileName() const;
    void setFilePath(QString path);
    void setFileName(QString name);

    void save();
    bool isSaved();

private:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void s_updateLineNumberAreaWidth(int newBlockCount);
    void s_highlightCurrentLine();
    void s_updateLineNumberArea(const QRect &rect, int dy);
    void s_textEdited();

private:
    QWidget *m_lineNumberArea;
    QString m_filePath;
    QString m_fileName;
    bool m_saved;
};

#endif // CODEEDITOR_H
