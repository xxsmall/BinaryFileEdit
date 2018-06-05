#include "mainwindow.h"
#include <QApplication>
#include<QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QTranslator *translator = new QTranslator;
    //2018
    //translator->load("/home/桌面/TestHello/TestHello.qm");
    //a.installTranslator(translator);
    //
    MainWindow w;
    w.show();
    
    return a.exec();
}
