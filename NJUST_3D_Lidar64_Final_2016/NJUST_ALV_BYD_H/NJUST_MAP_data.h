
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_MAP_data.h
//  Author: 蔡云飞
//  Date:   2015.6.6
//  Description: 地图数据
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_MAP_DATA_H_
#define _NJUST_MAP_DATA_H_
#include <stdio.h>
#include <stdlib.h>

#include "NJUST_Global_Def.h"
#include "NJUST_IP_comm.h" 

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  设置对齐方式
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push) //保存对齐状态
#pragma pack(1)    //按1字节对齐,必须放在第一句

//全局变量
#define NJUST_MAP_GPS_POINT    20
#define NJUST_MAP_OBSTACLE_POINT 20
#define NJUST_MAP_DETECT_OBSTACLE_RADIUS_M 100 //检测障碍物的半径(M)  
//道路类型
enum  NJUST_MAP_ROAD_TYPE
{
    NJUST_MAP_ROAD_TYPE_NONE =0x00,         //未知
	NJUST_MAP_ROAD_TYPE_ASPHALT,            //柏油路
	NJUST_MAP_ROAD_TYPE_CEMENT,             //水泥路
	NJUST_MAP_ROAD_TYPE_DIRT,               //土路
	NJUST_MAP_ROAD_TYPE_COBBLED,            //石子路
	NJUST_MAP_ROAD_TYPE_HIGHWAY,            //高速公路
	NJUST_MAP_ROAD_TYPE_BRIDGE,             //桥梁
	NJUST_MAP_ROAD_TYPE_TUNNEL,             //隧道
	NJUST_MAP_ROAD_TYPE_CULVERT,             //涵洞
	NJUST_MAP_ROAD_TYPE_TOTAL_NUM           //结尾
};

 //行道线
enum   NJUST_MAP_LANE_LINE_TYPE
{
    NJUST_MAP_LANE_LINE_TYPE_NONE = 0x00,              //无行道线
    NJUST_MAP_LANE_LINE_TYPE_WHITEDOTTEDLINE,          //白色虚线
    NJUST_MAP_LANE_LINE_TYPE_WHITESOLIDLINE,           //白色实线
    NJUST_MAP_LANE_LINE_TYPE_YELLOWDOTTEDLINE,         //黄色虚线
    NJUST_MAP_LANE_LINE_TYPE_YELLOWSOLIDLINE,          //黄色实线
	NJUST_MAP_LANE_LINE_TYPE_TOTAL_NUM          //结尾
};

//道路左边界
enum   NJUST_MAP_ROAD_BOUNDARY_TYPE
{
    NJUST_MAP_ROAD_BOUNDARY_TYPE_NONE = 0x00,                    //未知
    NJUST_MAP_ROAD_BOUNDARY_TYPE_CONVEXBOUNDARY,                 //凸边界
    NJUST_MAP_ROAD_BOUNDARY_TYPE_CONCAVEBOUNDARY,                //凹边界
    NJUST_MAP_ROAD_BOUNDARY_TYPE_DANGEROUSBOUNDARY,             //危险边界（如悬崖，探测不到底）
	NJUST_MAP_ROAD_BOUNDARY_TYPE_TOTAL_NUM           //结尾
};

//路口类型
enum   NJUST_MAP_NODE_TYPE
{
    NJUST_MAP_NODE_TYPE_NONE = 0x00,                    //未知
    NJUST_MAP_NODE_TYPE_CROSSROADS ,                    //十字路口
    NJUST_MAP_NODE_TYPE_TNODE,                          //丁字路口
    NJUST_MAP_NODE_TYPE_YNODE,                          //Y路口
    NJUST_MAP_NODE_TYPE_RIGHTANGLENODE,                 //直角路口
    NJUST_MAP_NODE_TYPE_STRAIGHTLINENODE ,              //直行路口
	NJUST_MAP_NODE_TYPE_TOTAL_NUM           //结尾
};

