#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sprite sprite;
    MainWindow w(sprite);
    w.show();
    return a.exec();
}
