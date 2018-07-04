#include "mainwindow.h"
#include "settingsdialog.h"
#include "ui_mainwindow.h"

#include <map>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings(new SettingsDialog),
    serial(new QSerialPort)
{
    ui->setupUi(this);
    connect(ui->actionRun, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionSettings, &QAction::triggered, settings, &SettingsDialog::show);

//    std::map<int, qreal> points{{0,0.0}, {1, 11.0}, {2, 7.0}, {3, 15.0}, {4, 10.0}, {5, 8.0}, {6, 7.0}, {7, 10.0}};
    chart = new ChartWindow();
//    chart->addData(points);
    setCentralWidget(chart);
}

MainWindow::~MainWindow(){
    delete settings;
    delete ui;
}

void MainWindow::writeData(const QByteArray &data){
    serial->write(data);
}


void MainWindow::openSerialPort(){
//    std::map<int, int> p1oints{{0,1}, {5, 9}, {6, 8}, {7, 11}};
//    std::cout << chart->chart()->series().size() << std::endl;
//    QtCharts::QLineSeries* series =   static_cast<QtCharts::QLineSeries*>(chart->chart()->series()[0]);
//    chart->updateData(p1oints, series);
    const SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    serial->open(QIODevice::ReadWrite);
    writeData("30/06/2018 15:01:33");
    serial->flush();
    serial->waitForBytesWritten(1000);
    serial->waitForReadyRead(1000);
    serial->setBreakEnabled(false);
    std::map<int, qreal> map_x, map_y, map_z;
    qreal x_1 = 0.0, y_1 = 0.0, z_1 = 0.0;
    size_t start_time;
    bool start = true;
    QStringList line;

    auto seriesX = chart->addData(map_x);
    auto seriesY = chart->addData(map_y);
    auto seriesZ = chart->addData(map_z);
    while (serial->isOpen())
    {
        size_t y;
        QByteArray data = serial->readLine();
        QString DataAsString = QTextCodec::codecForMib(106)->toUnicode(data);
        line = DataAsString.split(' ', QString::SkipEmptyParts);
        if (line.length() != 12) continue;
        if (start)
        {
            start_time = line[2].toULongLong();
            y = 0;
            start = false;
        }
        else
            y = line[2].toULongLong();
        x_1 += line[3].toDouble();
        y_1 += line[4].toDouble();
        z_1 += line[5].toDouble();
        size_t time = y - start_time;
        map_x[time] = x_1;
        map_y[time] = y_1;
        map_z[time] = z_1;
        chart->updateData(map_x, seriesX);
        chart->updateData(map_y, seriesY);
        chart->updateData(map_z, seriesZ);
    }
    //here we need to add another method instead of the map
    std::vector<std::map<int, qreal>> params_vector {map_x, map_y, map_z};
//    return params_vector;
}
