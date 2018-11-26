////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_3D64_data.h
//  Author: 蔡云飞
//  Date:   2015.6.27
//  Description: 3D模块的数据定义
//
//  修改记录
//  Date:   2016.5.5,
//          增加栅格类型
//  Date:   2016.7.15
//          任明武
//          增加了岔路数据,见NJUST_3D64_CROSS_TO_FU
//  Date:   2016.8.25
//          徐峰磊
//          增加侦查结构体
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_3D64_DATA_H_
#define _NJUST_3D64_DATA_H_

#include "NJUST_Global_Def.h"
#include "NJUST_IP_comm.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  设置对齐方式
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push) //保存对齐状态
#pragma pack(1)    //按1字节对齐,必须放在第一句

////////////////////////////////////////////////////////////////////////////////////////////
//
//  常量定义
//
////////////////////////////////////////////////////////////////////////////////////////////
#define NJUST_3D64_MAX_OBS_POINT_NUM               4             //一个障碍物最多用多少点表示
#define NJUST_3D64_MAX_OBS_NUM		             150             //障碍物的最大个数
#define NJUST_3D64_MAX_ROAD_POINT_NUM	         200             //道边点的最大数目
#define MAX_OBJ_NUM								 4               //检测出汽油桶的最大个数
////////////////////////////////////////////////////////////////////////////////////////////
//
//  64线雷达
//
////////////////////////////////////////////////////////////////////////////////////////////
//处理范围
#define NJUST_LIDAR64_VERTICAL_DISTANCE_CM       8000           //64线雷达的垂直距离8000cm,车前6000cm,车后2000cm            
#define NJUST_LIDAR64_HORIZONTAL_DISTANCE_CM     4000           //64线雷达的水平距离4000cm,车左2000cm,车右2000cm            
//分辨率
#define NJUST_LIDAR64_VERTICAL_RESOLUTION_CM      20            //64线雷达的垂直分辨率20cm
#define NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM    20            //64线雷达的水平分辨率20cm
//垂直栅格数200个,必须保证是8的倍数
#define NJUST_LIDAR64_VERTICAL_GRID_NUM          (NJUST_LIDAR64_VERTICAL_DISTANCE_CM/NJUST_LIDAR64_VERTICAL_RESOLUTION_CM)           
//水平栅格数400个,必须保证是8的倍数
#define NJUST_LIDAR64_HORIZONTAL_GRID_NUM        (NJUST_LIDAR64_HORIZONTAL_DISTANCE_CM/NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM)
////////////////////////////////////////////////////////////////////////////////////////////
//
//  类型定义
//
////////////////////////////////////////////////////////////////////////////////////////////
//道边类型  
enum  NJUST_3D64_ROAD_POINT_TYPE
{
	NJUST_3D64_ROAD_POINT_TYPE_LEFT = 0x00, //左边界
	NJUST_3D64_ROAD_POINT_TYPE_RIGHT,       //右边界
   	NJUST_3D64_ROAD_POINT_TYPE_TOTAL_NUM    //边界类型总数
};               

//坡度类型
enum  NJUST_3D64_ROAD_SLOPE_TYPE
{
	NJUST_3D64_ROAD_SLOPE_TYPE_FLAT = 0x00, //平地
	NJUST_3D64_ROAD_SLOPE_TYPE_UP,          //上坡
	NJUST_3D64_ROAD_SLOPE_TYPE_DOWN,        //下坡
	NJUST_3D64_ROAD_SLOPE_TYPE_TOTAL_NUM	 //类型总数目
};

//3D模块中的点定义
struct NJUST_3D64_POINT_2D
{
	int      x_cm;                               //x坐标单位 cm
	int      y_cm;                               //y坐标单位 cm
};
//汽油桶的最大边界（栅格的行列号）
struct NJUST_3D64_RECT_2D
{
	int      left_j;                             //汽油桶的最大左边界（栅格的列号）
	int      right_j;                            //汽油桶的最大右边界（栅格的列号）
	int      up_i;                               //汽油桶的最大上边界（栅格的行号）
	int      low_i;                              //汽油桶的最大下边界（栅格的行号）
};

