#include"parameters.h"

// 参数映射表(1000-1104)
QMap<ProductParameter, ParameterInfo> ProductParameter_MAP = {

{ProductParameter::LoaderA_RodInitialPos,
 ParameterInfo(1, 1000, 0.0, "上料车A料杆初始位置")},
    {ProductParameter::LoaderB_RodInitialPos,
     ParameterInfo(2, 1002, 0.0, "上料车B料杆初始位置")},
    {ProductParameter::LoaderC_RodInitialPos,
     ParameterInfo(3, 1004, 0.0, "上料车C料杆初始位置")},
    {ProductParameter::LoaderA_RodUpperLimit,
     ParameterInfo(4, 1006, 0.0, "上料车A料杆上限位置")},
    {ProductParameter::LoaderB_RodUpperLimit,
     ParameterInfo(5, 1008, 0.0, "上料车B料杆上限位置")},
    {ProductParameter::LoaderC_RodUpperLimit,
     ParameterInfo(6, 1010, 0.0, "上料车C料杆上限位置")},

    {ProductParameter::UnloaderA_RodInitialPos,
     ParameterInfo(7, 1012, 0.0, "下料车A料杆初始位置")},
    {ProductParameter::UnloaderB_RodInitialPos,
     ParameterInfo(8, 1014, 0.0, "下料车B料杆初始位置")},
    {ProductParameter::UnloaderC_RodInitialPos,
     ParameterInfo(9, 1016, 0.0, "下料车C料杆初始位置")},
    {ProductParameter::UnloaderA_RodUpperLimit,
     ParameterInfo(10, 1018, 0.0, "下料车A料杆上限位置")},
    {ProductParameter::UnloaderB_RodUpperLimit,
     ParameterInfo(11, 1020, 0.0, "下料车B料杆上限位置")},
    {ProductParameter::UnloaderC_RodUpperLimit,
     ParameterInfo(12, 1022, 0.0, "下料车C料杆上限位置")},

    {ProductParameter::XRay_Z_InitialPos,
     ParameterInfo(13, 1024, 0.0, "X光检测Z轴初始位置")},
    {ProductParameter::XRay_Z_DetectPos,
     ParameterInfo(14, 1026, 0.0, "X光检测Z轴检测位置")},

    // 上料机械手位置
    {ProductParameter::LoaderArm_PickPos_J1,
     ParameterInfo(15, 1028, 0.0, "上料机械手取料位置_J1")},
    {ProductParameter::LoaderArm_PickPos_J2,
     ParameterInfo(16, 1030, 0.0, "上料机械手取料位置_J2")},
    {ProductParameter::LoaderArm_PickPos_J3,
     ParameterInfo(17, 1032, 0.0, "上料机械手取料位置_J3")},
    {ProductParameter::LoaderArm_PickPos_J4,
     ParameterInfo(18, 1034, 0.0, "上料机械手取料位置_J4")},
    {ProductParameter::LoaderArm_IntermediatePos_J1,
     ParameterInfo(19, 1036, 0.0, "上料机械手取料中转位置_J1")},
    {ProductParameter::LoaderArm_IntermediatePos_J2,
     ParameterInfo(20, 1038, 0.0, "上料机械手取料中转位置_J2")},
    {ProductParameter::LoaderArm_IntermediatePos_J3,
     ParameterInfo(21, 1040, 0.0, "上料机械手取料中转位置_J3")},
    {ProductParameter::LoaderArm_IntermediatePos_J4,
     ParameterInfo(22, 1042, 0.0, "上料机械手取料中转位置_J4")},
    {ProductParameter::LoaderArm_PlacePos_J1,
     ParameterInfo(23, 1044, 0.0, "上料机械手放料位置_J1")},
    {ProductParameter::LoaderArm_PlacePos_J2,
     ParameterInfo(24, 1046, 0.0, "上料机械手放料位置_J2")},
    {ProductParameter::LoaderArm_PlacePos_J3,
     ParameterInfo(25, 1048, 0.0, "上料机械手放料位置_J3")},
    {ProductParameter::LoaderArm_PlacePos_J4,
     ParameterInfo(26, 1050, 0.0, "上料机械手放料位置_J4")},

    // 下料机械手位置
    {ProductParameter::UnloaderArm_PickPos_J1,
     ParameterInfo(27, 1052, 0.0, "下料机械手取料位置_J1")},
    {ProductParameter::UnloaderArm_PickPos_J2,
     ParameterInfo(28, 1054, 0.0, "下料机械手取料位置_J2")},
    {ProductParameter::UnloaderArm_PickPos_J3,
     ParameterInfo(29, 1056, 0.0, "下料机械手取料位置_J3")},
    {ProductParameter::UnloaderArm_PickPos_J4,
     ParameterInfo(30, 1058, 0.0, "下料机械手取料位置_J4")},
    {ProductParameter::UnloaderArm_IntermediatePos_J1,
     ParameterInfo(31, 1060, 0.0, "下料机械手取料中转位置_J1")},
    {ProductParameter::UnloaderArm_IntermediatePos_J2,
     ParameterInfo(32, 1062, 0.0, "下料机械手取料中转位置_J2")},
    {ProductParameter::UnloaderArm_IntermediatePos_J3,
     ParameterInfo(33, 1064, 0.0, "下料机械手取料中转位置_J3")},
    {ProductParameter::UnloaderArm_IntermediatePos_J4,
     ParameterInfo(34, 1066, 0.0, "下料机械手取料中转位置_J4")},
    {ProductParameter::UnloaderArm_LabelPos_J1,
     ParameterInfo(35, 1068, 0.0, "下料机械手贴标位置_J1")},
    {ProductParameter::UnloaderArm_LabelPos_J2,
     ParameterInfo(36, 1070, 0.0, "下料机械手贴标位置_J2")},
    {ProductParameter::UnloaderArm_LabelPos_J3,
     ParameterInfo(37, 1072, 0.0, "下料机械手贴标位置_J3")},
    {ProductParameter::UnloaderArm_LabelPos_J4,
     ParameterInfo(38, 1074, 0.0, "下料机械手贴标位置_J4")},
    {ProductParameter::UnloaderArm_PlacePos_J1,
     ParameterInfo(39, 1076, 0.0, "下料机械手放料位置_J1")},
    {ProductParameter::UnloaderArm_PlacePos_J2,
     ParameterInfo(40, 1078, 0.0, "下料机械手放料位置_J2")},
    {ProductParameter::UnloaderArm_PlacePos_J3,
     ParameterInfo(41, 1080, 0.0, "下料机械手放料位置_J3")},
    {ProductParameter::UnloaderArm_PlacePos_J4,
     ParameterInfo(42, 1082, 0.0, "下料机械手放料位置_J4")},

    // 绑定参数
    {ProductParameter::LoaderA_BindedQuantity,
     ParameterInfo(43, 1084, 0.0, "上料车A料杆绑定上料数量")},
    {ProductParameter::LoaderB_BindedQuantity,
     ParameterInfo(44, 1086, 0.0, "上料车B料杆绑定上料数量")},
    {ProductParameter::LoaderC_BindedQuantity,
     ParameterInfo(45, 1088, 0.0, "上料车C料杆绑定上料数量")},
    {ProductParameter::Probe1_BasePos,
     ParameterInfo(46, 1090, 0.0, "点料测试探针1#基准位置")},
    {ProductParameter::Probe2_BasePos,
     ParameterInfo(47, 1092, 0.0, "点料测试探针2#基准位置")},
    {ProductParameter::UnloaderA_TraySize,
     ParameterInfo(48, 1094, 0.0, "下料车A料杆绑定料盘尺寸")},
    {ProductParameter::UnloaderB_TraySize,
     ParameterInfo(49, 1096, 0.0, "下料车B料杆绑定料盘尺寸")},
    {ProductParameter::UnloaderC_TraySize,
     ParameterInfo(50, 1098, 0.0, "下料车C料杆绑定料盘尺寸")},
    {ProductParameter::UnloaderA_TrayQuantity,
     ParameterInfo(51, 1100, 0.0, "下料车A料杆绑定料盘数量")},
    {ProductParameter::UnloaderB_TrayQuantity,
     ParameterInfo(52, 1102, 0.0, "下料车B料杆绑定料盘数量")},
{ProductParameter::UnloaderC_TrayQuantity,
        ParameterInfo(53, 1104, 0.0, "下料车C料杆绑定料盘数量")}

};

