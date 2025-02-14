#include "mainwindow.h"
#include "ad.h"

#include <QApplication>
#include <iostream>

using namespace std;



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //ad AD;
    //AD.show();
    return a.exec();
}

