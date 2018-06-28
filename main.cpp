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

std::vector<std::map<size_t, qreal>> port_read(QByteArray d_time)
{
    std::map<size_t, qreal> map_x, map_y, map_z;
    qreal x_1 = 0.0, y_1 = 0.0, z_1 = 0.0;
    QSerialPort serial;
    size_t start_time;
    bool start = true;
    QStringList line;
    serial.setPortName("/dev/cu.usbmodem14121");
    serial.setBaudRate(QSerialPort::Baud57600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.open(QIODevice::ReadWrite);
    serial.write(d_time);
    serial.flush();
    serial.waitForBytesWritten(1000);
    serial.waitForReadyRead(1000);
    while (serial.canReadLine())
    {
        size_t y;
        QByteArray data = serial.readLine();
        QString DataAsString = QTextCodec::codecForMib(106)->toUnicode(data);
        std::cout << DataAsString.toStdString() << std::endl;
        line = DataAsString.split(' ', QString::SkipEmptyParts);
        if (start){
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
        std::cout << time << std::endl;
        map_x[time] = x_1;
        map_y[time] = y_1;
        map_z[time] = z_1;
    }
    std::vector<std::map<size_t, qreal>> params_vector {map_x, map_y, map_z};
    return params_vector;
}

int main()
{
    std::vector<std::map<size_t, qreal>> a;
    a = port_read("27/06/2018 17:42:33");
    for (auto const& x: a[0])
        std::cout << x.first << ":" << x.second << std::endl;
    return 0;
}