// 系统参数映射表（100-310）
 QMap<SystemParameter, ParameterInfo> SystemParameter_MAP = {
    {SystemParameter::Software_Enabled,
     ParameterInfo(54, 100, 0.0, "上位机软件已开启")},

    {SystemParameter::Device_ErrorCode,
     ParameterInfo(55, 200, 0.0, "设备报警代码")},

    {SystemParameter::PLC_Start,
     ParameterInfo(56, 202, 0.0, "PLC启动")},

    {SystemParameter::PLC_Stop,
     ParameterInfo(57, 204, 0.0, "PLC停止")},

    {SystemParameter::PLC_Reset,
     ParameterInfo(58, 206, 0.0, "PLC复位")},

    {SystemParameter::PLC_RequestTrayPhoto,
     ParameterInfo(59, 208, 0.0, "PLC通知PC料盘码拍照")},

    {SystemParameter::PLC_RequestXRay,
     ParameterInfo(60, 210, 0.0, "PLC通知PCX光检测")},

    {SystemParameter::PLC_RequestLabel,
     ParameterInfo(61, 212, 0.0, "PLC通知PC请求出标")},

    {SystemParameter::PLC_RequestLCRPhoto,
     ParameterInfo(62, 214, 0.0, "PLC通知PClcr测试拍照")},

    {SystemParameter::PLC_StartLCRTest,
     ParameterInfo(63, 216, 0.0, "PLC通知PC开始lcr测试")},

    {SystemParameter::Loader_DetectedQuantity,
     ParameterInfo(64, 218, 0.0, "PLC上料检测数量")},

    {SystemParameter::Production_Quantity,
     ParameterInfo(65, 220, 0.0, "生产数量")},

    {SystemParameter::Cycle_Time,
     ParameterInfo(66, 222, 0.0, "节拍")},

    {SystemParameter::Software_Status,
     ParameterInfo(67, 300, 0.0, "上位机状态")},

    {SystemParameter::LoaderA_TraySize,
     ParameterInfo(68, 302, 0.0, "上料车A料杆料盘尺寸")},

    {SystemParameter::LoaderB_TraySize,
     ParameterInfo(69, 304, 0.0, "上料车B料杆料盘尺寸")},

    {SystemParameter::LoaderC_TraySize,
     ParameterInfo(70, 306, 0.0, "上料车C料杆料盘尺寸")},

    {SystemParameter::Probe1_Offset,
     ParameterInfo(71, 308, 0.0, "点料测试探针1#补偿值")},

    {SystemParameter::Probe2_Offset,
     ParameterInfo(72, 310, 0.0, "点料测试探针2#补偿值")}


};

// 二维码参数映射表（400-600）
QMap<TrayCodeParameter, TrayCodeParameterInfo> TrayCodeParameter_MAP = {
    {TrayCodeParameter::LoaderA_TrayCode,
     TrayCodeParameterInfo(73, 400, 0, "上料车A料杆料盘码")},

    {TrayCodeParameter::LoaderB_TrayCode,
     TrayCodeParameterInfo(74, 500, 0, "上料车B料杆料盘码")},

    {TrayCodeParameter::LoaderC_TrayCode,
     TrayCodeParameterInfo(75, 600, 0, "上料车C料杆料盘码")}


};
