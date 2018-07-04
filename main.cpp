#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QVector>
#include <numeric>
#include <QTextCodec>
#include <QSerialPort>
#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
