
#include <QApplication>
#include <QScreen>
#include <QClipboard>

#include "editor.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QObject::connect(&app, &QApplication::aboutToQuit, []() {
        QApplication::clipboard()->clear();
    });
    Editor e;
    e.move(e.screen()->availableGeometry().topLeft() + QPoint(20, 20));
    e.show();
    return app.exec();
}


