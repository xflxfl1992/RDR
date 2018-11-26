////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, �Ͼ�����ѧ�������ѧ�빤��ѧԺ, ���ܿ�ѧ�뼼��ϵ
//  FileName:  NJUST_2D_data.h
//  Author: ����
//  Date:   2015.5.14
//  Description: 2Dģ������ݶ���
//  Modification:
//          2015.6.26, ������
//  Modification:
//          2015.8.5, ����
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
//  ���ö��뷽ʽ
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push) //�������״̬
#pragma pack(1)    //��1�ֽڶ���,������ڵ�һ��

//ϵͳ����
#define NJUST_2D_MAX_CAMERA_NUM             8       //���8�����,���[0..7]
#define NJUST_2D_MAX_NATURE_BOUNDARY_NUM    (2*3)   //��Ȼ������2����ѵ���,4����ѡ����
#define NJUST_2D_MAX_LANE_MAKDERS_NUM       6       //������6������,6������

//ͼ������
#define NJUST_2D_MAX_FILENAME_LEN           255     //�ļ�������󳤶�
#define NJUST_2D_MAX_IP_IMG_WIDTH           1024    //IP�����ͼ�����ݵ������
#define NJUST_2D_MAX_IP_IMG_HEIGHT          1024    //IP�����ͼ�����ݵ����߶�
#define NJUST_2D_MAX_IP_IMG_SIZE           (NJUST_2D_MAX_IP_IMG_WIDTH*NJUST_2D_MAX_IP_IMG_HEIGHT*3)  //IP�����ͼ�����ݵ�����ֽڸ���

//���ݶ���
#define NJUST_2D_MAX_POINT_NUM              2048   //������2048����
#define NJUST_2D_MAX_DBG_INFO_LEN           1024   //������Ϣ���1024���ַ�

//��������
enum NJUST_2D_OBJECT_TYPE
{
   NJUST_2D_BOUNDARY_LEFT_BEST = 0x00,      //��Ȼ���ߵ������߽�
   NJUST_2D_BOUNDARY_LEFT_1_MAYBE,          //��Ȼ���ߵĿ�����߽�1
   NJUST_2D_BOUNDARY_LEFT_2_MAYBE,          //��Ȼ���ߵĿ�����߽�2
   NJUST_2D_BOUNDARY_RIGHT_BEST,            //��Ȼ���ߵ�����ұ߽�
   NJUST_2D_BOUNDARY_RIGHT_1_MAYBE,         //��Ȼ���ߵĿ����ұ߽�1
   NJUST_2D_BOUNDARY_RIGHT_2_MAYBE,         //��Ȼ���ߵĿ����ұ߽�2
   NJUST_2D_LANE_MARKER_1_WHITE,            //��1������
   NJUST_2D_LANE_MARKER_2_WHITE,            //��2������
   NJUST_2D_LANE_MARKER_3_WHITE,            //��3������
   NJUST_2D_LANE_MARKER_4_WHITE,            //��4������
   NJUST_2D_LANE_MARKER_5_WHITE,            //��5������
   NJUST_2D_LANE_MARKER_6_WHITE,            //��6������
   NJUST_2D_LANE_MARKER_1_YELLOW,           //��1������
   NJUST_2D_LANE_MARKER_2_YELLOW,           //��2������
   NJUST_2D_LANE_MARKER_3_YELLOW,           //��3������
   NJUST_2D_LANE_MARKER_4_YELLOW,           //��4������
   NJUST_2D_LANE_MARKER_5_YELLOW,           //��5������
   NJUST_2D_LANE_MARKER_6_YELLOW,           //��6������
   NJUST_2D_STOP_LINE,                      //����ֹͣ��
   NJUST_2D_ZEBRA_CROSSING,                 //���а���Ϸ
   NJUST_2D_SUSPICIOUS_OBJECT_WATER,        //������ˮ��
   NJUST_2D_SUSPICIOUS_OBJECT_VEHICLE,      //�����ǳ���
   NJUST_2D_SUSPICIOUS_OBJECT_PEDESTRIAN,   //����������
   NJUST_2D_OBJECT_TYPE_TOTAL_NUM	          //���������
};

typedef struct tagNJUST_2D_POINT
{
    int  x_cm;     //���X����,��������
    int  y_cm;     //���Y����,��������
	NJUST_2D_OBJECT_TYPE type; //�������
}NJUST_2D_POINT;

//2D���ں�ģ�������
typedef struct tagNJUST_2D_RESULT
{
	int camerID; //�������ID��(��0��ʼ)
	int frameID;   //ͼ���֡��(��0��ʼ)
	NJUST_IP_TIME synTime; //ͼ���ȡ��ʱ��
  int navID;  //��ͼ���ȡʱ����ӽ��ĵ������ݱ��(��0��ʼ)
  int width; //ͼ��Ŀ��
	int height; //ͼ��ĸ߶�
	NJUST_2D_POINT pPoint[NJUST_2D_MAX_POINT_NUM];  //��������
	int nPoint; //��Ч��ĸ���
	int nSize; //�ýṹ��Ĵ�С
	unsigned char checksum;  //����:��������֮��
}NJUST_2D_RES_TO_FU;

//2D���Կ��ն˵ĵ�����Ϣ
typedef struct NJUST_2D_DBG_INFO
{
	int camerID; //�������ID��(��0��ʼ)
	int frameID; //ͼ��֡��(��0��ʼ)
	NJUST_IP_TIME synTime;   //ϵͳʱ��
	char pInfo[NJUST_2D_MAX_DBG_INFO_LEN+1]; //������Ϣ
	int nLen;  //������Ϣ��ʵ���ַ�����(�������ַ�����β�ַ�0)
	int nSize; //�ýṹ��Ĵ�С
	unsigned char checksum;  //����:��������֮��
}NJUST_2D_INF_TO_MO;

//2D���Կ��ն˵�ͼ����Ϣ
typedef struct NJUST_2D_IMAGE
{
    char pFileName[NJUST_2D_MAX_FILENAME_LEN+1]; //�ļ���
	int frameID; //ͼ��֡��(��0��ʼ)
	int width; //ͼ��߶�
	int height; //ͼ����
    int nBits;  //1-��ֵͼ��; 8-�Ҷ�ͼ��; 24-��ɫͼ��
    unsigned char pData[NJUST_2D_MAX_IP_IMG_SIZE]; //ͼ������
	int nSize; //�ýṹ��Ĵ�С
	unsigned char checksum;  //����:��������֮��
}NJUST_2D_IMG_TO_MO;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  �ָ����뷽ʽ
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//�ָ�����״̬

#endif
