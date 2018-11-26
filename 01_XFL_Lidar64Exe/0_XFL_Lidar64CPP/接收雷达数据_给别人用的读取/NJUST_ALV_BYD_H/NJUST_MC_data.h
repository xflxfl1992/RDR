////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_MC_data.h
//  Author: 夏青元
//  Date:   2015.6.6
//  Description: 车体底层控制模块的数据
//  Modification: 
//          2015.7.3, 任明武
//  Declare:
//          NJUST_MC_STATE_INFO
//          NJUST_MC_NAV_INFO
//          NJUST_MC_DRIVE_INFO
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_MC_DATA_H_
#define _NJUST_MC_DATA_H_

#include "NJUST_Global_Def.h"
#include "NJUST_IP_comm.h" 

#include "NJUST_PL_data.h" 
#include "NJUST_MC_typedef.h" 

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  设置对齐方式
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push) //保存对齐状态
#pragma pack(1)    //按1字节对齐,必须放在第一句

//系统定义
#define NJUST_MAX_MC_PATH_POINTS_NUM      20       //路径执行的最多点数

typedef struct tagNJUST_MC_POINT
{
    INT16  x_cm;     //点的X坐标,车底坐标系,单位:厘米
    INT16  y_cm;     //点的Y坐标,车底坐标系,单位:厘米
}NJUST_MC_POINT;	

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  车体状态
//
////////////////////////////////////////////////////////////////////////////////////////////////
//GPS状态
#define NJUST_MC_GPS_NO_GPS      		0x00          //无GPS
#define NJUST_MC_GPS_FIX_OK      		0x01          //单点定位
#define NJUST_MC_GPS_NARROW_FLOAT 	0X02				// Floating narrow-lane ambiguity solution
#define NJUST_MC_GPS_WIDE_INT				0X03				// Integer wide-lane ambiguity solution
#define NJUST_MC_GPS_NARROW_INT			0X04			// Integer narrow-lane ambiguity solution
#define NJUST_MC_GPS_INS_RTKFLOAT		0X05 
#define NJUST_MC_GPS_INS_RTKFIXED		0X06
#define NJUST_MC_GPS_DIF_USED    		0x07          //使用了差分GPS
#define NJUST_MC_GPS_RTK         		0x08          //RTK

//挂挡器档位
#define NJUST_MC_GEAR_P          0x00          //P挡
#define NJUST_MC_GEAR_R          0x01          //R挡
#define NJUST_MC_GEAR_N          0x02          //N挡
#define NJUST_MC_GEAR_D          0x03          //D挡

//应急停车
#define NJUST_MC_EMERGENCY_STOP_REMOUTE         (1<<0) //远程无线应急停止
#define NJUST_MC_EMERGENCY_STOP_CONTRILLER      (1<<1) //控制器应急停止
#define NJUST_MC_EMERGENCY_STOP_INNER           (1<<2) //车内急停止
#define NJUST_MC_EMERGENCY_STOP_OUTTER          (1<<3) //车外应急停止

//方向灯状态
#define NJUST_MC_TURN_LIGHT_LEFT_ON             (1<<0) //左转向灯在亮
#define NJUST_MC_TURN_LIGHT_RIGHT_ON            (1<<1) //右转向灯在亮





//传感器供电状态
#define NJUST_MC_SENSOR_POWER_ON_COMPUTER_1      	(1<<0) //工控机1
#define NJUST_MC_SENSOR_POWER_ON_COMPUTER_2      	(1<<1) //工控机2
#define NJUST_MC_SENSOR_POWER_ON_COMPUTER_3      	(1<<2) //工控机3
#define NJUST_MC_SENSOR_POWER_ON_LASER_RADAR_64   (1<<3) //64线激光雷达
#define NJUST_MC_SENSOR_POWER_ON_NAV							(1<<4) //惯导系统
#define NJUST_MC_SENSOR_POWER_ON_MC								(1<<5) //底层驱动
#define NJUST_MC_SENSOR_POWER_ON_MONITORMACHINE  	(1<<6)    // 监视计算机	
#define NJUST_MC_SENSOR_POWER_ON_LASER_RADAR_1    (1<<7) //单（四）线激光雷达
#define NJUST_MC_SENSOR_POWER_ON_CAMEAR_2D      	(1<<8) //2D的可见光传感器
#define NJUST_MC_SENSOR_POWER_ON_76G_RADAR   			(1<<9) //车头的76G的毫米波雷达
#define NJUST_MC_SENSOR_POWER_ON_CAMEAR_3D      	(1<<10) //3D的可见光传感器(云台）
#define NJUST_MC_SENSOR_POWER_ON_COOLFAN      		(1<<11) //散热风扇
#define NJUST_MC_SENSOR_POWER_ON_LEDSCR_TAIL      (1<<12) //后视LED屏
#define NJUST_MC_SENSOR_POWER_ON_CAUTION_LIGHT    (1<<13) // 警示LED灯

