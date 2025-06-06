#include "mymodbus.h"
#include <QDebug>
#include <QModbusReply>
#include <QtEndian>

// 初始化静态成员变量
MyModbus* MyModbus::instance = nullptr;
QMutex MyModbus::mutex;

MyModbus::MyModbus(QObject *parent)
    : QObject(parent),
    modbusDevice(nullptr),
    heartbeatTimer(nullptr),
    currentState(Disconnected)
{
    modbusDevice = new QModbusTcpClient(this);
    heartbeatTimer = new QTimer(this);

    // 设置心跳检测间隔500ms
    heartbeatTimer->setInterval(500);
    connect(heartbeatTimer, &QTimer::timeout, this, &MyModbus::onHeartbeatTimeout);

    // 连接状态变化信号
    connect(modbusDevice, &QModbusClient::stateChanged, this, &MyModbus::onModbusStateChanged);
}

MyModbus::~MyModbus()
{
    disconnectFromPLC();
    heartbeatTimer->stop();
    delete modbusDevice;
}

MyModbus* MyModbus::getInstance()
{
    QMutexLocker locker(&mutex);
    if (instance == nullptr) {
        instance = new MyModbus();
    }
    return instance;
}

void MyModbus::releaseInstance()
{
    QMutexLocker locker(&mutex);
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

void MyModbus::setState(ConnectionState newState)
{
    if (currentState != newState) {
        currentState = newState;
        emit stateChanged(currentState);
    }
}

void MyModbus::connectToPLC(const QString &ip, int port)
{
    if (currentState != Disconnected) {
        lastError = tr("当前状态不允许连接");
        emit connectionResult(InvalidState);
        return;
    }

    setState(Connecting);
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, ip);
    modbusDevice->setTimeout(1000);
    modbusDevice->setNumberOfRetries(1);//失败重连次数

    if (!modbusDevice->connectDevice()) {
        lastError = tr("连接失败: %1").arg(modbusDevice->errorString());
        //qDebug()<<lastError;
        setState(Disconnected);
        emit connectionResult(Failed);
    }
}

void MyModbus::disconnectFromPLC()
{
    if (currentState == Disconnected || currentState == Disconnecting) {
        return;
    }

    setState(Disconnecting);
    heartbeatTimer->stop();
    modbusDevice->disconnectDevice();
}

void MyModbus::onModbusStateChanged(QModbusDevice::State state)
{
    switch (state) {
    case QModbusDevice::UnconnectedState:
        setState(Disconnected);
        heartbeatTimer->stop();
        emit connectionResult(currentState == Disconnecting ? Success : Failed);
        break;
    case QModbusDevice::ConnectingState:
        setState(Connecting);
        break;
    case QModbusDevice::ConnectedState:
        setState(Connected);
        heartbeatTimer->start();
        emit connectionResult(Success);
        break;
    case QModbusDevice::ClosingState:
        setState(Disconnecting);
        break;
    }
}

void MyModbus::onHeartbeatTimeout()
{
    if (currentState != Connected) {
        return;
    }

    // 简单的心跳检测 - 写入一个保持寄存器
    int testAddress = 100;
    // 静态变量保存当前心跳值（1或2）
    static quint16 heartbeatValue = 1;

    // 心跳值跳变（1变2，2变1）
    heartbeatValue = (heartbeatValue == 1) ? 2 : 1;

    // 写入心跳值到保持寄存器
    QVector<quint16> registers;
    quint16 regHigh, regLow;
    doubleToRegisters(heartbeatValue, regHigh, regLow);
    registers.append(regLow);
    registers.append(regHigh);
    QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, testAddress, registers);
    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, 1)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::NoError) {
                    emit heartbeat(true);
                } else {
                    lastError = tr("心跳检测失败: %1").arg(reply->errorString());
                    emit heartbeat(false);
                }
                reply->deleteLater();
            });
        }
    } else {
        emit heartbeat(false);
        lastError = tr("心跳检测请求失败: %1").arg(modbusDevice->errorString());
        setState(Disconnected);
        emit connectionResult(Failed);
    }



}



