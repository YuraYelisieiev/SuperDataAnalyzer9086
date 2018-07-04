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
#include <QApplication>
#include <QPushButton>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
