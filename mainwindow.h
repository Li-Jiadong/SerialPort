#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QStringList>
#include <QByteArray>
#include <QtCharts>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QPixmap>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString hello;

private slots:

    void receiveInfo();

    void on_c_portchoose_currentIndexChanged(const QString &arg1);

    void on_c_ratechoose_currentIndexChanged(int index);

    void on_bt_more_clicked();

    void on_checkBox_stateChanged(int arg1);

    void openfile();

    void savefile();

    void save_asfile();

    void savepic();

    void on_b_send_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QStringList serialPortName;
    QChart *chart;
    QLineSeries *series;
    qint64 startime;
    QValueAxis *axisX;
    QValueAxis *axisY;
    qreal xMax;
    qreal xRange;
    qreal yMax;
    qreal yMin;
    bool moveCursor;
    QFile *file;
};

#endif // MAINWINDOW_H
