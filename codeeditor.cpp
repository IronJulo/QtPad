#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>
#include <QDebug>

#include "codeeditor.h"
#include "linenumberarea.h"

CodeEditor::CodeEditor(QString openedFileName, QString openedFilePath, QWidget *parent)
    : QPlainTextEdit(parent)
{
    m_lineNumberArea = new LineNumberArea(this);
    m_fileName = openedFileName;
    m_filePath = openedFilePath;
    m_saved = true;
    setPlainText("");

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::s_updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::s_updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::s_highlightCurrentLine);
    connect(this, &CodeEditor::textChanged, this, &CodeEditor::s_textEdited);

    s_updateLineNumberAreaWidth(0);
    s_highlightCurrentLine();
}

CodeEditor::~CodeEditor()
{
 qDebug() << "deleting code editor";;
}
void CodeEditor::save()
{
    m_saved=true;
}
bool CodeEditor::isSaved()
{
    return m_saved;
}
QString CodeEditor::getFilePath() const
{
    return m_filePath;
}
QString CodeEditor::getFileName() const
{
    return m_fileName;
}
void CodeEditor::setFilePath(QString path)
{
    m_filePath = path;
    setFileName(getFilePath().split("/").back());
}
void CodeEditor::setFileName(QString name)
{
    m_fileName = name;
}
int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::s_updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::s_updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        m_lineNumberArea->scroll(0, dy);
    else
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        s_updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::s_highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, m_lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
void CodeEditor::s_textEdited()
{
    m_saved = false;
}
