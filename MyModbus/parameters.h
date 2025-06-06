// parameters.h
#pragma once
#include <QString>
#include <QMap>

class ParameterInfo {
public:
    ParameterInfo() = default;

    ParameterInfo(int id, int address, double defaultValue, const QString& chineseName)
        : m_id(id),
        m_address(address),
        m_value(defaultValue),
        m_chineseName(chineseName) {}

    // Getter方法
    int id() const { return m_id; }
    int address() const { return m_address; }
    double value() const { return m_value; }
    QString chineseName() const { return m_chineseName; }

    // Setter方法
    void setValue(double value) { m_value = value; }

private:
    int m_id;            // 参数唯一ID
    int m_address;       // PLC寄存器地址（如D1018）
    double m_value;      // 当前数值
    QString m_chineseName; // 中文描述
};

class TrayCodeParameterInfo {
public:
    TrayCodeParameterInfo() = default;

    TrayCodeParameterInfo(int id, int address, int defaultValue, const QString& chineseName)
        : m_id(id),
        m_address(address),
        m_value(defaultValue),
        m_chineseName(chineseName) {}

    // Getter方法
    int id() const { return m_id; }
    int address() const { return m_address; }
    int value() const { return m_value; }
    QString chineseName() const { return m_chineseName; }

    // Setter方法
    void setValue(double value) { m_value = value; }

private:
    int m_id;            // 参数唯一ID
    int m_address;       // PLC寄存器地址（如D1018）
    int m_value;      // 当前数值
    QString m_chineseName; // 中文描述
};

//配方参数
enum class ProductParameter {
    // 上料车料杆位置
    LoaderA_RodInitialPos,
    LoaderB_RodInitialPos,
    LoaderC_RodInitialPos,
    LoaderA_RodUpperLimit,
    LoaderB_RodUpperLimit,
    LoaderC_RodUpperLimit,

    // 下料车料杆位置
    UnloaderA_RodInitialPos,
    UnloaderB_RodInitialPos,
    UnloaderC_RodInitialPos,
    UnloaderA_RodUpperLimit,
    UnloaderB_RodUpperLimit,
    UnloaderC_RodUpperLimit,

    // X光检测
    XRay_Z_InitialPos,
    XRay_Z_DetectPos,

    // 上料机械手位置
    LoaderArm_PickPos_J1,
    LoaderArm_PickPos_J2,
    LoaderArm_PickPos_J3,
    LoaderArm_PickPos_J4,
    LoaderArm_IntermediatePos_J1,
    LoaderArm_IntermediatePos_J2,
    LoaderArm_IntermediatePos_J3,
    LoaderArm_IntermediatePos_J4,
    LoaderArm_PlacePos_J1,
    LoaderArm_PlacePos_J2,
    LoaderArm_PlacePos_J3,
    LoaderArm_PlacePos_J4,

    // 下料机械手位置
    UnloaderArm_PickPos_J1,
    UnloaderArm_PickPos_J2,
    UnloaderArm_PickPos_J3,
    UnloaderArm_PickPos_J4,
    UnloaderArm_IntermediatePos_J1,
    UnloaderArm_IntermediatePos_J2,
    UnloaderArm_IntermediatePos_J3,
    UnloaderArm_IntermediatePos_J4,
    UnloaderArm_LabelPos_J1,
    UnloaderArm_LabelPos_J2,
    UnloaderArm_LabelPos_J3,
    UnloaderArm_LabelPos_J4,
    UnloaderArm_PlacePos_J1,
    UnloaderArm_PlacePos_J2,
    UnloaderArm_PlacePos_J3,
    UnloaderArm_PlacePos_J4,

    // 绑定参数
    LoaderA_BindedQuantity,
    LoaderB_BindedQuantity,
    LoaderC_BindedQuantity,
    Probe1_BasePos,
    Probe2_BasePos,
    UnloaderA_TraySize,
    UnloaderB_TraySize,
    UnloaderC_TraySize,
    UnloaderA_TrayQuantity,
    UnloaderB_TrayQuantity,
    UnloaderC_TrayQuantity


};

enum class SystemParameter{
    // 系统状态
    Software_Enabled,
    Device_ErrorCode,
    PLC_Start,
    PLC_Stop,
    PLC_Reset,
    PLC_RequestTrayPhoto,
    PLC_RequestXRay,
    PLC_RequestLabel,
    PLC_RequestLCRPhoto,
    PLC_StartLCRTest,
    Loader_DetectedQuantity,
    Production_Quantity,
    Cycle_Time,
    Software_Status,
    LoaderA_TraySize,
    LoaderB_TraySize,
    LoaderC_TraySize,
    Probe1_Offset,
    Probe2_Offset

};

enum class TrayCodeParameter{
    LoaderA_TrayCode,
    LoaderB_TrayCode,
    LoaderC_TrayCode
};

// 声明为外部变量
extern  QMap<ProductParameter, ParameterInfo> ProductParameter_MAP;

extern  QMap<SystemParameter, ParameterInfo> SystemParameter_MAP;

extern QMap<TrayCodeParameter, TrayCodeParameterInfo> TrayCodeParameter_MAP;




