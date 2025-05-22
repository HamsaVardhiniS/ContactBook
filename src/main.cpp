#include "mainwindow.h"
#include <QApplication>
#include<QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir currentDir = QDir::current();
    qDebug() << "Current directory:" << currentDir.absolutePath();
    MainWindow w;
    w.show();
    return a.exec();
}
