////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_LS_data.h
//  Author: 张重阳
//  Date:   2015.6.6
//  Description: 感知灯光模块和交通标识模块,给融合模块点的数据
//  Modification: 
//          2015.6.26, 任明武
//  Declare:
//          NJUST_LS_SIGN_RES_TO_FU
//          NJUST_LS_LIGHT_RES_TO_FU
//          NJUST_LS_INF_TO_MO
//          NJUST_LS_IMG_TO_MO
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_LS_DATA_H__
#define _NJUST_LS_DATA_H__

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
#define NJUST_LS_MAX_TRAFFIC_SIGN_NUM         12    //一幅图像最多输出12个交通标识
#define NJUST_LS_MAX_TRAFFIC_LIGHT_NUM        12    //一幅图像最多输出12个交通灯
#define NJUST_LS_MAX_DBG_INFO_LEN           1024    //调试信息最多1024个字符

//图像数据
#define NJUST_LS_MAX_FILENAME_LEN           255     //文件名的最大长度
#define NJUST_LS_MAX_IP_IMG_WIDTH           1024    //IP传输的图像数据的最大宽度
#define NJUST_LS_MAX_IP_IMG_HEIGHT          1024    //IP传输的图像数据的最大高度
#define NJUST_LS_MAX_IP_IMG_SIZE           (NJUST_LS_MAX_IP_IMG_WIDTH*NJUST_LS_MAX_IP_IMG_HEIGHT*3)  //IP传输的图像数据的最大字节个数

//数据类型定义
typedef struct tagNJUST_LS_RECT
{
    int left; //图像坐标
	int right; //图像坐标 
    int top; //图像坐标
	int bottom; //图像坐标
}NJUST_LS_RECT;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  交通标识
//
////////////////////////////////////////////////////////////////////////////////////////////////
enum NJUST_LS_SIGNAL_TYPE
{
	NJUST_LS_SIGN_TYPE_INVALID = 0x00,			// 0-invalid or fail detect
	NJUST_LS_SIGN_TYPE_FORBID_LEFT,				// 1-forbid left turn
	NJUST_LS_SIGN_TYPE_FORBID_RIGHT,			// 2-forbid right turn 
	NJUST_LS_SIGN_TYPE_FORBID_STRAIGHT,			// 3-forbid straight 
	NJUST_LS_SIGN_TYPE_FORBID_LEFT_RIGHT,		// 4-forbid left turn and right turn 
	NJUST_LS_SIGN_TYPE_FORBID_STRAIGHT_LEFT,	// 5-forbid straight and left turn 
	NJUST_LS_SIGN_TYPE_FORBID_STRAIGHT_RIGHT,	// 6-forbid straight and right turn 
	NJUST_LS_SIGN_TYPE_FORBID_DRIVE_INTO,		// 7-forbid drive into 
	NJUST_LS_SIGN_TYPE_STOP,					// 8-forbid stop 
	NJUST_LS_SIGN_TYPE_SLOW_DOWN,				// 9-slow down 
	NJUST_LS_SIGN_TYPE_PEDESTRIAN,				// 10-be careful of pedestrian 
	NJUST_LS_SIGN_TYPE_UNEVEN_ROAD,				// 11-uneven road 
	NJUST_LS_SIGN_TYPE_EMERGENCY_STOP,			// 12-emergency stop region 
	NJUST_LS_SIGN_TYPE_STRAIGHT,				// 13-straight forward 
	NJUST_LS_SIGN_TYPE_LEFT_TURN,				// 14-left turn forward 
	NJUST_LS_SIGN_TYPE_RIGHT_TURN,				// 15-right turn forward 
	NJUST_LS_SIGN_TYPE_STRAIGHT_LEFT,			// 16-straight or left turn forward 
	NJUST_LS_SIGN_TYPE_STRAIGHT_RIGHT,			// 17-straight or right turn forward 
	NJUST_LS_SIGN_TYPE_LEFT_RIGHT,				// 18-left turn or right turn forward 
	NJUST_LS_SIGN_TYPE_ALONG_RIGHT_SIDE,		// 19-drive along with right side 
	NJUST_LS_SIGN_TYPE_ALONG_LEFT_SIDE,			// 20-drive along with left side 
	NJUST_LS_SIGN_TYPE_HONKING,					// 21-hoking 
	NJUST_LS_SIGN_TYPE_DRIVE_AROUND_ISLAND,		// 22-drive along the island 
	NJUST_LS_SIGN_TYPE_CROSSWALK,				// 23-crosswalk in front 
	NJUST_LS_SIGN_TYPE_U_TURN,					// 24-allow u_turn 
	NJUST_LS_SIGN_TYPE_PARKING,					// 25-parking in front 
	NJUST_LS_SIGN_TYPE_TRAFFIC_MARK_CONE,		// 26-traffic mark cone 
	NJUST_LS_SIGN_TYPE_LEFT_FORWORD,			// 27-drive left forward for construction
	NJUST_LS_SIGN_TYPE_RIGHT_FORWORD,			// 28-drive right forward for construction
	NJUST_LS_SIGN_TYPE_CHILD,					// 29-be careful of child 
	NJUST_LS_SIGN_TYPE_CONSTRUCTION,			// 30-be careful of construction site 
	NJUST_LS_SIGN_TYPE_SPEED_LIMIT,				// 31-Speed limit 
	NJUST_LS_SIGN_TYPE_TOTAL_NUM		        // 32-total number of events 
};

