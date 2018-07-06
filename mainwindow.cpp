#include <QMessageBox>
#include "mainwindow.h"
#include "settingsdialog.h"
#include "ui_mainwindow.h"

#include <map>
#include "fouriertransformation.cpp"
#include "integration.cpp"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings(new SettingsDialog),
    serial(new QSerialPort),
    chart(new ChartWindow)
{
    chart->setTitle(tr("Function of data received from COM-port"));
    ui->setupUi(this);
    chartOptions = new QComboBox();
    integralSum = new QLabel("");
    chartOptions->addItem("Function of data");
    chartOptions->addItem("Fourier Transformation of function");
    connect(chartOptions, SIGNAL(currentIndexChanged(int)), this, SLOT(changeChart(int)));
    ui->toolBar->addWidget(chartOptions);
    ui->toolBar->addWidget(integralSum);
    connect(ui->actionRun, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionSettings, &QAction::triggered, settings, &SettingsDialog::show);
    setCentralWidget(chart);

// If u r Roman and don't have STM uncomment below
//    map_x = std::map<size_t, qreal>({{0,0.0}, {1, 11.0}, {2, 7.0}, {3, 15.0}, {4, 10.0}, {5, 8.0}, {6, 7.0}, {7, 10.0}});
//    map_y = std::map<size_t, qreal>({{0,1}, {5, 9}, {6, 8}, {7, 11}});
//    seriesX = chart->addData(map_x);
//    seriesY = chart->addData(map_y);
//    seriesZ = chart->addData(map_z);
//    this->repaint();

//    timer = new QTimer();
//    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//    timer->start(1000);
}

void MainWindow::changeChart(int index) {
//    std::cout << index << std::endl;
    if(!fourier) {
        fourier = new ChartWindow();
        fseriesX = fourier->addData(ft(map_x));
        fseriesY = fourier->addData(ft(map_y));
        fseriesZ = fourier->addData(ft(map_z));
    }
    if(index == 0) {
        chart->setTitle(tr("Function of data received from COM-port"));
        setCentralWidget(chart);
    } else if(index == 1) {
        chart->setTitle(tr("Fourier Transformation of function"));
        setCentralWidget(fourier);
    }
    QtCharts::QLineSeries *tmp_seriesX, *tmp_seriesY, *tmp_seriesZ;
    tmp_seriesX = seriesX;
    seriesX = fseriesX;
    fseriesX = tmp_seriesX;
    tmp_seriesY = seriesY;
    seriesY = fseriesY;
    fseriesY = tmp_seriesY;
    tmp_seriesZ = seriesZ;
    seriesZ = fseriesZ;
    fseriesZ = tmp_seriesZ;
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
    integralSum->setText(" Integral x: " + QString::number(integrate(map_x)) +
                         " Integral y: " + QString::number(integrate(map_y)) +
                         " Integral z: " + QString::number(integrate(map_z)));
    chart->updateData(map_x, seriesX);
    chart->updateData(map_y, seriesY);
    chart->updateData(map_z, seriesZ);
    chart->repaint();
    this->repaint();
}
void MainWindow::openSerialPort(){
    delete chart;
    chart = new ChartWindow();
    setCentralWidget(chart);
    this->repaint();
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
//    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//    timer->start(3000);
    while (serial->isOpen())
    {
        size_t y = 0, time = 0;
        QByteArray data = serial->readLine();
        QString DataAsString = QTextCodec::codecForMib(106)->toUnicode(data);
        line = DataAsString.split(' ', QString::SkipEmptyParts);
        qDebug() << line;
        qDebug() << line.length();
        size_t len = line.length();

        if(len == 0) {break;}
        else if (len != 12) {continue;}

        if (start) {
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
//        chart->updateData(time, x_1, seriesX);
//        chart->updateData(time, y_1, seriesY);
//        chart->updateData(time, z_1, seriesZ);
        this->update();
        this->repaint();
    }

    qDebug() << "end while";
//    std::map<size_t, qreal> *x, *y, *z;
//    x = &map_x, y = &map_y, z = &map_z;
//    delete x, delete y, delete z;
    timer->stop();
//    map_x.clear(), map_y.clear(), map_z.clear();
    qDebug() << "end memory free";
    serial->close();
}
