
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, �Ͼ�����ѧ�������ѧ�빤��ѧԺ, ���ܿ�ѧ�뼼��ϵ
//  FileName:  NJUST_MAP_data.h
//  Author: ���Ʒ�
//  Date:   2015.6.6
//  Description: ��ͼ����
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
//  ���ö��뷽ʽ
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push) //�������״̬
#pragma pack(1)    //��1�ֽڶ���,������ڵ�һ��

//ȫ�ֱ���
#define NJUST_MAP_GPS_POINT    20
#define NJUST_MAP_OBSTACLE_POINT 20
#define NJUST_MAP_DETECT_OBSTACLE_RADIUS_M 100 //����ϰ���İ뾶(M)  
//��·����
enum  NJUST_MAP_ROAD_TYPE
{
    NJUST_MAP_ROAD_TYPE_NONE =0x00,         //δ֪
	NJUST_MAP_ROAD_TYPE_ASPHALT,            //����·
	NJUST_MAP_ROAD_TYPE_CEMENT,             //ˮ��·
	NJUST_MAP_ROAD_TYPE_DIRT,               //��·
	NJUST_MAP_ROAD_TYPE_COBBLED,            //ʯ��·
	NJUST_MAP_ROAD_TYPE_HIGHWAY,            //���ٹ�·
	NJUST_MAP_ROAD_TYPE_BRIDGE,             //����
	NJUST_MAP_ROAD_TYPE_TUNNEL,             //���
	NJUST_MAP_ROAD_TYPE_CULVERT,             //����
	NJUST_MAP_ROAD_TYPE_TOTAL_NUM           //��β
};

 //�е���
enum   NJUST_MAP_LANE_LINE_TYPE
{
    NJUST_MAP_LANE_LINE_TYPE_NONE = 0x00,              //���е���
    NJUST_MAP_LANE_LINE_TYPE_WHITEDOTTEDLINE,          //��ɫ����
    NJUST_MAP_LANE_LINE_TYPE_WHITESOLIDLINE,           //��ɫʵ��
    NJUST_MAP_LANE_LINE_TYPE_YELLOWDOTTEDLINE,         //��ɫ����
    NJUST_MAP_LANE_LINE_TYPE_YELLOWSOLIDLINE,          //��ɫʵ��
	NJUST_MAP_LANE_LINE_TYPE_TOTAL_NUM          //��β
};

//��·��߽�
enum   NJUST_MAP_ROAD_BOUNDARY_TYPE
{
    NJUST_MAP_ROAD_BOUNDARY_TYPE_NONE = 0x00,                    //δ֪
    NJUST_MAP_ROAD_BOUNDARY_TYPE_CONVEXBOUNDARY,                 //͹�߽�
    NJUST_MAP_ROAD_BOUNDARY_TYPE_CONCAVEBOUNDARY,                //���߽�
    NJUST_MAP_ROAD_BOUNDARY_TYPE_DANGEROUSBOUNDARY,             //Σ�ձ߽磨�����£�̽�ⲻ���ף�
	NJUST_MAP_ROAD_BOUNDARY_TYPE_TOTAL_NUM           //��β
};

//·������
enum   NJUST_MAP_NODE_TYPE
{
    NJUST_MAP_NODE_TYPE_NONE = 0x00,                    //δ֪
    NJUST_MAP_NODE_TYPE_CROSSROADS ,                    //ʮ��·��
    NJUST_MAP_NODE_TYPE_TNODE,                          //����·��
    NJUST_MAP_NODE_TYPE_YNODE,                          //Y·��
    NJUST_MAP_NODE_TYPE_RIGHTANGLENODE,                 //ֱ��·��
    NJUST_MAP_NODE_TYPE_STRAIGHTLINENODE ,              //ֱ��·��
	NJUST_MAP_NODE_TYPE_TOTAL_NUM           //��β
};

