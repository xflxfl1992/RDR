////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, �Ͼ�����ѧ�������ѧ�빤��ѧԺ, ���ܿ�ѧ�뼼��ϵ
//  FileName:  NJUST_IR_data.h
//  Author: ������
//  Date:   2017.7.15
//  Description: ���������ǵ����ݴ���ģ��
//  Ŀǰ�����
//          ��Ȼ����,ˮ��
//  Modification: 
//          2016.7.15, ������
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
//  ���ö��뷽ʽ
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push) //�������״̬
#pragma pack(1)    //��1�ֽڶ���,������ڵ�һ��

//ϵͳ����
#define NJUST_IR_MAX_NATURE_BOUNDARY_NUM    (2+4)   //��Ȼ������2����ѵ���,4����ѡ����

//ͼ������
#define NJUST_IR_MAX_FILENAME_LEN           255     //�ļ�������󳤶�
#define NJUST_IR_MAX_IP_IMG_WIDTH           1024    //IP�����ͼ�����ݵ������
#define NJUST_IR_MAX_IP_IMG_HEIGHT          1024    //IP�����ͼ�����ݵ����߶�
#define NJUST_IR_MAX_IP_IMG_SIZE           (NJUST_IR_MAX_IP_IMG_WIDTH*NJUST_IR_MAX_IP_IMG_HEIGHT)  //IP�����ͼ�����ݵ�����ֽڸ���

//���ݶ���
#define NJUST_IR_MAX_POINT_NUM              2048   //������2048����
#define NJUST_IR_MAX_DBG_INFO_LEN           1024   //������Ϣ���1024���ַ�

//��������
enum NJUST_IR_OBJECT_TYPE
{
   NJUST_IR_BOUNDARY_LEFT_BEST = 0x00,      //��Ȼ���ߵ������߽�
   NJUST_IR_BOUNDARY_LEFT_1_MAYBE,          //��Ȼ���ߵĿ�����߽�1
   NJUST_IR_BOUNDARY_LEFT_2_MAYBE,          //��Ȼ���ߵĿ�����߽�2
   NJUST_IR_BOUNDARY_RIGHT_BEST,            //��Ȼ���ߵ�����ұ߽�
   NJUST_IR_BOUNDARY_RIGHT_1_MAYBE,         //��Ȼ���ߵĿ����ұ߽�1
   NJUST_IR_BOUNDARY_RIGHT_2_MAYBE,         //��Ȼ���ߵĿ����ұ߽�2
   NJUST_IR_SUSPICIOUS_OBJECT_WATER,        //������ˮ��
   NJUST_IR_SUSPICIOUS_OBJECT_VEHICLE,      //�����ǳ���
   NJUST_IR_SUSPICIOUS_OBJECT_PEDESTRIAN,   //����������
   NJUST_IR_OBJECT_TYPE_TOTAL_NUM	        //���������
};

typedef struct tagNJUST_IR_POINT
{
    int  x_cm;     //���X����,��������
    int  y_cm;     //���Y����,��������
	NJUST_IR_OBJECT_TYPE type; //�������
	int objectID; //���������ͬһ��Ŀ��,����ˮ�ӵ�������Ҫ�����,��ͼ����Ҳ�����ж��ˮ��;��0����.
}NJUST_IR_POINT;	

//IR���ں�ģ�������
typedef struct tagNJUST_IR_RESULT
{
	int frameID;   //ͼ���֡��(��0��ʼ)
	NJUST_IP_TIME synTime; //ͼ���ȡ��ʱ��
    int navID;  //��ͼ���ȡʱ����ӽ��ĵ������ݱ��(��0��ʼ)
    int width; //ͼ��Ŀ��
	int height; //ͼ��ĸ߶�
	NJUST_IR_POINT pPoint[NJUST_IR_MAX_POINT_NUM];  //��������
	int nPoint; //��Ч��ĸ���
	int nSize; //�ýṹ��Ĵ�С
	unsigned char checksum;  //����:��������֮��
}NJUST_IR_RES_TO_FU;

//IR���Կ��ն˵ĵ�����Ϣ
typedef struct NJUST_IR_DBG_INFO
{
	int frameID; //ͼ��֡��(��0��ʼ)
	NJUST_IP_TIME synTime;   //ϵͳʱ��
	char pInfo[NJUST_IR_MAX_DBG_INFO_LEN+1]; //������Ϣ
	int nLen;  //������Ϣ��ʵ���ַ�����(�������ַ�����β�ַ�0)
	int nSize; //�ýṹ��Ĵ�С
	unsigned char checksum;  //����:��������֮��
}NJUST_IR_INF_TO_MO;

//IR���Կ��ն˵�ͼ����Ϣ
typedef struct NJUST_IR_IMAGE
{
    char pFileName[NJUST_IR_MAX_FILENAME_LEN+1]; //�ļ���
	int frameID; //ͼ��֡��(��0��ʼ)
	int width; //ͼ��߶�
	int height; //ͼ����
    int nBits;  //1-��ֵͼ��; 8-�Ҷ�ͼ��; 24-��ɫͼ��
    unsigned char pData[NJUST_IR_MAX_IP_IMG_SIZE]; //ͼ������ 
	int nSize; //�ýṹ��Ĵ�С
	unsigned char checksum;  //����:��������֮��
}NJUST_IR_IMG_TO_MO; 

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  �ָ����뷽ʽ
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//�ָ�����״̬

#endif
