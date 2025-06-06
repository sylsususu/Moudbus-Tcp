// mymodbuscontrol.h
#pragma once
#include "mymodbus.h"
#include <QObject>
#include <QTimer>
#include <QMap>
#include<QDebug>
#include <QString>
#include<QThread>
#include<QSettings>
#include<QList>
#include<math.h>

class MyModbusControl : public QObject
{
    Q_OBJECT
private:
    explicit MyModbusControl(QObject *parent = nullptr);
    ~MyModbusControl();
    static MyModbusControl* instance;
    static QMutex mutex;

    MyModbus* modbus;  // 持有MyModbus单例

public:
    static MyModbusControl* getInstance();
    static void releaseInstance();

    /**
 * @brief 连接到plc
 * @param plc的ip地址
 * @param plc的端口号
 *
 * @return 无返回值。
 * @autor ylshi
 * @note
 *
 */
    void connectToPLC(const QString &ip, int port = 502);

    /**
 * @brief 断开plc连接
 * @param
 * @param
 *
 * @return 无返回值。
 * @autor ylshi
 * @note
 *
 */
    void disconnectFromPLC();


    /**
 * @brief 从PLC批量读取32位浮点数据（同步模式）
 * @param[in] addressList 要读取的寄存器地址列表（自动排序去重）
 * @param[out] result 读取结果映射表<地址, 值>
 * @param[in] slaveId 从机设备ID，默认为1
 * @return true 读取成功，false 读取失败（可通过lastError()获取错误信息）
 */
    bool readFromPlc(QList<int>address ,QMap<int,double> &result,int slaveId = 1);

    /**
 * @brief 向PLC批量写入32位浮点数据（同步模式）
 * @param[in] map 要写入的<地址, 值>映射表（自动排序去重）
 * @param[in] slaveId 从机设备ID，默认为1
 * @return true 全部写入成功，false 写入失败（可通过lastError()获取错误信息）
 */
    bool writeToPlc(QMap<int,double>map,int slaveId = 1);

    /**
 * @brief 从单个地址读取32位浮点数（同步模式）
 * @param[in] address 要读取的寄存器地址
 * @param[out] result 读取结果映射表<地址, 值>（单元素）
 * @param[in] slaveId 从机设备ID，默认为1
 * @return true 读取成功，false 读取失败
 */
    bool readFromPlc(int address,QMap<int,double> &result,int slaveId = 1);

    /**
 * @brief 从PLC读取ASCII字符串（同步模式）
 * @param[in] startAddress 起始寄存器地址
 * @param[in] endAddress 结束寄存器地址（包含）
 * @param[out] str 读取到的字符串结果
 * @param[in] slaveId 从机设备ID，默认为1
 * @return true 读取成功，false 读取失败
 * @see qsToRegisters()
 */
    bool readASCIIFromPlc(int startAddress, int endAddress, QString &str,int slaveId=1);

    /**
 * @brief 向单个地址写入32位浮点数（同步模式）
 * @param[in] address 目标寄存器地址
 * @param[in] value 要写入的double值
 * @param[in] slaveId 从机设备ID，默认为1
 * @return true 写入成功，false 写入失败
 */
    bool writeToPlc(int address,double value,int slaveId = 1);

    /**
 * @brief 向PLC写入ASCII字符串（同步模式）
 * @param[in] address 起始寄存器地址
 * @param[in] str 要写入的字符串
 * @param[in] slaveId 从机设备ID，默认为1
 * @return true 写入成功，false 写入失败
 */
    bool writeToPlc(int address,QString str,int slaveId = 1);



signals:
    //连接状态信号 bool:true-已连接 false-未连接
    void signalConnectionState(bool connected);

    //心跳检测信号
    void signalHeartbeat(bool connected);

};
