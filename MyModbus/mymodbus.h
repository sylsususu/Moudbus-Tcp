#ifndef MYMODBUS_H
#define MYMODBUS_H

#include <QObject>
#include <QModbusTcpClient>
#include <QModbusDataUnit>
#include <QMutex>
#include <QTimer>
#include<QEventLoop>


class MyModbus : public QObject
{
    Q_OBJECT
public:
    enum ConnectionState {
        Disconnected,    // 未连接
        Connecting,      // 连接中
        Connected,       // 已连接
        Disconnecting    // 断开中
    };
    Q_ENUM(ConnectionState)

    enum OperationStatus {
        Success,         // 操作成功
        Failed,          // 操作失败
        Timeout,         // 操作超时
        InvalidState     // 无效状态
    };
    Q_ENUM(OperationStatus)
    const int MAX_REGISTERS = 50; // 保守值
private:
    explicit MyModbus(QObject *parent = nullptr);
    ~MyModbus();

    // 禁止拷贝和赋值
    MyModbus(const MyModbus&) = delete;
    MyModbus& operator=(const MyModbus&) = delete;

    static MyModbus* instance;
    static QMutex mutex;

    QModbusTcpClient *modbusDevice;
    QTimer *heartbeatTimer;
    ConnectionState currentState;
    QString lastError;

    void setState(ConnectionState newState);


public:
    // 获取单例实例
    static MyModbus* getInstance();
    // 释放单例实例
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
 * @return 无返回值。
 * @autor ylshi
 * @note
 *
 */
    void disconnectFromPLC();

    /**
 * @brief 从单个地址读取32位浮点数（同步模式）
 * @param[in] registerAddress 要读取的寄存器地址
 * @param[out] resultMap 读取结果映射表<地址, 值>（单元素）
 * @param[in] slaveId 从机设备ID，默认为1
 * @return true 读取成功，false 读取失败
 */
    bool readDouble32(int registerAddress, QMap<int, double> &resultMap, int slaveId = 1);

    /**
 * @brief 从PLC读取ASCII字符串（同步模式）
 * @param[in] startAdd 起始寄存器地址
 * @param[in] endAdd 结束寄存器地址（包含）
 * @param[out] str 读取到的字符串结果
 * @param[in] slaveId 从机设备ID，默认为1
 * @return true 读取成功，false 读取失败
 * @see qsToRegisters()
 */
    bool readASCII(int startAdd, int endAdd, QString &str, int slaveId);

    /**
 * @brief 从PLC批量读取32位浮点数据（同步模式）
 * @param[in] addressList 要读取的寄存器地址列表（自动排序去重）
 * @param[out] result 读取结果映射表<地址, 值>
 * @param[in] slaveId 从机设备ID，默认为1
 * @return true 读取成功，false 读取失败（可通过lastError()获取错误信息）
 */
    bool readRegisters(QList<int> addressList, QMap<int, double> &resultMap, int slaveId = 1);



    /**
 * @brief 向单个地址写入32位浮点数（同步模式）
 * @param[in] address 目标寄存器地址
 * @param[in] value 要写入的double值
 * @param[in] slaveId 从机设备ID，默认为1
 * @return true 写入成功，false 写入失败
 */
    bool writeDouble32(int registerAddress, double value, int slaveId = 1);


    /**
 * @brief 向PLC写入ASCII字符串（同步模式）
 * @param[in] address 起始寄存器地址
 * @param[in] str 要写入的字符串
 * @param[in] slaveId 从机设备ID，默认为1
 * @return true 写入成功，false 写入失败
 */
    bool writeQString(int registerAddress, const QString &value, int slaveId = 1);


    /**
 * @brief 向PLC批量写入32位浮点数据（同步模式）
 * @param[in] map 要写入的<地址, 值>映射表（自动排序去重）
 * @param[in] slaveId 从机设备ID，默认为1
 * @return true 全部写入成功，false 写入失败（可通过lastError()获取错误信息）
 */
    bool writeRegisters(const QMap<int, double> &map, int slaveId = 1);

    /**
 * @brief 获取当前状态
 *
 * @return ConnectionState,当前状态
 * @autor ylshi
 * @note
 *
 */
    ConnectionState state() const { return currentState; }

    /**
 * @brief 获取最后错误信息
 *
 * @return QString错误信息
 * @autor ylshi
 * @note
 *
 */
    QString errorString() const { return lastError; }

signals:
    // 状态变化信号
    void stateChanged(MyModbus::ConnectionState state);
    // 连接结果信号
    void connectionResult(MyModbus::OperationStatus status);

    // 心跳信号
    void heartbeat(bool connected);

private slots:
    void onModbusStateChanged(QModbusDevice::State state);
    void onHeartbeatTimeout();

private:

    // 将浮点数转换为两个16位寄存器值
    void doubleToRegisters(double value,quint16 &high,quint16& low);

    // 将QString转换为16位寄存器值
    QVector<quint16> qsToRegisters(QString value);

    // 将两个16位寄存器值转换为浮点数
    double registersToDouble(quint16 high, quint16 low);


};

#endif // MYMODBUS_H