typedef struct tgaNJUST_LS_TRAFFIC_SIGN
{
    int signType;  //标识类型
    int value;     //其值
    int reliability;  //可信度[0-100].
    NJUST_LS_RECT pos;  //在图像中的位置
}NJUST_LS_TRAFFIC_SIGN;

//交通标识给融合模块的数据
typedef struct NJUST_LS_SIGN_RESULT
{
	int frameID;   //图像的帧号(从0开始)
	NJUST_IP_TIME synTime; //图像获取的时间
    int navID;  //与图像获取时间最接近的导航数据编号(从0开始)
    NJUST_LS_TRAFFIC_SIGN pSign[NJUST_LS_MAX_TRAFFIC_SIGN_NUM]; //各标识
    int nSign; //有效标识的个数
	int nSize; //该结构体的大小
	unsigned char checksum; //检查和:以上数据之和
}NJUST_LS_SIGN_RES_TO_FU;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  交通灯描述
//
////////////////////////////////////////////////////////////////////////////////////////////////
//灯的颜色
enum NJUST_LS_LIGHT_COLOR
{
	NJUST_LS_LIGHT_COLOR_UNKNOWN = 0x00,   //未知
	NJUST_LS_LIGHT_COLOR_RED,			   //红灯
	NJUST_LS_LIGHT_COLOR_GREEN,			   //绿灯
	NJUST_LS_LIGHT_COLOR_YELLOW,		   //黄灯
	NJUST_LS_LIGHT_COLOR_TOTAL_NUM         //总的色灯种类数
};

//灯控类型
enum NJUST_LS_LIGHT_TYPE
{
    NJUST_LS_LIGHT_TYPE_UNKNOWN = 0x00,    //未知
    NJUST_LS_LIGHT_TYPE_ROUND,              //圆形
    NJUST_LS_LIGHT_TYPE_LEFT,               //左拐
    NJUST_LS_LIGHT_TYPE_RIGHT,              //右拐
    NJUST_LS_LIGHT_TYPE_FORWARD,            //前进
    NJUST_LS_LIGHT_TYPE_BACKWARD,           //掉头
	NJUST_LS_LIGHT_TYPE_TOTAL_NUM          //总的类型种类数
};