enum   NJUST_MAP_NODE_PASS_TYPE
{
    NJUST_MAP_NODE_PASS_TYPE_NONE = 0x00,                //未知
    NJUST_MAP_NODE_PASS_TYPE_STRAIGHTLINE,               //直行通过
    NJUST_MAP_NODE_PASS_TYPE_TURNLEFT,                   //左转弯
    NJUST_MAP_NODE_PASS_TYPE_TURNRIGHT,                  //右转弯
    NJUST_MAP_NODE_PASS_TYPE_TURNAROUND,                 //掉头
	NJUST_MAP_NODE_PASS_TYPE_TOTAL_NUM           //结尾
};
//红绿灯位置
enum   NJUST_MAP_TRAFFIC_LIGHTS_POSITION
{
    NJUST_MAP_TRAFFIC_LIGHTS_POSITION_NONE = 0x00,            //未知
    NJUST_MAP_TRAFFIC_LIGHTS_POSITION_RIGHTFRONT,             //右前
    NJUST_MAP_TRAFFIC_LIGHTS_POSITION_LEFTRONT,               //左前
    NJUST_MAP_TRAFFIC_LIGHTS_POSITION_FRONT,                  //正前
	NJUST_MAP_TRAFFIC_LIGHTS_POSITION_TOTAL_NUM           //结尾
};

//红绿灯类型
enum   NJUST_MAP_TRAFFIC_LIGHTS_TYPE
{
    NJUST_MAP_TRAFFIC_LIGHTS_TYPE_NONE = 0x00,         //没有红绿灯
    NJUST_MAP_TRAFFIC_LIGHTS_TYPE_LEFTSTRAIGHTRIGHT,   //左直右
    NJUST_MAP_TRAFFIC_LIGHTS_TYPE_LEFTSTRAIGHT,        //左直
    NJUST_MAP_TRAFFIC_LIGHTS_TYPE_RIGHTSTRAIGHT,       //右直
    NJUST_MAP_TRAFFIC_LIGHTS_TYPE_LEFTRIGHT,           //左右
    NJUST_MAP_TRAFFIC_LIGHTS_TYPE_STRAIGHT,           //直
	NJUST_MAP_TRAFFIC_LIGHTS_TYPE_TOTAL_NUM           //结尾
};

//道路结构体

enum NJUST_MAP_NODE_PROPRITY     //路点属性
{
	NJUST_MAP_START = 0x00, //起点
	NJUST_MAP_STRAIGHT,     //直行
	NJUST_MAP_LEFT,         //左拐
	NJUST_MAP_RIGHT,        //右拐
	NJUST_MAP_Uturn,        //Uturn
	NJUST_MAP_TRAFICSIGN,   //交通标志
	NJUST_MAP_NODE_PROPRITY_TOTAL_NUM           //结尾
};

enum NJUST_MAP_ROAD_STRUCTURE    //道路结构
{
	NJUST_MAP_STRUCTURED_ROAD=0X00, //结构化道路
	NJUST_MAP_HALF_STRUCTURED_ROAD,  //半结构化道路
	NJUST_MAP_NON_STRUCTURED_ROAD,   //非结构化道路
	NJUST_MAP_ROAD_STRUCTURE_TOTAL_NUM           //结尾
};

enum NJUST_MAP_GPS_SOURCE      //GPS信息来源
{
	NJUST_MAP_GPS_FROM_CAR=0X00,    //GPS序列是车行驶采集的
	NJUST_MAP_GPS_FROM_HAND_DEVICE, //GPS序列是手持GPS设备采集的
	NJUST_MAP_GPS_FROM_HAND_DRAW,   //GPS序列是人工绘制编辑的
	NJUST_MAP_GPS_SOURCE_TOTAL_NUM           //结尾
};


typedef struct gpspoint
{
	double longtitude;
	double latitude;
}NJUST_MAP_GPSPoint;

//TODO 障碍物描述
typedef struct obstacle
{
	NJUST_MAP_GPSPoint    ObstacleCenterGPS; //障碍物中心位置的经纬度
	int             RadialCM;            //半径，以圆来描述,单位:cm
}NJUST_MAP_OBSTACLE;



