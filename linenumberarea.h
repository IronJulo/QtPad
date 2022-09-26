#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>

class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    explicit LineNumberArea(CodeEditor *editor);

signals:

};

#endif // LINENUMBERAREA_H
