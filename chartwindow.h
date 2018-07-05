#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QVector>
#include <climits>
#include <map>

#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H


class ChartWindow : public QtCharts::QChartView
{
    Q_OBJECT
public:
    ChartWindow();
    ChartWindow(const std::map<size_t, qreal>& points);
    void setTitle(const QString&);
    QtCharts::QLineSeries* addData(const std::map<size_t, qreal>& points);
    void updateData(const std::map<size_t, qreal>& points, QtCharts::QLineSeries *series);
    void updateData(size_t x, qreal y, QtCharts::QLineSeries *series);
private:
    QtCharts::QValueAxis *axisX;
    QtCharts::QValueAxis *axisY;
};

#endif // CHARTWINDOW_H