struct NJUST_MAP_INFO_ROAD
{
	NJUST_IP_TIME                     synTime;                             //时间戳
	int                               frameID;                             //帧号
    int                               roadNum;                             //道路编号
	NJUST_MAP_ROAD_TYPE               roadType;                            //道路类型
	NJUST_MAP_ROAD_BOUNDARY_TYPE      leftRoadBoundaryType;                //左边界线类型
	NJUST_MAP_ROAD_BOUNDARY_TYPE      rightRoadBoundaryType;               //右边界线类型
	int                               roadWidthCm;                         //道路宽度,单位：cm
	int                               curbWidthCm;                         //马路崖子宽度,单位：cm (0: 无马路崖子)
	NJUST_MAP_LANE_LINE_TYPE          leftLaneLineType;                    //车道左边沿行道线类型
 	NJUST_MAP_LANE_LINE_TYPE          centerLaneLineType;                  //车道中间行道线类型
	NJUST_MAP_LANE_LINE_TYPE          rightLaneLineType;                   //车道右边沿行道线类型
	int                               nLaneNum;                            //车道数量
	int                               idealSpeedKMH;                      //建议车速 单位: km/h
	int                               distToNextNodeM;                    //距离下一路口距离 单位: m
	NJUST_MAP_GPSPoint                nextGPSPointQueue[NJUST_MAP_GPS_POINT];//下20个点序列
	int                               GPSPointQueuelength;                      //点序列中的有效点的个数
	NJUST_MAP_GPSPoint                lastNodeGps;                         //上一个路口中心位置的GPS经纬度
	NJUST_MAP_GPSPoint                nextNodeGps;                         //下一个路口中心位置的GPS经纬度
	NJUST_MAP_ROAD_STRUCTURE          structure;                           //结构化属性
	NJUST_MAP_GPS_SOURCE              GPSDataFrom;                         //GPS信息来源
	NJUST_MAP_OBSTACLE                mapObstacleQueue[NJUST_MAP_OBSTACLE_POINT];  //  检测半径内的障碍物序列                     //障碍物描述
	bool                              obstacleQueueLength;                      //障碍物个数
	int                               nSize;                               //该结构体的大小
	unsigned char                     checksum;                            //检查和:以上数据之和
};



//交叉路口结构体
struct NJUST_MAP_INFO_NODE
{
	NJUST_IP_TIME                      synTime;                  //时间戳
	int                                frameID;                  //帧号
    int                                nodeNum;                 //节点编号
	int                                xCM;                    //节点中心点大地坐标x,单位:cm
	int                                yCM;                    //节点中心点大地坐标y,单位:cm
	NJUST_MAP_NODE_TYPE                nodeType;                //路口类型
	NJUST_MAP_NODE_PASS_TYPE           nodepassType;            //通过路口方式
	NJUST_MAP_TRAFFIC_LIGHTS_POSITION  trafficLightsPosition;   //红绿灯位置
	NJUST_MAP_TRAFFIC_LIGHTS_TYPE      trafficLightsType;       //红绿灯类型
	int                                zebraCrossing;           //是否有斑马线
	NJUST_MAP_GPSPoint                 nextGPSPointQueue[NJUST_MAP_GPS_POINT];//下20个点序列
	int                                GPSPointQueuelength;     //点序列中的有效点的个数
	NJUST_MAP_GPS_SOURCE               GPSProprity;              //GPS序列的属性来源
	NJUST_MAP_OBSTACLE                 mapObstacleQueue[NJUST_MAP_OBSTACLE_POINT];  //  检测半径内的障碍物序列                     //障碍物描述
	bool                               obstacleQueueLength;                      //障碍物个数
	int                                nSize;                   //该结构体的大小
	unsigned char                      checksum;                //检查和:以上数据之和
};

struct NJUST_MAP_INFO_DIRECTION
{
	NJUST_IP_TIME                      synTime;                  //时间戳
	int                                frameID;                   //帧号
	NJUST_MAP_NODE_PROPRITY            nodeproprity;            //路点属性
	int                                nSize;                   //该结构体的大小
	unsigned char                      checksum;                //检查和:以上数据之和
};

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  恢复对齐方式
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//恢复对齐状态

#endif
