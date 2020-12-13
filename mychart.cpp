#include "mychart.h"

MyChart::MyChart()
{

}

MyChart::MyChart(QWidget*& w):QChartView (w){
    is_pressed=0;
}

void MyChart::mouseMoveEvent(QMouseEvent *e){
    QPoint curpos=e->pos();
    if(is_pressed){
        QPoint offset = curpos - startPoint;
        startPoint = curpos;
        this->chart()->scroll(-offset.x(), offset.y());
    }
}
void MyChart::mousePressEvent(QMouseEvent *e){
    if(e->button()==Qt::LeftButton){
        is_pressed=1;
        startPoint=e->pos();
    }
}
void MyChart::mouseReleaseEvent(QMouseEvent *e){
    is_pressed=0;
}
void MyChart::getMain(MainWindow *m){
    this->m=m;
}
