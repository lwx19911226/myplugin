#include <QtGui/QApplication>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTranslator qtTranslator;
    qtTranslator.load("myplugin.qm");
    a.installTranslator(&qtTranslator);
    MainWindow w;
    w.show();    
    return a.exec();
}
