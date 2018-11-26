////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_2D_data.h
//  Author: 王欢
//  Date:   2015.5.14
//  Description: 2D模块的数据定义
//  Modification:
//          2015.6.26, 任明武
//  Modification:
//          2015.8.5, 王欢
//  Declare:
//          NJUST_2D_RES_TO_FU
//          NJUST_2D_INF_TO_MO
//          NJUST_2D_IMG_TO_MO
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_2D_DATA_H_
#define _NJUST_2D_DATA_H_

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
#define NJUST_2D_MAX_CAMERA_NUM             8       //最多8个相机,编号[0..7]
#define NJUST_2D_MAX_NATURE_BOUNDARY_NUM    (2*3)   //自然道边有2条最佳道边,4条候选道边
#define NJUST_2D_MAX_LANE_MAKDERS_NUM       6       //最多输出6条黄线,6条白线

//图像数据
#define NJUST_2D_MAX_FILENAME_LEN           255     //文件名的最大长度
#define NJUST_2D_MAX_IP_IMG_WIDTH           1024    //IP传输的图像数据的最大宽度
#define NJUST_2D_MAX_IP_IMG_HEIGHT          1024    //IP传输的图像数据的最大高度
#define NJUST_2D_MAX_IP_IMG_SIZE           (NJUST_2D_MAX_IP_IMG_WIDTH*NJUST_2D_MAX_IP_IMG_HEIGHT*3)  //IP传输的图像数据的最大字节个数

//数据定义
#define NJUST_2D_MAX_POINT_NUM              2048   //最多输出2048个点
#define NJUST_2D_MAX_DBG_INFO_LEN           1024   //调试信息最多1024个字符

//数据类型
enum NJUST_2D_OBJECT_TYPE
{
   NJUST_2D_BOUNDARY_LEFT_BEST = 0x00,      //自然道边的最佳左边界
   NJUST_2D_BOUNDARY_LEFT_1_MAYBE,          //自然道边的可能左边界1
   NJUST_2D_BOUNDARY_LEFT_2_MAYBE,          //自然道边的可能左边界2
   NJUST_2D_BOUNDARY_RIGHT_BEST,            //自然道边的最佳右边界
   NJUST_2D_BOUNDARY_RIGHT_1_MAYBE,         //自然道边的可能右边界1
   NJUST_2D_BOUNDARY_RIGHT_2_MAYBE,         //自然道边的可能右边界2
   NJUST_2D_LANE_MARKER_1_WHITE,            //第1条白线
   NJUST_2D_LANE_MARKER_2_WHITE,            //第2条白线
   NJUST_2D_LANE_MARKER_3_WHITE,            //第3条白线
   NJUST_2D_LANE_MARKER_4_WHITE,            //第4条白线
   NJUST_2D_LANE_MARKER_5_WHITE,            //第5条白线
   NJUST_2D_LANE_MARKER_6_WHITE,            //第6条白线
   NJUST_2D_LANE_MARKER_1_YELLOW,           //第1条黄线
   NJUST_2D_LANE_MARKER_2_YELLOW,           //第2条黄线
   NJUST_2D_LANE_MARKER_3_YELLOW,           //第3条黄线
   NJUST_2D_LANE_MARKER_4_YELLOW,           //第4条黄线
   NJUST_2D_LANE_MARKER_5_YELLOW,           //第5条黄线
   NJUST_2D_LANE_MARKER_6_YELLOW,           //第6条黄线
   NJUST_2D_STOP_LINE,                      //车辆停止线
   NJUST_2D_ZEBRA_CROSSING,                 //人行斑马戏
   NJUST_2D_SUSPICIOUS_OBJECT_WATER,        //可能是水面
   NJUST_2D_SUSPICIOUS_OBJECT_VEHICLE,      //可能是车辆
   NJUST_2D_SUSPICIOUS_OBJECT_PEDESTRIAN,   //可能是行人
   NJUST_2D_OBJECT_TYPE_TOTAL_NUM	          //最多类型数
};

typedef struct tagNJUST_2D_POINT
{
    int  x_cm;     //点的X坐标,车体坐标
    int  y_cm;     //点的Y坐标,车体坐标
	NJUST_2D_OBJECT_TYPE type; //点的类型
}NJUST_2D_POINT;

//2D给融合模块的数据
typedef struct tagNJUST_2D_RESULT
{
	int camerID; //摄像机的ID号(从0开始)
	int frameID;   //图像的帧号(从0开始)
	NJUST_IP_TIME synTime; //图像获取的时间
  int navID;  //与图像获取时间最接近的导航数据编号(从0开始)
  int width; //图像的宽度
	int height; //图像的高度
	NJUST_2D_POINT pPoint[NJUST_2D_MAX_POINT_NUM];  //各点数据
	int nPoint; //有效点的个数
	int nSize; //该结构体的大小
	unsigned char checksum;  //检查和:以上数据之和
}NJUST_2D_RES_TO_FU;

//2D给显控终端的调试信息
typedef struct NJUST_2D_DBG_INFO
{
	int camerID; //摄像机的ID号(从0开始)
	int frameID; //图像帧号(从0开始)
	NJUST_IP_TIME synTime;   //系统时间
	char pInfo[NJUST_2D_MAX_DBG_INFO_LEN+1]; //调试信息
	int nLen;  //调试信息的实际字符个数(不包含字符串结尾字符0)
	int nSize; //该结构体的大小
	unsigned char checksum;  //检查和:以上数据之和
}NJUST_2D_INF_TO_MO;

//2D给显控终端的图像信息
typedef struct NJUST_2D_IMAGE
{
    char pFileName[NJUST_2D_MAX_FILENAME_LEN+1]; //文件名
	int frameID; //图像帧号(从0开始)
	int width; //图像高度
	int height; //图像宽度
    int nBits;  //1-二值图像; 8-灰度图像; 24-彩色图像
    unsigned char pData[NJUST_2D_MAX_IP_IMG_SIZE]; //图像数据
	int nSize; //该结构体的大小
	unsigned char checksum;  //检查和:以上数据之和
}NJUST_2D_IMG_TO_MO;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  恢复对齐方式
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//恢复对齐状态

#endif
