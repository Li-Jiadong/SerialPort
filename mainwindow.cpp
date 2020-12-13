#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hello="hello world";
    ui->g_chart->getMain(this);

    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        ui->c_portchoose->addItem(info.portName());
        qDebug()<<"serialPortName:"<<info.portName();
    }
    serial=new QSerialPort;
    chart=new QChart;
    series=new QLineSeries;
    axisX=new QValueAxis;
    axisY=new QValueAxis;
    file=new QFile;
    xMax=50000;
    xRange=50000;
    yMax=5;
    yMin=0;
    moveCursor=0;
//    for (quint32 i = 0; i < 100; i++) {
//        series->append(i, sin(0.6*i));

//    }
    chart->addSeries(series);
    axisX->setMin(xMax-xRange);
    axisX->setMax(xMax);

    axisY->setMin(yMin);
    axisY->setMax(yMax);

    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    ui->g_chart->setChart(chart);
    ui->g_chart->setRenderHint(QPainter::Antialiasing);

}

MainWindow::~MainWindow()
{
    serial->close();
    delete serial;
    delete ui;
    if(file->isOpen()){
        file->close();

    }
    delete file;
}
void MainWindow::receiveInfo(){
    QByteArray barray=serial->readAll();
    qDebug()<<barray.toDouble()<<endl;
    ui->t_output->insertPlainText(barray);

    qreal x=QDateTime::currentMSecsSinceEpoch()-startime;
    qreal y=barray.toDouble();
    if(y>yMax){
        yMax=y;
        axisY->setMax(yMax);
    }
    if(y<yMin){
        yMin=y;
        axisY->setMin(yMin);
    }
    if(moveCursor){
        if(x>xMax){
            axisX->setMax(x);
            axisX->setMin(x-xRange);
            xMax=x;
        }
    }

    series->append(x,y);
}

void MainWindow::on_c_portchoose_currentIndexChanged(const QString &arg1)
{
    qDebug()<<"hello world "<<arg1<<endl;
    if(serial->isOpen()){
        serial->clear();
        serial->close();

    }
    serial->setPortName(arg1);
    if(!serial->open(QIODevice::ReadWrite)){
        qDebug()<<"open error"<<endl;
        return;
    }else{
        serial->setDataTerminalReady(true);
        serial->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        connect(serial,SIGNAL(readyRead()),this,SLOT(receiveInfo()));
        startime=QDateTime::currentMSecsSinceEpoch();
    }

}

void MainWindow::on_c_ratechoose_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        serial->setBaudRate(QSerialPort::Baud1200);
        break;
    case 1:
        serial->setBaudRate(QSerialPort::Baud2400);
        break;
    case 2:
        serial->setBaudRate(QSerialPort::Baud4800);
        break;
    case 3:
        serial->setBaudRate(QSerialPort::Baud9600);
        break;
    case 4:
        serial->setBaudRate(QSerialPort::Baud19200);
        break;
    case 5:
        serial->setBaudRate(QSerialPort::Baud38400);
        break;
    case 6:
        serial->setBaudRate(QSerialPort::Baud57600);
        break;
    case 7:
        serial->setBaudRate(QSerialPort::Baud115200);
        break;
    }
}

void MainWindow::on_bt_more_clicked()
{

}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    moveCursor=arg1;
}

void MainWindow::openfile(){
    if(file->isOpen()){
        file->close();
    }
    QFileDialog *qopenfile=new QFileDialog;
    qopenfile->setWindowTitle("open file");
    qopenfile->setDirectory(".");
    qopenfile->setNameFilter("*.txt");
    qopenfile->setViewMode(QFileDialog::Detail);
    QString path;
    if(qopenfile->exec()){
        path=qopenfile->selectedFiles()[0];
        file->setFileName(path);
        file->open(QIODevice::ReadWrite);
        QByteArray b=file->readAll();
        ui->t_output->setText(QString(b));

        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    delete qopenfile;
}

void MainWindow::savefile(){

    qDebug()<<file<<endl;
    if(file->isOpen()){
        QString s=ui->t_output->toPlainText();
        file->write(s.toUtf8());
        file->flush();
    }else{
        QString path=QFileDialog::getSaveFileName(this,"save file",".","Text fiels(*.txt)");
        if(path.isEmpty())
            return;
        file=new QFile(path);
        file->open(QIODevice::ReadWrite);
        QString s=ui->t_output->toPlainText();
        file->write(s.toUtf8());
        file->flush();

    }
}

void MainWindow::save_asfile(){
    QString path=QFileDialog::getSaveFileName(this,"save file",".","Text fiels(*.txt)");
    if(path.isEmpty())
        return;
    file=new QFile(path);
    file->open(QIODevice::ReadWrite);
    QString s=ui->t_output->toPlainText();
    file->write(s.toUtf8());
    file->flush();
}

void MainWindow::savepic(){
    QString path=QFileDialog::getSaveFileName(this,"save file",".","Picture(*.png)");
    if(path.isEmpty())
        return;
    QScreen * screen = QGuiApplication::primaryScreen();
    QPixmap p = screen->grabWindow(ui->g_chart->winId());
    QImage image = p.toImage();
    image.save(path);
}

void MainWindow::on_b_send_clicked()
{
    QString data=ui->t_send->toPlainText();
    QByteArray b=data.toUtf8();
    serial->write(b);
    ui->t_send->setText("");
}
