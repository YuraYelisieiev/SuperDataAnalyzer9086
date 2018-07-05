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
#include "fouriertransformation.cpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
//    std::map<int, qreal> points{{0,0.0}, {1, 11.0}, {2, 7.0}, {3, 15.0}, {4, 10.0}, {5, 8.0}, {6, 7.0}, {7, 10.0}};
//    std::map<int, qreal> points{{0, 0.0}, {1, 0.707}, {2, 1.0}, {3, 0.707}, {4, 0.0}, {5, -0.707}, {6, -1.0}, {7.0, -0.707}};
//    ft(points);
    return app.exec();
}
