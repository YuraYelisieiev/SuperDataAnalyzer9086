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
private:
    Ui::MainWindow *ui = nullptr;
    SettingsDialog *settings = nullptr;
    QSerialPort *serial = nullptr;
    QPushButton *configure = nullptr;
    QPushButton *start = nullptr;
    ChartWindow *chart = nullptr;
    QTimer *timer = nullptr;
};
#endif // MAINWIDGET_H
