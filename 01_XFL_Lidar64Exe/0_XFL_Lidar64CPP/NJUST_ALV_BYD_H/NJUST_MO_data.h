////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_MO_data.h
//  Author: 张重阳
//  Date:   2015.6.6
//  Description: 显控模块和其他模块间的命令和状态
//  Modification: 
//          2015.7.2, 任明武
//  Declare:
//          NJUST_FROM_MO_COMMAND
//          NJUST_TO_MO_WORKSTAT
//
//  修改
//  日期：  2016.5.5
//  内容：添加逻辑控制结构体（CFG）
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_MO_DATA_H_
#define _NJUST_MO_DATA_H_

#include "NJUST_Global_Def.h"
#include "NJUST_IP_comm.h" 

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  设置对齐方式
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push) //保存对齐状态
#pragma pack(1)    //按1字节对齐,必须放在第一句

//系统定义
#define  NJUST_MO_MAX_COMMAND_PARA_LEN          32     //最大命令参数长度
#define  NJUST_MO_WORK_STAT_ERRMSG_MAXLEN       32     //工作状态错误消息的最大长度
#define  NJUST_MO_DEBUGMSG_MAXLEN              128     //调试信息的最大长度

#define  NJUST_MO_MAX_CFG_NUM               32    //一个模块最多逻辑指令个数
#define  NJUST_MO_MAX_CFG_PARA_LEN          32     //最大逻辑命令参数长度
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  命令: 显控发给各模块
//
////////////////////////////////////////////////////////////////////////////////////////////////
enum NJUST_MO_COMMAND_TYPE
{
	NJUST_MO_COMMAND_TYPE_NO = 0x00,                  //无命令
	NJUST_MO_COMMAND_TYPE_COMPUTER_RESTART,           //计算机重新启动
	NJUST_MO_COMMAND_TYPE_COMPUTER_SHUTDOWN,          //计算机关机
	NJUST_MO_COMMAND_TYPE_MODULE_RESTART,             //模块重新启动
	NJUST_MO_COMMAND_TYPE_MODULE_DEBUG,               //模块切换到调试模式
	NJUST_MO_COMMAND_TYPE_MODULE_RELEASE,             //模块切换到运行模式
    NJUST_MO_COMMAND_TYPE_TOTAL_NUM	                 //最多命令种类数
};

enum NJUST_MO_MODULE_ID
{

};

typedef struct tagNJUST_FROM_MO_COMMAND
{   
	int frameID; //命令编号(从0开始)
	NJUST_IP_TIME synTime;  //系统时间
    int moduleID;   //模块ID
    NJUST_MO_COMMAND_TYPE cmd; //命令
    char pPara[NJUST_MO_MAX_COMMAND_PARA_LEN+1]; //命令参数
	int nPara; //有效参数的字节数
	int nSize; //该结构体的大小 
}NJUST_FROM_MO_COMMAND;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  模块工作状态: 各模块发给显控
//
////////////////////////////////////////////////////////////////////////////////////////////////
enum NJUST_MO_MODULE_STATUS
{ 
    NJUST_MO_WORK_STAT_INVAILD = 0x00, //模块无结果输出
    NJUST_MO_WORK_STAT_VAILD,          //模块输出有效
    NJUST_MO_WORK_STAT_TOTAL_NUM	  //最多状态种类数
};
enum NJUST_MO_MODULE_ERRCODE
{ 
    NJUST_MO_ERRCODE_NOERR = 0x00,   //无错误
    NJUST_MO_ERRCODE_ERROR,          //错误
    NJUST_MO_ERRCODE_TOTAL_NUM	    //最多错误码种类数
};

//各模块给显控的工作状态数据
typedef struct tagNJUST_TO_MO_WORKSTAT
{
	NJUST_IP_TIME synTime;  //系统时间
    int moduleID;   //模块ID
	int myselfTimeOutMS; //模块的超时,单位:ms;若总控在该时间内没有收到状态数据,则认为本模块不在线
    NJUST_MO_MODULE_STATUS stat;    // 工作状态
	int PELR; //按千分比表示的丢包率
	int timeConsumingMS; //本帧处理所用的时间,单位ms
	NJUST_MO_MODULE_ERRCODE errCode;     //错误码
    char pErrMsg[NJUST_MO_WORK_STAT_ERRMSG_MAXLEN+1];   //错误消息
}NJUST_TO_MO_WORKSTAT;


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  逻辑控制设置：显控发给各模块
//
////////////////////////////////////////////////////////////////////////////////////////////////
enum NJUST_MO_CFG_TYPE
{
	NJUST_MO_CFG_TYPE_NO = 0x00,                 //无配置 
	NJUST_MO_CFG_TYPE_USE_MAPINFO,               //使用MAP信息
	NJUST_MO_CFG_TYPE_PL_ZEBRA_SLOW,             //PL遇到人行横道线减速
	NJUST_MO_CFG_TYPE_MAP_SEND_MANMADE_OBS,      //MAP发送人工标记的障碍
	NJUST_MO_CFG_TYPE_2D_3D_CONFIRM_MANMADE_OBS, //环境感知是否再次辨认人工发用的人工障碍
	NJUST_MO_CFG_TYPE_FU_OBS_REMAIN_TIME,        //FU对障碍的保持帧数
	NJUST_MO_CFG_TYPE_PL_FOLLOW_GPS,             //PL是否跟随GPS点
	NJUST_MO_CFG_TYPE_FU_USE_2DINF,              //FU是否使用2D信息
	NJUST_MO_CFG_TYPE_FU_USE_LSINF,              //FU是否使用交通灯，交通标志信息
	NJUST_MO_CFG_TYPE_FU_USE_3D64GRIDOBSINF,     //FU是否使用64线栅格障碍
	NJUST_MO_CFG_TYPE_FU_USE_3D64RECTOBSINF,     //FU是否使用64线矩形障碍
	NJUST_MO_CFG_TYPE_FU_USE_3D4INF,             //FU是否使用4线雷达
	NJUST_MO_CFG_TYPE_FU_USE_3DESRINF,           //FU是否使用毫米波
	NJUST_MO_CFG_TYPE_2D_ROADSTRPRO,             //2D道路结构化属性
	NJUST_MO_CFG_TYPE_2D_STRAIGHT_DETOUR_BYROAD, //2D选择直路，弯路，岔路
	NJUST_MO_CFG_TYPE_2D_EXECUTE_ALGORITHM,      //2D是否执行算法
	NJUST_MO_CFG_TYPE_2D_IMAGE_SAVE_FRAME_RATE,  //2D图像保存帧率
	NJUST_MO_CFG_TYPE_TOTAL_NUM	  		         //最多配置种类数
};

typedef struct tagNJUST_MO_CFG
{
	NJUST_MO_CFG_TYPE cfg; //配置的类型
	int value; //配置的值,0-无效，1-有效,其他-数值
}NJUST_MO_CFG;

typedef struct tagNJUST_FROM_MO_CFG
{
	int frameID; //命令编号(从0开始)
	NJUST_IP_TIME synTime;  //系统时间
	NJUST_MO_CFG pCFG[NJUST_MO_MAX_CFG_NUM]; //各配置
	int nCFG; //有效配置的个数
	int nSize; //该结构体的大小
	unsigned char checksum; //检查和:以上数据之和
}NJUST_FROM_MO_CFG;

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  恢复对齐方式
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//恢复对齐状态

#endif