//灯的描述
typedef struct tagNJUST_LS_TRAFIIC_LIGHT
{
    NJUST_LS_LIGHT_COLOR color;     //灯的颜色
    NJUST_LS_LIGHT_TYPE type;   //灯的类型
    int reliability;  //可信度[0-100].
    NJUST_LS_RECT pos;  //在图像中的位置
}NJUST_LS_TRAFIIC_LIGHT;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  通行状态
//
////////////////////////////////////////////////////////////////////////////////////////////////
//行为
enum NJUST_LS_LIGHT_MOVING_DIR
{
    NJUST_LS_LIGHT_MOVING_DIR_UNKNOWN = 0x00,  //未知
    NJUST_LS_LIGHT_MOVING_DIR_ROUND,            //各方向
    NJUST_LS_LIGHT_MOVING_DIR_LEFT,             //左拐
    NJUST_LS_LIGHT_MOVING_DIR_RIGHT,            //右拐
    NJUST_LS_LIGHT_MOVING_DIR_FORWARD,          //前进
    NJUST_LS_LIGHT_MOVING_DIR_BACKWARD,         //掉头
	NJUST_LS_LIGHT_MOVING_DIR_TOTAL_NUM        //总的运动方向类别数
};
//可行
enum NJUST_LS_LIGHT_PASSING_STATUS
{
    NJUST_LS_LIGHT_PASS_UNKNOWN = 0x00,         //未知
    NJUST_LS_LIGHT_PASS_PASSABLE,               //可通行
    NJUST_LS_LIGHT_PASS_PASSABLE_COUNTDOWN,     //通行倒计时
    NJUST_LS_LIGHT_PASS_IMPASSABLE,             //不可通行
    NJUST_LS_LIGHT_PASS_IMPASSABLE_COUNTDOWN,   //不可通行倒计时
};
//通行状态
typedef struct tagNJUST_LS_LIGHT_MOVING
{  
	NJUST_LS_LIGHT_MOVING_DIR dir; //运动方向 
	NJUST_LS_LIGHT_PASSING_STATUS status; //通行状态
    int nCountDownSecond;   //倒计时秒数
}NJUST_LS_LIGHT_MOVING;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  交通灯给融合模块的数据
//
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_LS_LIGHT_RESULT
{
	int frameID; //图像帧号(从0开始)
	NJUST_IP_TIME synTime;   //系统时间
	int navID;  //与图像获取时间最接近的导航数据编号(从0开始)
    int width; //图像的宽度
	int height; //图像的高度
    //交通灯
	NJUST_LS_TRAFIIC_LIGHT pLight[NJUST_LS_MAX_TRAFFIC_LIGHT_NUM]; //各个灯
	int nLight; //有效的灯的个数
	//通行状态
	NJUST_LS_LIGHT_MOVING pMoving[NJUST_LS_MAX_TRAFFIC_LIGHT_NUM]; //各状态
	int nMoving;
	//该结构体的大小 
	int nSize;
	//检查和:以上数据之和
	unsigned char checksum;
}NJUST_LS_LIGHT_RES_TO_FU;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  LS给显控模块的数据
//
////////////////////////////////////////////////////////////////////////////////////////////////
//LS给显控终端的调试信息
typedef struct tagNJUST_LS_DBG_INFO
{
	int camerID; //摄像机的ID号(从0开始)
	int frameID; //图像帧号(从0开始)
	NJUST_IP_TIME synTime;   //系统时间
    int width; //图像的宽度
	int height; //图像的高度
	char pInfo[NJUST_LS_MAX_DBG_INFO_LEN+1]; //调试信息
	int nLen;  //调试信息的实际字符个数(不包含字符串结尾字符0)
	int nSize; //该结构体的大小 
}NJUST_LS_INF_TO_MO;

//LS给显控终端的图像信息
typedef struct tagNJUST_LS_IMAGE
{
    char pFileName[NJUST_LS_MAX_FILENAME_LEN+1]; //文件名
    int width; //图像高度
	int height; //图像宽度
    int nBits;  //1-二值图像; 8-灰度图像; 24-彩色图像
    unsigned char *pData; //图像数据 
	int nSize; //该结构体的大小 
	unsigned char checksum; //检查和:以上数据之和
}NJUST_LS_IMG_TO_MO; 

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  恢复对齐方式
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//恢复对齐状态

#endif