// 面板灯的状态
#define NJUST_MC_SWLED_STEER 					(1<<0) // 方向盘
#define NJUST_MC_SWLED_BEEP						(1<<1) // 蜂鸣器
#define NJUST_MC_SWLED_COMPUTER 			(1<<2)//工控机
#define NJUST_MC_SWLED_LASER_RADAR_64 (1<<3) // 64线雷达
#define NJUST_MC_SWLED_STOPKEY_FRONT 	(1<<4)// 前急停
#define NJUST_MC_SWLED_TEMP1 					(1<<5)
#define NJUST_MC_SWLED_SILENCE  			(1<<6)// 消音
#define NJUST_MC_SWLED_MONITORMACHINE (1<<7)// 监视器
#define NJUST_MC_SWLED_LASER_RADAR_1 	(1<<8)// 单线雷达
#define NJUST_MC_SWLED_START	  			(1<<9) // 启动
#define NJUST_MC_SWLED_BRAKE					(1<<10)
#define NJUST_MC_SWLED_STOP 					(1<<11)// 急停
#define NJUST_MC_SWLED_LEDSCR_TAIL 		(1<<12)// 后方显示
#define NJUST_MC_SWLED_NAV 						(1<<13)
#define NJUST_MC_SWLED_STOPKEY_BACK 	(1<<14)  // 后急停
#define NJUST_MC_SWLED_AUTOKEY 				(1<<15)// 自主
#define NJUST_MC_SWLED_Fan   					(1<<16) // 风扇
#define NJUST_MC_SWLED_RADIO  				(1<<17)// 无线电台
#define NJUST_MC_SWLED_76G_RADAR 			(1<<18)// 毫米波雷达
#define NJUST_MC_SWLED_TEMP2 					(1<<19)
#define NJUST_MC_SWLED_SIMULAT 				(1<<20)// 仿真
#define NJUST_MC_SWLED_AUTOPOWER 			(1<<21)// 智能上电
#define NJUST_MC_SWLED_CAMERA3D 			(1<<22)// 全球相机
#define NJUST_MC_SWLED_CAMERA2D 			(1<<23)// 二维相机
#define NJUST_MC_SWLED_LED_ERROR  		(1<<24)
#define NJUST_MC_SWLED_NAV_STATUE_LED (1<<25)
#define NJUST_MC_SWLED_CHARGFULL_LED 	(1<<26)
#define NJUST_MC_SWLED_LOWVOT_LED 		(1<<27)
#define NJUST_MC_SWLED_SPEEK_LED			(1<<28)


 // 惯导组合状态 
#define NJUST_MC_NAV_STATUS_INS_INACTIVE 								0x00
#define NJUST_MC_NAV_STATUS_INS_ALIGNING 								0x01
#define NJUST_MC_NAV_STATUS_INS_HIGH_VARIANCE 					0x02
#define NJUST_MC_NAV_STATUS_INS_SOLUTION_GOOD 					0x03     // 正常工作状态
#define NJUST_MC_NAV_STATUS_INS_SOLUTION_FREE 					0x06
#define NJUST_MC_NAV_STATUS_INS_ALIGNMENT_COMPLETE 			0x07
#define NJUST_MC_NAV_STATUS_INS_DETERMINING_ORIENTATION 0x08
#define NJUST_MC_NAV_STATUS_INS_WAITING_INITIALPOS 			0x09


