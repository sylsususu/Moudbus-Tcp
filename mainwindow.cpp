#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"MyModbus/mymodbuscontrol.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //TODO 信号连接

    // 在需要监控连接状态的地方（如主窗口构造函数）
    QObject::connect(MyModbusControl::getInstance(), &MyModbusControl::signalConnectionState,
                     [](bool connected) {
                         if (connected) {
                             qDebug() << "Modbus连接已建立";

                         } else {
                             qDebug() << "Modbus连接已断开";

                         }
                     }
                     );

    connect(MyModbusControl::getInstance(), &MyModbusControl::signalHeartbeat,
            [](bool connected) {
                //qDebug()<<"心跳检测："<<connected;
            }
            );


}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    // 连接PLC
    MyModbusControl::getInstance()->connectToPLC("192.168.10.88",502);


}


void MainWindow::on_pushButton_2_clicked()
{

    MyModbusControl::getInstance()->disconnectFromPLC();

}


void MainWindow::on_pushButton_3_clicked()
{
    QList<int>address;
    for(int i=800;i<=834;i+=2)
    {
        address.push_back(i);
    }
    QMap<int,double> result;
    MyModbusControl::getInstance()->readFromPlc(address,result,1);

    qDebug() << "===== 寄存器读取结果 =====";
    for (auto it = result.constBegin(); it != result.constEnd(); ++it) {
        qDebug() << QString("地址: 0x%1  值: %2")
                        .arg(it.key(), 4, 16, QLatin1Char('0'))  // 4位16进制，前导0填充
                        .arg(it.value(), 0, 'f', 6);  // 固定6位小数
    }
    qDebug() << "=========================";

}


void MainWindow::on_pushButton_4_clicked()
{
    QMap<int,double>map;
    for(int i=800;i<=834;i+=2){
        map.insert(i,123.456);
    }
    MyModbusControl::getInstance()->writeToPlc(map,1);


}


void MainWindow::on_pushButton_5_clicked()
{
    QMap<int,double> result;
    MyModbusControl::getInstance()->readFromPlc(1102,result,1);

    qDebug() << "===== 寄存器读取结果 =====";
    for (auto it = result.constBegin(); it != result.constEnd(); ++it) {
        qDebug() << QString("地址: 0x%1  值: %2")
                        .arg(it.key(), 4, 16, QLatin1Char('0'))  // 4位16进制，前导0填充
                        .arg(it.value(), 0, 'f', 6);  // 固定6位小数
    }
    qDebug() << "=========================";
}




void MainWindow::on_pushButton_6_clicked()
{
    MyModbusControl::getInstance()->writeToPlc(202,1,1);
}




void MainWindow::on_pushButton_7_clicked()
{
    MyModbusControl::getInstance()->writeToPlc(500,"QWEASDZXC",1);
}

