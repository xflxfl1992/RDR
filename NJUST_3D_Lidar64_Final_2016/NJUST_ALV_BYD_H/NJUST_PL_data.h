////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 软件工程系
//  FileName:      NJUST_PL_data.h
//  Author:        杜鹏桢
//  Date:          2015.6.6
//  Description:   局部路径规划模块和其他模块间的命令和状态
//  Modification:
//                 2015.7.2, 任明武
//  Modification:
//                 2015.8.3, 李强
//  Modification:
//                 2015.8.11, 夏青元,袁峻
//  Modification:
//                 2015.9.11
//  Modification:
//                 2016.8.26
//                 杜泽炎: 增加,NJUST_PL_3D64_COMMAND_TYPE
//                         修改,NJUST_PL_TO_MAP
//                         修改,NJUST_PL_MAP_COMMAND_TYPE
//
//  Declare:
//                 NJUST_PL_TO_MC
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_PL_DATA_H_
#define _NJUST_PL_DATA_H_

#include "NJUST_Global_Def.h"
#include "NJUST_IP_comm.h"
#include "NJUST_MC_typedef.h"
#include "NJUST_MAP_data.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  设置对齐方式
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push) //保存对齐状态
#pragma pack(1)    //按1字节对齐,必须放在第一句

//系统定义
#define NJUST_MAX_PL_PATH_POINTS_NUM      20       //规划路径的最多点数

typedef struct tagNJUST_PL_PATH_POINT
{
    INT16  x_cm;		//点的X坐标,车底坐标系
    INT16  y_cm;		//点的Y坐标,车底坐标系
	INT8  x1_dm;		//此处允许的宽度的左区间,单位:分米
	INT8  x2_dm;		//此处允许的宽度的右区间,单位:分米
	INT16  speed_cm_ps;//该点的速度,单位:厘米/秒
}NJUST_PL_PATH_POINT;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  命令定义
//
////////////////////////////////////////////////////////////////////////////////////////////////
//动作
typedef enum NJUST_PL_DRV_TYPE
{
	NJUST_PL_COMMAND_DRV_WAIT = 0x00,           //等待
	NJUST_PL_COMMAND_DRV_GO_FOREWARD,           //前行
	NJUST_PL_COMMAND_DRV_GO_BACKWARD,           //倒车
	NJUST_PL_COMMAND_DRV_TURN_LEFT,             //左拐
	NJUST_PL_COMMAND_DRV_TURN_RIGHT,            //右拐
	NJUST_PL_COMMAND_DRV_TYPE_TOTAL_NUM	       //类型总数
} NJUST_PL_DRV;
//灯光
#define NJUST_PL_COMMAND_ON_LEFT_LAMP        (1<<0)  //亮左灯
#define NJUST_PL_COMMAND_ON_RIGHT_LAMP       (1<<1)  //亮右灯
//喇叭
#define NJUST_PL_COMMAND_HONK_HORN           (1<<0)  //响喇叭
#define NJUST_PL_COMMAND_HONK_ALARM          (1<<1)  //响警报

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  规划发送至控制的数据
//
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_PL_TO_MC
{
	UINT16 Head; // 数据帧头  固定值 0x0624
	UINT32 FrameID;//规划的帧号(从0开始)
	NJUST_IP_TIME synTime; //时刻	//规划时刻
	unsigned int navID;  //与图像获取时间最接近的导航数据编号(从0开始)
	UINT8 drvCmd; //动作命令 见"动作"
	UINT8 lampCmd; //车灯命令
	UINT8 honkCmd; //喇叭命令
	UINT8 nPoint; //有效点个数
	INT16 speed_cm_ps; //规划速度,单位:厘米/秒
	NJUST_PL_PATH_POINT pPoint[NJUST_MAX_PL_PATH_POINTS_NUM]; //车体坐标系下的路径点
	INT32 courseAngleRad; //航向角,单位:0.001,千分之一弧度
	INT32 earthCoord_x;     //车体的大地坐标x
	INT32 earthCoord_y;		//车体的大地坐标y
	UINT16 nSize; //结构体大小
	UINT8 checksum;  //Checksum of above data.
}NJUST_PL_TO_MC;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  规划发送至地图的数据
//
////////////////////////////////////////////////////////////////////////////////////////////////
enum NJUST_PL_MAP_COMMAND_TYPE
{
	NJUST_PL_MAP_COMMAND_TYPE_NO = 0x00,                 //无命令
	NJUST_PL_MAP_COMMAND_TYPE_REPLAN_ASK,           	  //PL请求MAP重规划
	NJUST_PL_MAP_COMMAND_TYPE_CAR_REVERSE_STOP,           //MAP通知PL停止倒车
	NJUST_PL_MAP_COMMAND_TYPE_REACH_ENDPOINT,             //MAP通知PL已到达终点
	NJUST_PL_MAP_COMMAND_TYPE_REACH_TASKPOINT,            //MAP通知PL已到达任务点
    NJUST_PL_MAP_COMMAND_TYPE_TOTAL_NUM	                  //最多命令种类数
};

typedef struct tagNJUST_PL_TO_MAP
{
	int cmdID;
	NJUST_IP_TIME synTime;
	NJUST_PL_MAP_COMMAND_TYPE cmd;  	//1表示进入规划状态，0表示退出规划状态
	NJUST_MAP_GPSPoint GPSPoint;
	int nSize; //结构体大小
	unsigned char checksum;  //Checksum of above data.
}NJUST_PL_TO_MAP;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  规划发送至3D64的数据
//
////////////////////////////////////////////////////////////////////////////////////////////////
enum NJUST_PL_3D64_COMMAND_TYPE
{
	NJUST_PL_3D64_COMMAND_TYPE_NO = 0x00,                 //无命令
	NJUST_PL_3D64_COMMAND_TYPE_REPLAN_ASK,           	  //PL通知3D64重规划
	NJUST_PL_3D64_COMMAND_TYPE_CAR_REVERSE_STOP,           //PL通知3D64停止倒车
	NJUST_PL_3D64_COMMAND_TYPE_SCOUT_START,           //PL通知3D64开始侦查
	NJUST_PL_3D64_COMMAND_TYPE_SCOUT_END,           //PL通知3D64结束侦查
	NJUST_PL_3D64_COMMAND_TYPE_TOTAL_NUM	               //最多命令种类数
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  恢复对齐方式
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//恢复对齐状态

#endif
