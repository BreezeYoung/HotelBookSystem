#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include"commonhelper.h"
#include"platform.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //加载QSS样式
    CommonHelper::setStyle(":/csdnstyle.qss",a);


    Platform *p=new Platform;
    p->ReadDatabase();

    MainWindow w(p);
    w.setup();
    a.exec();

    p->WriteIntoDatabase();
    p->clear();
    delete p;
    return 0;
}