bool MyModbus::readDouble32(int registerAddress, QMap<int, double> &resultMap, int slaveId)
{
    resultMap.clear();    // 清空输出参数

    if (currentState != Connected) {
        lastError = tr("设备未连接");
        return false;
    }

    QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, registerAddress, 2);

    if (auto *reply = modbusDevice->sendReadRequest(readUnit, slaveId)) {
        QEventLoop eventLoop;
        QTimer timeoutTimer;
        QObject::connect(&timeoutTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);
        QObject::connect(reply, &QModbusReply::finished, &eventLoop, &QEventLoop::quit);

        timeoutTimer.setSingleShot(true);
        timeoutTimer.start(3000);  // 3秒超时
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

        bool isTimeout = !timeoutTimer.isActive();
        reply->deleteLater();

        // 处理超时
        if (isTimeout) {
            lastError = tr("响应超时");
            modbusDevice->disconnectDevice();
            return false;
        }

        // 处理正常响应
        if (reply->error() == QModbusDevice::NoError) {
            const QModbusDataUnit unit = reply->result();
            if (unit.valueCount() >= 2) {
                // 解析双精度值
                double value = registersToDouble(unit.value(1), unit.value(0));

                // 同时更新成员变量和输出参数
                resultMap.insert(registerAddress, value);
                return true;
            }
            lastError = tr("响应数据长度不足");
        } else {
            lastError = tr("Modbus错误: %1").arg(reply->errorString());
        }
    } else {
        lastError = tr("请求发送失败: %1").arg(modbusDevice->errorString());
    }

    return false;
}



bool MyModbus::readASCII(int startAdd, int endAdd, QString &str, int slaveId)
{
    str.clear();  // 清空输出参数

    if (currentState != Connected) {
        lastError = tr("设备未连接");
        return false;
    }

    const int registerCount = endAdd - startAdd + 1;  // 修正寄存器数量计算
    QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, startAdd, registerCount);

    if (auto *reply = modbusDevice->sendReadRequest(readUnit, slaveId)) {
        QEventLoop eventLoop;
        QTimer timeoutTimer;
        timeoutTimer.setSingleShot(true);

        // 连接信号到事件循环
        QObject::connect(&timeoutTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);
        QObject::connect(reply, &QModbusReply::finished, &eventLoop, &QEventLoop::quit);

        // 设置3秒超时（可根据需要调整）
        timeoutTimer.start(3000);
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

        const bool isTimeout = !timeoutTimer.isActive();
        reply->deleteLater();

        // 处理超时
        if (isTimeout) {
            lastError = tr("操作超时");
            modbusDevice->disconnectDevice();
            return false;
        }

        // 处理Modbus错误
        if (reply->error() != QModbusDevice::NoError) {
            lastError = tr("通信错误: %1").arg(reply->errorString());
            return false;
        }

        // 处理数据解析
        const QModbusDataUnit unit = reply->result();
        if (unit.valueCount() != registerCount) {
            lastError = tr("数据长度不匹配 预期:%1 实际:%2")
                            .arg(registerCount).arg(unit.valueCount());
            return false;
        }

        // 转换寄存器数据为ASCII
        QString buffer;
        for (int i = 0; i < unit.valueCount(); ++i) {
            const quint16 regValue = unit.value(i);

            // 处理高字节（MSB）
            const char highChar = static_cast<char>((regValue >> 8) & 0xFF);
            if (highChar != '\0' && QChar::isPrint(highChar)) {
                buffer.append(highChar);
            }

            // 处理低字节（LSB）
            const char lowChar = static_cast<char>(regValue & 0xFF);
            if (lowChar != '\0' && QChar::isPrint(lowChar)) {
                buffer.append(lowChar);
            }
        }

        // 返回结果
        str = buffer;
        return true;
    }

    lastError = tr("请求发送失败: %1").arg(modbusDevice->errorString());
    return false;
}



