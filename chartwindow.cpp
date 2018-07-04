#include "chartwindow.h"

#include <iostream>

ChartWindow::ChartWindow()
{
    axisX = new QtCharts::QValueAxis;
    axisY = new QtCharts::QValueAxis;
}

ChartWindow::ChartWindow(const std::map<int, qreal> &points) : ChartWindow() {
    addData(points);
}

void ChartWindow::setTitle(const QString& title) {
    chart()->setTitle(title);
}

QtCharts::QLineSeries* ChartWindow::addData(const std::map<int, qreal> &points) {
    auto series = new QtCharts::QLineSeries;
    chart()->addSeries(series);
    chart()->setAxisX(axisX, series);
    chart()->setAxisY(axisY, series);
    updateData(points, series);
    return series;
}

void ChartWindow::updateData(const std::map<int, qreal> &points, QtCharts::QLineSeries *series) {
    qreal maxX = INT_MIN;
    qreal minX = INT_MAX;
    qreal maxY = INT_MIN;
    qreal minY = INT_MAX;
    series->clear();
    for(auto st = points.begin(); st != points.end(); ++st) {
        const int& x = st->first;
        const int& y = st->second;
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
