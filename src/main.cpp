#include <QApplication>

#include "dialog.h"
#include "resizeworker.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("White-walls");
    a.setApplicationName("Resizer3");

    qRegisterMetaType<ResizeWorker::Progress>("Progress");

    Dialog w;
    w.show();
    return a.exec();
}
