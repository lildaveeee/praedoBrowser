#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.resize(1600, 1200);
    window.setWindowFlags(window.windowFlags() | Qt::FramelessWindowHint);
    window.show();

    return app.exec();
}