//障碍物
struct NJUST_3D64_OBS_DATA  
{
	unsigned int         OBSID;                                  //障碍物ID
	NJUST_3D64_POINT_2D  pPoint[NJUST_3D64_MAX_OBS_POINT_NUM];   //目测障碍物最左边一个可见点，以下为逆时针旋转其他三个角点。
	int                  nPoint;                                 //障碍物有效点数
	int                  z_cm;                                   //普通负障碍类型=0；壕沟类型=1；
    int                  fbSpeed_cmps;                           //障碍物的前后速度上的分量,单位:厘米/秒
	int                  lrSpeed_cmps;                           //障碍物的左右方向上的速度分量,单位:厘米/秒
};
////////////////////////////////////////////////////////////////////////////////////////////
//
//  3D给融合模块的障碍数据
//
////////////////////////////////////////////////////////////////////////////////////////////
typedef struct NJUST_3D64_OBS                              
{
	int                    frameID;                               //帧ID(从0开始)
	NJUST_IP_TIME          synTime;                               //时间戳
	unsigned int           navID;                                 //与图像获取时间最接近的导航数据编号(从0开始)
	NJUST_3D64_OBS_DATA    pObj[NJUST_3D64_MAX_OBS_NUM];          //障碍物数组
	int                    nObj;                                  //障碍物有效数量
	int                    nSize;                                 //该结构体的大小
	unsigned char		   checksum;                              //检查和:以上数据之和
}NJUST_3D64_OBS_TO_FU;
////////////////////////////////////////////////////////////////////////////////////////////
//
//  3D给融合模块的道路边缘
//
////////////////////////////////////////////////////////////////////////////////////////////
typedef struct NJUST_3D64_ROAD_BOUNDARY 
{
	int                          frameID;                            //帧ID(从0开始)
	NJUST_IP_TIME                synTime;                            //时间戳
	unsigned int                 navID;                              //与图像获取时间最接近的导航数据编号(从0开始)
	NJUST_3D64_POINT_2D          Point[NJUST_3D64_MAX_ROAD_POINT_NUM]; //道边点数组
	int                          nPoint;                             //道边点有效个数
	NJUST_3D64_ROAD_POINT_TYPE   Type[NJUST_3D64_MAX_ROAD_POINT_NUM];  //道边点类型 左边、右边，与道路点数组一一对应
	int                          nSize;                              //该结构体的大小
	unsigned char                checksum;                           //检查和:以上数据之和
}NJUST_3D64_ROAD_BOUNDARY_TO_FU;
////////////////////////////////////////////////////////////////////////////////////////////
//
//  3D给融合模块的道路纵向坡度描述
//
////////////////////////////////////////////////////////////////////////////////////////////
typedef struct NJUST_3D64_ROAD_SLOPE
{
	int                                frameID;                  //帧ID(从0开始)
	NJUST_IP_TIME                      synTime;                  //时间戳
	unsigned int                       navID;                    //与图像获取时间最接近的导航数据编号(从0开始)
	NJUST_3D64_ROAD_SLOPE_TYPE    	   type;                     //坡度类型，平地、上坡、下坡
	int                                degree;                   //坡度，单位: 度
	int                                nSize;                    //该结构体的大小
	unsigned char                      checksum;                 //检查和:以上数据之和
}NJUST_3D64_ROAD_SLOPE_TO_FU;
////////////////////////////////////////////////////////////////////////////////////////////
//
//  3D给融合模块的网格数据
//
////////////////////////////////////////////////////////////////////////////////////////////
//栅格类型
enum NJUST_3D64_GRID_TYPE
{
	NJUST_3D64_GRID_TYPE_NORMAL = 0x00, //平地
	NJUST_3D64_GRID_TYPE_OBS,           //正障碍
	NJUST_3D64_GRID_TYPE_NEGTIVE_OBS,   //负障碍
	NJUST_3D64_GRID_TYPE_WATER,         //水
	NJUST_3D64_GRID_TYPE_OBJ_DETECTED,  //侦查目标
	NJUST_3D64_GRID_TYPE_HIGH_OBS,		//高目标
	NJUST_3D64_GRID_TYPE_TOTAL_NUM      //类型总数,最多不能超过256种.
};
typedef struct tagNJUST_3D64_GRID_TO_FU  
{
	int                    frameID;                               //帧ID(从0开始)
	NJUST_IP_TIME          synTime;                               //时间戳
	unsigned int           navID;                                 //与图像获取时间最接近的导航数据编号(从0开始)
	BYTE                   gridMsk[NJUST_LIDAR64_VERTICAL_GRID_NUM*NJUST_LIDAR64_HORIZONTAL_GRID_NUM];  //高200*宽400
}NJUST_3D64_GRID_TO_FU;

////////////////////////////////////////////////////////////////////////////////////////////
//
//  3D给融合模块的岔路数据
//
////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_3D64_CROSS_TO_FU  
{
	int                    frameID;                               //帧ID(从0开始)
	NJUST_IP_TIME          synTime;                               //时间戳
	unsigned int           navID;                                 //与图像获取时间最接近的导航数据编号(从0开始)
	double                 A,B,C;                                 //岔路中心线的直线方程
	BYTE                   crossMsk[NJUST_LIDAR64_VERTICAL_GRID_NUM*NJUST_LIDAR64_HORIZONTAL_GRID_NUM];  //高200*宽400
}NJUST_3D64_CROSS_TO_FU;
////////////////////////////////////////////////////////////////////////////////////////////
//
//  3D给2D模块的侦查数据
//
////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_3D64_TARGET_TO_2D
{
	int				     frameID;               						//帧ID
	NJUST_IP_TIME        synTime;               						//时间戳
	unsigned int		 navID;           						        //惯导
	NJUST_3D64_POINT_2D  obj_earth_coord[MAX_OBJ_NUM];	                //每个目标的绝对大地坐标系
	NJUST_3D64_RECT_2D   obj_grid_coord[MAX_OBJ_NUM];		            //每个目标的外接矩形
	int				     objNum;                						//目标的实际(例如汽油桶)的个数
	int                  nSize;                                         //该结构体的大小
	unsigned char        checksum;                                      //检查和:以上数据之和
}NJUST_3D64_TARGET_TO_2D;

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  恢复对齐方式
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//恢复对齐状态

#endif
