#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QObject>

class CodeEditor : public QObject
{
    Q_OBJECT
public:
    explicit CodeEditor(QObject *parent = nullptr);

signals:

};

#endif // CODEEDITOR_H