typedef enum ControlState_type
{
		NJUST_MC_CONTROLSTATE_WAIT = 0x00,
	
}ControlState;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  状态信息
//
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_MC_STATE_INFO
{
	UINT16 Head;// 数据帧头  固定值 0x0924
	UINT32 FrameID;
	NJUST_IP_TIME synTime; //时刻
	UINT8 GPS_Status; //GPS状态
	UINT8 IS_NAV_Err; //导航数据是否有错: 0-无错,非零-有错
	UINT32 navID; //导航信息编号(从0开始)
	//状态相关
	UINT16 Voltage_CarBattle_deci; //车辆电瓶电压,单位0.1V
	UINT16 Voltage_SysBattle_deci; //系统电瓶电压,单位0.1V
	UINT16 Current_CarBattle_centi; //车辆电瓶电流,单位0.01A
	UINT16 Current_SysBattle_centi; //系统电瓶电流,单位0.01A
	UINT8 Gear_Position; //挂挡器档位    详见上“挂挡器档位”值
	UINT16 Break_Position; //刹车位置
	INT16 Steer_Position;  //方向盘位置
	UINT8 Throttle_Position; //油门
	UINT16 Speed_Engine; //发动机速度
	UINT8 Turn_Light_Status; //方向灯状态  详见上“方向灯状态”
    //外设管理
	UINT32 bSensor_Power;  //传感器供电状态,对应位1-ON,0-OFF 详见上 "传感器供电状态"
	//面板灯的状态
	UINT32 bPanelLamp; //bit表示,对应位1-ON,0-OFF
	//自主按键的状态
	UINT8 isAutoButtonPressed;  //自主按键的状态
    //运行模式
	UINT8 runMode;//自主标志位, 0-人工,1-自主
	//控制相关
	UINT8 bSTOP_Emergency; //紧急停车
	UINT8 ControllerState; //控制器执行时的状态
	//安全
	UINT16 nSize; //结构体大小 
	UINT8 checksum;  //Checksum of above data.
} NJUST_MC_STATE_INFO;
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  导航信息
//
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_MC_NAV_INFO
{
	UINT16 Head;
	UINT32 FrameID; // 数据帧头  固定值 0x0824
	NJUST_IP_TIME synTime; //  //时刻
	UINT8 GPS_Status; //GPS状态
	UINT8 NAV_Status;  // 惯导状态 见上“惯导组合状态 ”
	
	UINT8 IS_NAV_Err; //导航数据是否有错: 0-无错,非零-有错
	UINT32 navID; //导航信息编号(从0开始)
	//位姿
	double Longitude_degree; //单位:度  
	double Latitude_degree; //单位:度
	double Altitude_m; //单位:米
	double EarthRefCoord[2]; // 地面平面坐标系 单位:米  [0] 北向 +X     [1] 东向 +Y   注意：暂时以校内智能楼差分站为参考基准
	
	float Roll_rad; //单位:弧度 
	float Pitch_rad; //单位:弧度
	float Yaw_rad; //单位:弧度
	//速度
	INT16 Speed_cm_ps; //单位:厘米/秒
	INT16 SPD_LefWheel_cm_ps; //单位:厘米/秒
	INT16 SPD_RightWhell_cm_ps; //单位:厘米/秒
	INT16 IMU_R_xyz_mrad[3];  //角速率:毫弧度
	INT16 IMU_A_xyz_mm_ps[3];  //IMU加速度:毫米
	//GPS
	UINT16 GpsSol_pDOP; //0.01,Postition DOP
	UINT8  GpsSol_numSV; //Number of Svs used in navigation solution;		
	UINT16 GpsSol_ecefX_cm; //ECEF X: 单位厘米
	UINT16 GpsSol_ecefY_cm; //ECEF Y: 单位厘米
	UINT16 GpsSol_ecefZ_cm; //ECEF Z: 单位厘米
	//安全
	UINT16 nSize; //结构体大小 
	UINT8 checksum;  //Checksum of above data.
}NJUST_MC_NAV_INFO;
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  驱动信息
//
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_MC_DRIVE_INFO
{
	UINT16 Head;
	UINT32 FrameID; // 数据帧头  固定值 0x0724
	NJUST_IP_TIME synTime; //时刻
	UINT8 GPS_Status; //GPS状态
	UINT8 IS_NAV_Err; //导航数据是否有错: 0-无错,非零-有错
	UINT32 navID; //导航信息编号(从0开始)
	//规划
	float Plan_angle_degree; //底层计算得到的车转向角:单位度
	float Real_angle_degree;  //实际车转向角:单位度
	float Real_yaw_rad;//单位:毫弧度
	INT16 Plan_Speed_cm_ps; //规划速度:厘米/秒
	INT16 Real_Speed_cm_ps; //实际速度:厘米/秒
	INT16 Brake_value;  //刹车值
	INT16 Throttle_value; //油门值
	//控制器计算的出来的路径
	NJUST_MC_POINT pPoint[NJUST_MAX_MC_PATH_POINTS_NUM]; //车体坐标系下的路径点
	UINT8 nPoint; //有效点个数
	//运行模式
	UINT8 runMode;//自主标志位, 0-人工,1-自主
	//控制相关
	UINT8 bSTOP_Emergency; //紧急停车
	UINT8 ControllerState; //控制器执行时的状态
	//安全
	UINT16 nSize; //结构体大小 
	UINT8 checksum;  //Checksum of above data.
}NJUST_MC_DRIVE_INFO;

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  恢复对齐方式
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//恢复对齐状态

#endif


