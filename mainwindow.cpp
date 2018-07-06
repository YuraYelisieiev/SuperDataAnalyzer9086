#include <QMessageBox>
#include "mainwindow.h"
#include "settingsdialog.h"
#include "ui_mainwindow.h"

#include <map>
#include "fouriertransformation.cpp"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings(new SettingsDialog),
    serial(new QSerialPort),
    chart(new ChartWindow)
{
    ui->setupUi(this);
    chartOptions = new QComboBox();
    chartOptions->addItem("Chart of Data");
    chartOptions->addItem("Chart of Fourier Transformated Data");
    connect(chartOptions, SIGNAL(currentIndexChanged(int)), this, SLOT(changeChart(int)));
    ui->toolBar->addWidget(chartOptions);
    connect(ui->actionRun, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionSettings, &QAction::triggered, settings, &SettingsDialog::show);
    setCentralWidget(chart);

    map_x = std::map<size_t, qreal>({{0,0.0}, {1, 11.0}, {2, 7.0}, {3, 15.0}, {4, 10.0}, {5, 8.0}, {6, 7.0}, {7, 10.0}});
    map_y = std::map<size_t, qreal>({{0,1}, {5, 9}, {6, 8}, {7, 11}});
    chart->addData(map_x);
    chart->addData(map_y);
    chart->addData(map_z);
    this->repaint();
}

void MainWindow::changeChart(int index) {
//    std::cout << index << std::endl;
    delete chart;
    chart = new ChartWindow();
    if(index == 0) {
        chart->addData(map_x);
        chart->addData(map_y);
        chart->addData(map_z);
    } else if(index == 1) {
        auto tmp_map_x = ft(map_x);
        auto tmp_map_y = ft(map_y);
        auto tmp_map_z = ft(map_z);
        chart->addData(tmp_map_x);
        chart->addData(tmp_map_y);
        chart->addData(tmp_map_z);
    }
    setCentralWidget(chart);
//    chart->repaint();
    this->repaint();
}

MainWindow::~MainWindow(){
    delete chart;
    delete settings;
    delete serial;
    delete configure;
    delete start;
    delete chartOptions;
    delete timer;
    delete ui;
}

void MainWindow::writeData(const QByteArray &data){
    serial->write(data);
}

void MainWindow::update() {
    chart->updateData(map_x, seriesX);
    chart->updateData(map_y, seriesY);
    chart->updateData(map_z, seriesZ);
    chart->repaint();
    this->repaint();
}
void MainWindow::openSerialPort(){
//    std::map<int, int> p1oints{{0,1}, {5, 9}, {6, 8}, {7, 11}};
//    std::cout << chart->chart()->series().size() << std::endl;
//    QtCharts::QLineSeries* series =   static_cast<QtCharts::QLineSeries*>(chart->chart()->series()[0]);
//    chart->updateData(p1oints, series);
    const SettingsDialog::Settings p = settings->settings();
//    chart = new ChartWindow();
//    setCentralWidget(chart);
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    serial->open(QIODevice::ReadWrite);
    if(!serial->isOpen()) {
        QMessageBox msg;
        msg.setText("Cannot open Device for COM-port");
        msg.exec();
        return;
    }
    writeData("06/07/2018 01:52:33");
    serial->flush();
    serial->waitForBytesWritten(1000);
    serial->waitForReadyRead(1000);
    serial->setBreakEnabled(false);
//    std::map<size_t, qreal> map_x, map_y, map_z;
    qreal x_1 = 0.0, y_1 = 0.0, z_1 = 0.0;
    size_t start_time;
    bool start = true;
    QStringList line;

    seriesX = chart->addData(map_x);
    seriesY = chart->addData(map_y);
    seriesZ = chart->addData(map_z);
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
    while (serial->isOpen())
    {
        size_t y = 0, time = 0;
        QByteArray data = serial->readLine();
        QString DataAsString = QTextCodec::codecForMib(106)->toUnicode(data);
        line = DataAsString.split(' ', QString::SkipEmptyParts);
        if (line.length() != 12) break;
        if (start)
        {
            start_time = line[2].toULongLong();
            start = false;
        }
        else
        {
            y = line[2].toULongLong();
            time = y - start_time;
        }
        x_1 += line[3].toDouble();
        y_1 += line[4].toDouble();
        z_1 += line[5].toDouble();
        qDebug() << x_1 << " " << y_1 << " " << z_1;
        map_x[time] = x_1;
        map_y[time] = y_1;
        map_z[time] = z_1;
        chart->updateData(time, x_1, seriesX);
        chart->updateData(time, y_1, seriesY);
        chart->updateData(time, z_1, seriesZ);
        this->repaint();
    }
}
