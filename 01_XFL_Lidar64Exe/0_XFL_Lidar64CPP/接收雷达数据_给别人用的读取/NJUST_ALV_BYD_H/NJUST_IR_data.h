////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_IR_data.h
//  Author: 任明武
//  Date:   2017.7.15
//  Description: 红外热像仪的数据处理模块
//  目前输出：
//          自然道边,水坑
//  Modification: 
//          2016.7.15, 任明武
//  Declare:
//          NJUST_IR_RES_TO_FU
//          NJUST_IR_INF_TO_MO
//          NJUST_IR_IMG_TO_MO
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_IR_DATA_H_
#define _NJUST_IR_DATA_H_

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
#define NJUST_IR_MAX_NATURE_BOUNDARY_NUM    (2+4)   //自然道边有2条最佳道边,4条候选道边

//图像数据
#define NJUST_IR_MAX_FILENAME_LEN           255     //文件名的最大长度
#define NJUST_IR_MAX_IP_IMG_WIDTH           1024    //IP传输的图像数据的最大宽度
#define NJUST_IR_MAX_IP_IMG_HEIGHT          1024    //IP传输的图像数据的最大高度
#define NJUST_IR_MAX_IP_IMG_SIZE           (NJUST_IR_MAX_IP_IMG_WIDTH*NJUST_IR_MAX_IP_IMG_HEIGHT)  //IP传输的图像数据的最大字节个数

//数据定义
#define NJUST_IR_MAX_POINT_NUM              2048   //最多输出2048个点
#define NJUST_IR_MAX_DBG_INFO_LEN           1024   //调试信息最多1024个字符

//数据类型
enum NJUST_IR_OBJECT_TYPE
{
   NJUST_IR_BOUNDARY_LEFT_BEST = 0x00,      //自然道边的最佳左边界
   NJUST_IR_BOUNDARY_LEFT_1_MAYBE,          //自然道边的可能左边界1
   NJUST_IR_BOUNDARY_LEFT_2_MAYBE,          //自然道边的可能左边界2
   NJUST_IR_BOUNDARY_RIGHT_BEST,            //自然道边的最佳右边界
   NJUST_IR_BOUNDARY_RIGHT_1_MAYBE,         //自然道边的可能右边界1
   NJUST_IR_BOUNDARY_RIGHT_2_MAYBE,         //自然道边的可能右边界2
   NJUST_IR_SUSPICIOUS_OBJECT_WATER,        //可能是水面
   NJUST_IR_SUSPICIOUS_OBJECT_VEHICLE,      //可能是车辆
   NJUST_IR_SUSPICIOUS_OBJECT_PEDESTRIAN,   //可能是行人
   NJUST_IR_OBJECT_TYPE_TOTAL_NUM	        //最多类型数
};

typedef struct tagNJUST_IR_POINT
{
    int  x_cm;     //点的X坐标,车体坐标
    int  y_cm;     //点的Y坐标,车体坐标
	NJUST_IR_OBJECT_TYPE type; //点的类型
	int objectID; //多个点属于同一个目标,比如水坑的描述需要多个点,而图像中也可能有多个水坑;从0编起.
}NJUST_IR_POINT;	

//IR给融合模块的数据
typedef struct tagNJUST_IR_RESULT
{
	int frameID;   //图像的帧号(从0开始)
	NJUST_IP_TIME synTime; //图像获取的时间
    int navID;  //与图像获取时间最接近的导航数据编号(从0开始)
    int width; //图像的宽度
	int height; //图像的高度
	NJUST_IR_POINT pPoint[NJUST_IR_MAX_POINT_NUM];  //各点数据
	int nPoint; //有效点的个数
	int nSize; //该结构体的大小
	unsigned char checksum;  //检查和:以上数据之和
}NJUST_IR_RES_TO_FU;

//IR给显控终端的调试信息
typedef struct NJUST_IR_DBG_INFO
{
	int frameID; //图像帧号(从0开始)
	NJUST_IP_TIME synTime;   //系统时间
	char pInfo[NJUST_IR_MAX_DBG_INFO_LEN+1]; //调试信息
	int nLen;  //调试信息的实际字符个数(不包含字符串结尾字符0)
	int nSize; //该结构体的大小
	unsigned char checksum;  //检查和:以上数据之和
}NJUST_IR_INF_TO_MO;

//IR给显控终端的图像信息
typedef struct NJUST_IR_IMAGE
{
    char pFileName[NJUST_IR_MAX_FILENAME_LEN+1]; //文件名
	int frameID; //图像帧号(从0开始)
	int width; //图像高度
	int height; //图像宽度
    int nBits;  //1-二值图像; 8-灰度图像; 24-彩色图像
    unsigned char pData[NJUST_IR_MAX_IP_IMG_SIZE]; //图像数据 
	int nSize; //该结构体的大小
	unsigned char checksum;  //检查和:以上数据之和
}NJUST_IR_IMG_TO_MO; 

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  恢复对齐方式
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//恢复对齐状态

#endif