enum   NJUST_MAP_NODE_PASS_TYPE
{
    NJUST_MAP_NODE_PASS_TYPE_NONE = 0x00,                //δ֪
    NJUST_MAP_NODE_PASS_TYPE_STRAIGHTLINE,               //ֱ��ͨ��
    NJUST_MAP_NODE_PASS_TYPE_TURNLEFT,                   //��ת��
    NJUST_MAP_NODE_PASS_TYPE_TURNRIGHT,                  //��ת��
    NJUST_MAP_NODE_PASS_TYPE_TURNAROUND,                 //��ͷ
	NJUST_MAP_NODE_PASS_TYPE_TOTAL_NUM           //��β
};
//���̵�λ��
enum   NJUST_MAP_TRAFFIC_LIGHTS_POSITION
{
    NJUST_MAP_TRAFFIC_LIGHTS_POSITION_NONE = 0x00,            //δ֪
    NJUST_MAP_TRAFFIC_LIGHTS_POSITION_RIGHTFRONT,             //��ǰ
    NJUST_MAP_TRAFFIC_LIGHTS_POSITION_LEFTRONT,               //��ǰ
    NJUST_MAP_TRAFFIC_LIGHTS_POSITION_FRONT,                  //��ǰ
	NJUST_MAP_TRAFFIC_LIGHTS_POSITION_TOTAL_NUM           //��β
};

//���̵�����
enum   NJUST_MAP_TRAFFIC_LIGHTS_TYPE
{
    NJUST_MAP_TRAFFIC_LIGHTS_TYPE_NONE = 0x00,         //û�к��̵�
    NJUST_MAP_TRAFFIC_LIGHTS_TYPE_LEFTSTRAIGHTRIGHT,   //��ֱ��
    NJUST_MAP_TRAFFIC_LIGHTS_TYPE_LEFTSTRAIGHT,        //��ֱ
    NJUST_MAP_TRAFFIC_LIGHTS_TYPE_RIGHTSTRAIGHT,       //��ֱ
    NJUST_MAP_TRAFFIC_LIGHTS_TYPE_LEFTRIGHT,           //����
    NJUST_MAP_TRAFFIC_LIGHTS_TYPE_STRAIGHT,           //ֱ
	NJUST_MAP_TRAFFIC_LIGHTS_TYPE_TOTAL_NUM           //��β
};

//��·�ṹ��

enum NJUST_MAP_NODE_PROPRITY     //·������
{
	NJUST_MAP_START = 0x00, //���
	NJUST_MAP_STRAIGHT,     //ֱ��
	NJUST_MAP_LEFT,         //���
	NJUST_MAP_RIGHT,        //�ҹ�
	NJUST_MAP_Uturn,        //Uturn
	NJUST_MAP_TRAFICSIGN,   //��ͨ��־
	NJUST_MAP_NODE_PROPRITY_TOTAL_NUM           //��β
};

enum NJUST_MAP_ROAD_STRUCTURE    //��·�ṹ
{
	NJUST_MAP_STRUCTURED_ROAD=0X00, //�ṹ����·
	NJUST_MAP_HALF_STRUCTURED_ROAD,  //��ṹ����·
	NJUST_MAP_NON_STRUCTURED_ROAD,   //�ǽṹ����·
	NJUST_MAP_ROAD_STRUCTURE_TOTAL_NUM           //��β
};

enum NJUST_MAP_GPS_SOURCE      //GPS��Ϣ��Դ
{
	NJUST_MAP_GPS_FROM_CAR=0X00,    //GPS�����ǳ���ʻ�ɼ���
	NJUST_MAP_GPS_FROM_HAND_DEVICE, //GPS�������ֳ�GPS�豸�ɼ���
	NJUST_MAP_GPS_FROM_HAND_DRAW,   //GPS�������˹����Ʊ༭��
	NJUST_MAP_GPS_SOURCE_TOTAL_NUM           //��β
};


typedef struct gpspoint
{
	double longtitude;
	double latitude;
}NJUST_MAP_GPSPoint;

//TODO �ϰ�������
typedef struct obstacle
{
	NJUST_MAP_GPSPoint    ObstacleCenterGPS; //�ϰ�������λ�õľ�γ��
	int             RadialCM;            //�뾶����Բ������,��λ:cm
}NJUST_MAP_OBSTACLE;



