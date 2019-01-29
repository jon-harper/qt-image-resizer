#include "debugbox.h"
#include <stdio.h>

DebugBox::DebugBox(QWidget *parent) :
    QTextEdit(parent)
{
    setReadOnly(true);
    setAcceptRichText(true);
    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Sunken);
}

DebugBox::~DebugBox()
{

}

DebugBox::DebugBox(const QString &text, QWidget *parent) :
    DebugBox(parent)
{
    setText(text);
}

void DebugBox::postMessage(QtMsgType type, const char *msg)
{
    switch (type) {
    case QtInfoMsg:
        [[fallthrough]];
    case QtDebugMsg:
        append(QString(msg));
        break;
    case QtWarningMsg:
        append(QString("Warning: ").append(QString(msg)));
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s\n", msg);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", msg);
        abort();
    }
}
