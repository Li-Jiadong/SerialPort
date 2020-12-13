#ifndef MYCHART_H
#define MYCHART_H
#include <QtCharts>
#include <QDebug>

class MainWindow;

class MyChart : public QChartView
{
public:
    MyChart();
    MyChart(QWidget*&);
    void getMain(MainWindow *m);
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
private:
    bool is_pressed;
    QPoint startPoint;
    MainWindow *m;
};

#endif // MYCHART_H
