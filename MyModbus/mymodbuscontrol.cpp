#include "mymodbuscontrol.h"


MyModbusControl* MyModbusControl::instance = nullptr;
QMutex MyModbusControl::mutex;

MyModbusControl::MyModbusControl(QObject *parent)
    : QObject(parent), modbus(MyModbus::getInstance())
{

    // 连接状态变化
    QObject::connect(modbus, &MyModbus::connectionResult, [this](MyModbus::OperationStatus status) {
        if(status==MyModbus::OperationStatus::Success)
        {
            emit signalConnectionState(true);
        }else
            {
            emit signalConnectionState(false);
        }
    });

    // 心跳信号
    QObject::connect(modbus, &MyModbus::heartbeat, [this](bool connected) {
        emit signalHeartbeat(connected);
    });


}

MyModbusControl::~MyModbusControl()
{
    MyModbus::releaseInstance();
}

MyModbusControl *MyModbusControl::getInstance()
{
    QMutexLocker locker(&mutex);
    if (instance == nullptr) {
        instance = new MyModbusControl();
    }
    return instance;
}

void MyModbusControl::releaseInstance()
{
    QMutexLocker locker(&mutex);
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

void MyModbusControl::connectToPLC(const QString &ip, int port)
{

    modbus->connectToPLC(ip,port);
}

void MyModbusControl::disconnectFromPLC()
{
    modbus->disconnectFromPLC();
}

bool MyModbusControl::readFromPlc(QList<int>address ,QMap<int,double> &result,int slaveId)
{
    return modbus->readRegisters(address,result,slaveId);
}


bool MyModbusControl::writeToPlc(QMap<int, double> map, int slaveId)
{

   return modbus->writeRegisters(map,slaveId);
}

bool MyModbusControl::readFromPlc(int address, QMap<int, double> &result, int slaveId)
{
    return modbus->readDouble32(address,result,1);
}



bool MyModbusControl::readASCIIFromPlc(int startAddress, int endAddress, QString &str, int slaveId)
{
    return modbus->readASCII(startAddress,endAddress,str,slaveId);
}

bool MyModbusControl::writeToPlc(int address, double value, int slaveId)
{
    return modbus->writeDouble32(address,value,slaveId);
}


bool MyModbusControl::writeToPlc(int address, QString str, int slaveId)
{
    return modbus->writeQString(address,str,slaveId);
}