bool MyModbus::readRegisters(QList<int> addressList, QMap<int, double> &resultMap, int slaveId)
{
    resultMap.clear();

    if (currentState != Connected) {
        lastError = tr("设备未连接");
        return false;
    }

    // 地址预处理（排序去重）
    std::sort(addressList.begin(), addressList.end());
    auto last = std::unique(addressList.begin(), addressList.end());
    addressList.erase(last, addressList.end());

    const int registersPerAddress = 2;
    const int maxBatchRegisters = MAX_REGISTERS;

    QEventLoop eventLoop;
    QTimer timeoutTimer;
    QModbusReply* currentReply = nullptr;

    // 配置公共信号连接
    timeoutTimer.setSingleShot(true);
    QObject::connect(&timeoutTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);

    // 处理所有地址分批次读取
    for (int i = 0; i < addressList.size(); ) {
        int startAddr = addressList[i];
        int currentRegCount = 0;
        QList<int> currentBatch;

        // 构建当前批次（保证寄存器数量不超过限制）
        while (i < addressList.size()) {
            int addr = addressList[i];
            int requiredRegs = (addr - startAddr)  + registersPerAddress;

            if (currentRegCount + requiredRegs > maxBatchRegisters)
                break;

            currentBatch.append(addr);
            currentRegCount = requiredRegs;
            i++;
        }

        // 计算实际需要读取的寄存器数量
        int lastAddr = currentBatch.last();
        int totalRegs = (lastAddr - startAddr)  + registersPerAddress;
        // qDebug()<<totalRegs;
        QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, startAddr, totalRegs);

        // 发送请求
        if ((currentReply = modbusDevice->sendReadRequest(readUnit, slaveId))) {
            // 连接完成信号
            QObject::connect(currentReply, &QModbusReply::finished, &eventLoop, &QEventLoop::quit);

            // 启动等待循环
            timeoutTimer.start(3000);
            eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

            // 清除信号连接
            QObject::disconnect(currentReply, nullptr, &eventLoop, nullptr);

            // 处理超时
            if (!timeoutTimer.isActive()) {
                lastError = tr("操作超时");
                currentReply->deleteLater();
                modbusDevice->disconnectDevice();
                return false;
            }

            // 处理响应
            if (currentReply->error() != QModbusDevice::NoError) {
                lastError = tr("Modbus错误: %1").arg(currentReply->errorString());
                currentReply->deleteLater();
                return false;
            }

            // 验证数据
            const QModbusDataUnit unit = currentReply->result();
            if (unit.valueCount() < totalRegs) {
                lastError = tr("数据长度不足 需要:%1 实际:%2").arg(totalRegs).arg(unit.valueCount());
                currentReply->deleteLater();
                return false;
            }

            // 解析数据
            for (int addr : currentBatch) {
                int offset = (addr - startAddr);
                // if (offset + 1 >= unit.valueCount()) {
                //     lastError = tr("数据偏移越界 地址:%1").arg(addr);
                //     currentReply->deleteLater();
                //     return false;
                // }

                double value = registersToDouble(
                    unit.value(offset + 1),  // 高位
                    unit.value(offset)       // 低位
                    );
                resultMap.insert(addr, value);
            }

            currentReply->deleteLater();
        } else {
            lastError = tr("请求发送失败: %1").arg(modbusDevice->errorString());
            return false;
        }
    }

    return true;
}



