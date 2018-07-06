#include "chartwindow.h"

#include <iostream>

ChartWindow::ChartWindow()
{
    axisX = new QtCharts::QValueAxis;
    axisY = new QtCharts::QValueAxis;
}

ChartWindow::ChartWindow(const std::map<size_t, qreal> &points) : ChartWindow() {
    addData(points);
}

void ChartWindow::setTitle(const QString& title) {
    chart()->setTitle(title);
}

QtCharts::QLineSeries* ChartWindow::addData(const std::map<size_t, qreal> &points) {
    auto series = new QtCharts::QLineSeries;
    chart()->addSeries(series);
    chart()->setAxisX(axisX, series);
    chart()->setAxisY(axisY, series);
    updateData(points, series);
    return series;
}

void ChartWindow::updateData(const std::map<size_t, qreal> &points, QtCharts::QLineSeries *series) {
    qreal maxX = 0;
    qreal minX = INT_MAX;
    qreal maxY = 0;
    qreal minY = INT_MAX;
    series->clear();
    for(auto st = points.begin(); st != points.end(); ++st) {
        const size_t& x = st->first;
        const size_t& y = st->second;
        series->append(x, y);
        if(x > maxX) maxX = x;
        if(x < minX) minX = x;
        if(y > maxY) maxY = y;
        if(y < minY) minY = y;
    }
    axisX->setTickCount(points.size()*2);
    axisY->setTickCount(points.size()*2);
    axisX->setRange(minX, maxX);
    axisY->setRange(minY, maxY);
}

void ChartWindow::updateData(size_t x, qreal y, QtCharts::QLineSeries *series) {
    qreal maxX = static_cast<QtCharts::QValueAxis>(chart()->axisX()).max();
    qreal minX = static_cast<QtCharts::QValueAxis>(chart()->axisX()).min();
    qreal maxY = static_cast<QtCharts::QValueAxis>(chart()->axisY()).max();
    qreal minY = static_cast<QtCharts::QValueAxis>(chart()->axisY()).min();
    if(x > maxX) maxX = x;
    if(x < minX) minX = x;
    if(y > maxY) maxY = y;
    if(y < minY) minY = y;
    series->append(x, y);
    this->repaint();
    axisX->setTickCount(15);
    axisY->setTickCount(15);
//    axisX->setTickCount(static_cast<QtCharts::QValueAxis>(chart()->axisY()).tickCount() + 1);
//    axisY->setTickCount(static_cast<QtCharts::QValueAxis>(chart()->axisY()).tickCount() + 1);
    axisX->setRange(minX, maxX);
    axisY->setRange(minY, maxY);
}