struct NJUST_MAP_INFO_ROAD
{
	NJUST_IP_TIME                     synTime;                             //ʱ���
	int                               frameID;                             //֡��
    int                               roadNum;                             //��·���
	NJUST_MAP_ROAD_TYPE               roadType;                            //��·����
	NJUST_MAP_ROAD_BOUNDARY_TYPE      leftRoadBoundaryType;                //��߽�������
	NJUST_MAP_ROAD_BOUNDARY_TYPE      rightRoadBoundaryType;               //�ұ߽�������
	int                               roadWidthCm;                         //��·���,��λ��cm
	int                               curbWidthCm;                         //��·���ӿ��,��λ��cm (0: ����·����)
	NJUST_MAP_LANE_LINE_TYPE          leftLaneLineType;                    //����������е�������
 	NJUST_MAP_LANE_LINE_TYPE          centerLaneLineType;                  //�����м��е�������
	NJUST_MAP_LANE_LINE_TYPE          rightLaneLineType;                   //�����ұ����е�������
	int                               nLaneNum;                            //��������
	int                               idealSpeedKMH;                      //���鳵�� ��λ: km/h
	int                               distToNextNodeM;                    //������һ·�ھ��� ��λ: m
	NJUST_MAP_GPSPoint                nextGPSPointQueue[NJUST_MAP_GPS_POINT];//��20��������
	int                               GPSPointQueuelength;                      //�������е���Ч��ĸ���
	NJUST_MAP_GPSPoint                lastNodeGps;                         //��һ��·������λ�õ�GPS��γ��
	NJUST_MAP_GPSPoint                nextNodeGps;                         //��һ��·������λ�õ�GPS��γ��
	NJUST_MAP_ROAD_STRUCTURE          structure;                           //�ṹ������
	NJUST_MAP_GPS_SOURCE              GPSDataFrom;                         //GPS��Ϣ��Դ
	NJUST_MAP_OBSTACLE                mapObstacleQueue[NJUST_MAP_OBSTACLE_POINT];  //  ���뾶�ڵ��ϰ�������                     //�ϰ�������
	bool                              obstacleQueueLength;                      //�ϰ������
	int                               nSize;                               //�ýṹ��Ĵ�С
	unsigned char                     checksum;                            //����:��������֮��
};



//����·�ڽṹ��
struct NJUST_MAP_INFO_NODE
{
	NJUST_IP_TIME                      synTime;                  //ʱ���
	int                                frameID;                  //֡��
    int                                nodeNum;                 //�ڵ���
	int                                xCM;                    //�ڵ����ĵ�������x,��λ:cm
	int                                yCM;                    //�ڵ����ĵ�������y,��λ:cm
	NJUST_MAP_NODE_TYPE                nodeType;                //·������
	NJUST_MAP_NODE_PASS_TYPE           nodepassType;            //ͨ��·�ڷ�ʽ
	NJUST_MAP_TRAFFIC_LIGHTS_POSITION  trafficLightsPosition;   //���̵�λ��
	NJUST_MAP_TRAFFIC_LIGHTS_TYPE      trafficLightsType;       //���̵�����
	int                                zebraCrossing;           //�Ƿ��а�����
	NJUST_MAP_GPSPoint                 nextGPSPointQueue[NJUST_MAP_GPS_POINT];//��20��������
	int                                GPSPointQueuelength;     //�������е���Ч��ĸ���
	NJUST_MAP_GPS_SOURCE               GPSProprity;              //GPS���е�������Դ
	NJUST_MAP_OBSTACLE                 mapObstacleQueue[NJUST_MAP_OBSTACLE_POINT];  //  ���뾶�ڵ��ϰ�������                     //�ϰ�������
	bool                               obstacleQueueLength;                      //�ϰ������
	int                                nSize;                   //�ýṹ��Ĵ�С
	unsigned char                      checksum;                //����:��������֮��
};

struct NJUST_MAP_INFO_DIRECTION
{
	NJUST_IP_TIME                      synTime;                  //ʱ���
	int                                frameID;                   //֡��
	NJUST_MAP_NODE_PROPRITY            nodeproprity;            //·������
	int                                nSize;                   //�ýṹ��Ĵ�С
	unsigned char                      checksum;                //����:��������֮��
};

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  �ָ����뷽ʽ
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//�ָ�����״̬

#endif