bool MyModbus::writeDouble32(int registerAddress, double value, int slaveId)
{
    if (currentState != Connected) {
        lastError = tr("设备未连接");
        return false;
    }

    // 将double转换为寄存器值
    QVector<quint16> registers;
    quint16 regHigh, regLow;
    doubleToRegisters(value, regHigh, regLow);
    registers.append(regLow);
    registers.append(regHigh);

    // 构造Modbus数据单元
    QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, registerAddress, registers);

    // 发送写入请求
    if (QModbusReply *reply = modbusDevice->sendWriteRequest(writeUnit, slaveId)) {
        QEventLoop eventLoop;
        QTimer timeoutTimer;

        // 配置超时定时器（3秒）
        timeoutTimer.setSingleShot(true);
        QObject::connect(&timeoutTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);
        QObject::connect(reply, &QModbusReply::finished, &eventLoop, &QEventLoop::quit);

        // 启动等待循环
        timeoutTimer.start(3000);
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

        // 检测超时
        const bool isTimeout = !timeoutTimer.isActive();
        const bool writeSuccess = !isTimeout && (reply->error() == QModbusDevice::NoError);

        // 清理资源
        reply->deleteLater();

        if (isTimeout) {
            lastError = tr("写入操作超时");
            modbusDevice->disconnectDevice();
            return false;
        }

        if (!writeSuccess) {
            lastError = tr("写入失败: %1").arg(reply->errorString());
            return false;
        }

        // 验证实际写入值（可选）
        QMap<int, double> verifyMap;
        if (!readDouble32(registerAddress, verifyMap, slaveId)) {
            lastError = tr("写入验证失败: %1").arg(lastError);
            return false;
        }

        if (qAbs(verifyMap.value(registerAddress) - value) > 0.0001) {
            lastError = tr("写入值验证不匹配");
            return false;
        }

        return true;
    }

    lastError = tr("请求发送失败: %1").arg(modbusDevice->errorString());
    return false;
}




bool MyModbus::writeQString(int registerAddress, const QString &value, int slaveId)
{
    if (currentState != Connected) {
        lastError = tr("设备未连接");
        return false;
    }

    const QVector<quint16> allRegisters = qsToRegisters(value);
    QEventLoop eventLoop;
    QTimer timeoutTimer;
    timeoutTimer.setSingleShot(true);

    // 配置公共信号连接
    QObject::connect(&timeoutTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);

    // 分批次写入
    for (int i = 0; i < allRegisters.size(); i += MAX_REGISTERS) {
        const int currentChunkSize = qMin(MAX_REGISTERS, allRegisters.size() - i);
        const QVector<quint16> chunk = allRegisters.mid(i, currentChunkSize);
        const int currentAddress = registerAddress + i;

        // 构造数据单元
        QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, currentAddress, chunk);

        // 发送请求
        if (QModbusReply *reply = modbusDevice->sendWriteRequest(writeUnit, slaveId)) {
            // 连接完成信号
            QObject::connect(reply, &QModbusReply::finished, &eventLoop, &QEventLoop::quit);

            // 启动等待循环（3秒超时）
            timeoutTimer.start(3000);
            eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

            // 断开信号连接
            QObject::disconnect(reply, nullptr, &eventLoop, nullptr);

            // 检测超时
            const bool isTimeout = !timeoutTimer.isActive();
            const bool writeSuccess = !isTimeout && (reply->error() == QModbusDevice::NoError);

            // 获取错误信息
            if (!writeSuccess) {
                lastError = isTimeout ? tr("操作超时")
                                      : tr("写入错误: %1").arg(reply->errorString());
                reply->deleteLater();
                if (isTimeout) modbusDevice->disconnectDevice();
                return false;
            }

            reply->deleteLater();



        } else {
            lastError = tr("请求发送失败: %1").arg(modbusDevice->errorString());
            return false;
        }
    }

    return true;
}



