#include <QWidget>
#include <QSerialPort>
#include <QHBoxLayout>
#include <QPushButton>
#include <QObject>
#include <QApplication>
#include <QTextCodec>
#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QComboBox>
#include <QLabel>

#include "chartwindow.h"
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class SettingsDialog;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void openSerialPort();
    void writeData(const QByteArray &data);
    void update();
    void changeChart(int index);
private:
    Ui::MainWindow *ui = nullptr;
    SettingsDialog *settings = nullptr;
    QSerialPort *serial = nullptr;
    QPushButton *configure = nullptr;
    QPushButton *start = nullptr;
    QComboBox *chartOptions = nullptr;
    ChartWindow *chart = nullptr;
    QLabel *integralSum = nullptr;
    QTimer *timer = nullptr;
    std::map<size_t, qreal> map_x, map_y, map_z;
    std::map<size_t, qreal> tmp_map_x, tmp_map_y, tmp_map_z;
    QtCharts::QLineSeries *seriesX, *seriesY, *seriesZ;
};
#endif // MAINWIDGET_H