bool MyModbus::writeRegisters(const QMap<int, double> &map, int slaveId)
{
    if (currentState != Connected) {
        lastError = tr("设备未连接");
        return false;
    }

    QList<int> addressList = map.keys();
    std::sort(addressList.begin(), addressList.end());
    auto last = std::unique(addressList.begin(), addressList.end());
    addressList.erase(last, addressList.end());

    const int registersPerAddress = 2;
    const int maxRegistersPerWrite = MAX_REGISTERS;

    QEventLoop eventLoop;
    QTimer timeoutTimer;
    timeoutTimer.setSingleShot(true);

    // 分批次写入
    for (int i = 0; i < addressList.size(); ) {
        int startAddr = addressList[i];
        int remainingRegisters = maxRegistersPerWrite;
        QVector<quint16> registers;

        // 构建当前批次数据
        while (i < addressList.size() && remainingRegisters >= registersPerAddress) {
            int currentAddr = addressList[i];

            // // 验证地址连续性（可选）
            // if (currentAddr != startAddr + (i * registersPerAddress)) {
            //     lastError = tr("地址不连续 %1").arg(currentAddr);
            //     return false;
            // }

            // 转换数据
            quint16 regHigh, regLow;
            doubleToRegisters(map[currentAddr], regHigh, regLow);
            registers.append(regLow);
            registers.append(regHigh);

            remainingRegisters -= registersPerAddress;
            i++;
        }

        // 发送写入请求
        QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, startAddr, registers);
        if (QModbusReply *reply = modbusDevice->sendWriteRequest(writeUnit, slaveId)) {
            // 配置信号连接
            QObject::connect(reply, &QModbusReply::finished, &eventLoop, &QEventLoop::quit);
            QObject::connect(&timeoutTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);

            // 同步等待
            timeoutTimer.start(3000);
            eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

            // 清理连接
            QObject::disconnect(reply, nullptr, &eventLoop, nullptr);

            // 处理超时
            const bool isTimeout = !timeoutTimer.isActive();
            const bool writeSuccess = !isTimeout && (reply->error() == QModbusDevice::NoError);

            reply->deleteLater();

            if (isTimeout) {
                lastError = tr("操作超时");
                modbusDevice->disconnectDevice();
                return false;
            }

            if (!writeSuccess) {
                lastError = tr("写入错误: %1").arg(reply->errorString());
                return false;
            }



        } else {
            lastError = tr("请求发送失败: %1").arg(modbusDevice->errorString());
            return false;
        }
    }

    return true;
}






void MyModbus::doubleToRegisters(double value, quint16 &high, quint16 &low)
{
    // 1. 保留3位小数（四舍五入）
    double roundedValue = std::round(value * 1000.0) / 1000.0;

    // 2. 将 double 转为 32 位 float（如果原输入是 double）
    float floatValue = static_cast<float>(roundedValue);

    // 3. 获取 float 的二进制表示（IEEE 754）
    quint32 binary32;
    std::memcpy(&binary32, &floatValue, sizeof(float));

    // 4. 拆分为高16位和低16位（大端序）
    high = static_cast<quint16>((binary32 >> 16) & 0xFFFF); // 高16位
    low  = static_cast<quint16>(binary32 & 0xFFFF);         // 低16位
}

QVector<quint16> MyModbus::qsToRegisters(QString value)
{
    QVector<quint16> registers;

    // 确保字符串长度为偶数
    if (value.length() % 2 != 0) {
        value.append(' '); // 如果长度为奇数，添加一个空格
    }

    for (int i = 0; i < value.length(); i += 2) {
        quint16 reg;
        if (i + 1 < value.length()) {
            // 获取两个字符对应的ASCII码
            reg = (static_cast<quint16>(value[i].toLatin1()) << 8) | static_cast<quint16>(value[i + 1].toLatin1());
        } else {
            // 处理最后一个字符时，使用0作为低字节
            reg = (static_cast<quint16>(value[i].toLatin1()) << 8);
        }

        registers.append(reg);
        //qDebug() << Qt::dec << reg; // 以十进制输出
    }

    return registers;

}

double MyModbus::registersToDouble(quint16 high, quint16 low)
{

    // 1. 将高低16位合并为32位二进制
    quint32 binary32 = (static_cast<quint32>(high) << 16) | low;

    // 2. 将32位二进制解释为float（IEEE 754）
    float floatValue;
    std::memcpy(&floatValue, &binary32, sizeof(float));

    // 3. 转为double并保留3位小数（四舍五入）
    double result = static_cast<double>(floatValue);
    result = std::round(result * 1000.0) / 1000.0;


    return result;
}
