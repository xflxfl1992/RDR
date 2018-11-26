////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, �Ͼ�����ѧ�������ѧ�빤��ѧԺ, ���ܿ�ѧ�뼼��ϵ
//  FileName:  NJUST_LS_data.h
//  Author: ������
//  Date:   2015.6.6
//  Description: ��֪�ƹ�ģ��ͽ�ͨ��ʶģ��,���ں�ģ��������
//  Modification: 
//          2015.6.26, ������
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
//  ���ö��뷽ʽ
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push) //�������״̬
#pragma pack(1)    //��1�ֽڶ���,������ڵ�һ��

//ϵͳ����
#define NJUST_LS_MAX_TRAFFIC_SIGN_NUM         12    //һ��ͼ��������12����ͨ��ʶ
#define NJUST_LS_MAX_TRAFFIC_LIGHT_NUM        12    //һ��ͼ��������12����ͨ��
#define NJUST_LS_MAX_DBG_INFO_LEN           1024    //������Ϣ���1024���ַ�

//ͼ������
#define NJUST_LS_MAX_FILENAME_LEN           255     //�ļ�������󳤶�
#define NJUST_LS_MAX_IP_IMG_WIDTH           1024    //IP�����ͼ�����ݵ������
#define NJUST_LS_MAX_IP_IMG_HEIGHT          1024    //IP�����ͼ�����ݵ����߶�
#define NJUST_LS_MAX_IP_IMG_SIZE           (NJUST_LS_MAX_IP_IMG_WIDTH*NJUST_LS_MAX_IP_IMG_HEIGHT*3)  //IP�����ͼ�����ݵ�����ֽڸ���

//�������Ͷ���
typedef struct tagNJUST_LS_RECT
{
    int left; //ͼ������
	int right; //ͼ������ 
    int top; //ͼ������
	int bottom; //ͼ������
}NJUST_LS_RECT;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ��ͨ��ʶ
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
    int signType;  //��ʶ����
    int value;     //��ֵ
    int reliability;  //���Ŷ�[0-100].
    NJUST_LS_RECT pos;  //��ͼ���е�λ��
}NJUST_LS_TRAFFIC_SIGN;

//��ͨ��ʶ���ں�ģ�������
typedef struct NJUST_LS_SIGN_RESULT
{
	int frameID;   //ͼ���֡��(��0��ʼ)
	NJUST_IP_TIME synTime; //ͼ���ȡ��ʱ��
    int navID;  //��ͼ���ȡʱ����ӽ��ĵ������ݱ��(��0��ʼ)
    NJUST_LS_TRAFFIC_SIGN pSign[NJUST_LS_MAX_TRAFFIC_SIGN_NUM]; //����ʶ
    int nSign; //��Ч��ʶ�ĸ���
	int nSize; //�ýṹ��Ĵ�С
	unsigned char checksum; //����:��������֮��
}NJUST_LS_SIGN_RES_TO_FU;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ��ͨ������
//
////////////////////////////////////////////////////////////////////////////////////////////////
//�Ƶ���ɫ
enum NJUST_LS_LIGHT_COLOR
{
	NJUST_LS_LIGHT_COLOR_UNKNOWN = 0x00,   //δ֪
	NJUST_LS_LIGHT_COLOR_RED,			   //���
	NJUST_LS_LIGHT_COLOR_GREEN,			   //�̵�
	NJUST_LS_LIGHT_COLOR_YELLOW,		   //�Ƶ�
	NJUST_LS_LIGHT_COLOR_TOTAL_NUM         //�ܵ�ɫ��������
};

//�ƿ�����
enum NJUST_LS_LIGHT_TYPE
{
    NJUST_LS_LIGHT_TYPE_UNKNOWN = 0x00,    //δ֪
    NJUST_LS_LIGHT_TYPE_ROUND,              //Բ��
    NJUST_LS_LIGHT_TYPE_LEFT,               //���
    NJUST_LS_LIGHT_TYPE_RIGHT,              //�ҹ�
    NJUST_LS_LIGHT_TYPE_FORWARD,            //ǰ��
    NJUST_LS_LIGHT_TYPE_BACKWARD,           //��ͷ
	NJUST_LS_LIGHT_TYPE_TOTAL_NUM          //�ܵ�����������
};

//�Ƶ�����
typedef struct tagNJUST_LS_TRAFIIC_LIGHT
{
    NJUST_LS_LIGHT_COLOR color;     //�Ƶ���ɫ
    NJUST_LS_LIGHT_TYPE type;   //�Ƶ�����
    int reliability;  //���Ŷ�[0-100].
    NJUST_LS_RECT pos;  //��ͼ���е�λ��
}NJUST_LS_TRAFIIC_LIGHT;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ͨ��״̬
//
////////////////////////////////////////////////////////////////////////////////////////////////
//��Ϊ
enum NJUST_LS_LIGHT_MOVING_DIR
{
    NJUST_LS_LIGHT_MOVING_DIR_UNKNOWN = 0x00,  //δ֪
    NJUST_LS_LIGHT_MOVING_DIR_ROUND,            //������
    NJUST_LS_LIGHT_MOVING_DIR_LEFT,             //���
    NJUST_LS_LIGHT_MOVING_DIR_RIGHT,            //�ҹ�
    NJUST_LS_LIGHT_MOVING_DIR_FORWARD,          //ǰ��
    NJUST_LS_LIGHT_MOVING_DIR_BACKWARD,         //��ͷ
	NJUST_LS_LIGHT_MOVING_DIR_TOTAL_NUM        //�ܵ��˶����������
};
//����
enum NJUST_LS_LIGHT_PASSING_STATUS
{
    NJUST_LS_LIGHT_PASS_UNKNOWN = 0x00,         //δ֪
    NJUST_LS_LIGHT_PASS_PASSABLE,               //��ͨ��
    NJUST_LS_LIGHT_PASS_PASSABLE_COUNTDOWN,     //ͨ�е���ʱ
    NJUST_LS_LIGHT_PASS_IMPASSABLE,             //����ͨ��
    NJUST_LS_LIGHT_PASS_IMPASSABLE_COUNTDOWN,   //����ͨ�е���ʱ
};
//ͨ��״̬
typedef struct tagNJUST_LS_LIGHT_MOVING
{  
	NJUST_LS_LIGHT_MOVING_DIR dir; //�˶����� 
	NJUST_LS_LIGHT_PASSING_STATUS status; //ͨ��״̬
    int nCountDownSecond;   //����ʱ����
}NJUST_LS_LIGHT_MOVING;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ��ͨ�Ƹ��ں�ģ�������
//
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_LS_LIGHT_RESULT
{
	int frameID; //ͼ��֡��(��0��ʼ)
	NJUST_IP_TIME synTime;   //ϵͳʱ��
	int navID;  //��ͼ���ȡʱ����ӽ��ĵ������ݱ��(��0��ʼ)
    int width; //ͼ��Ŀ��
	int height; //ͼ��ĸ߶�
    //��ͨ��
	NJUST_LS_TRAFIIC_LIGHT pLight[NJUST_LS_MAX_TRAFFIC_LIGHT_NUM]; //������
	int nLight; //��Ч�ĵƵĸ���
	//ͨ��״̬
	NJUST_LS_LIGHT_MOVING pMoving[NJUST_LS_MAX_TRAFFIC_LIGHT_NUM]; //��״̬
	int nMoving;
	//�ýṹ��Ĵ�С 
	int nSize;
	//����:��������֮��
	unsigned char checksum;
}NJUST_LS_LIGHT_RES_TO_FU;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  LS���Կ�ģ�������
//
////////////////////////////////////////////////////////////////////////////////////////////////
//LS���Կ��ն˵ĵ�����Ϣ
typedef struct tagNJUST_LS_DBG_INFO
{
	int camerID; //�������ID��(��0��ʼ)
	int frameID; //ͼ��֡��(��0��ʼ)
	NJUST_IP_TIME synTime;   //ϵͳʱ��
    int width; //ͼ��Ŀ��
	int height; //ͼ��ĸ߶�
	char pInfo[NJUST_LS_MAX_DBG_INFO_LEN+1]; //������Ϣ
	int nLen;  //������Ϣ��ʵ���ַ�����(�������ַ�����β�ַ�0)
	int nSize; //�ýṹ��Ĵ�С 
}NJUST_LS_INF_TO_MO;

//LS���Կ��ն˵�ͼ����Ϣ
typedef struct tagNJUST_LS_IMAGE
{
    char pFileName[NJUST_LS_MAX_FILENAME_LEN+1]; //�ļ���
    int width; //ͼ��߶�
	int height; //ͼ����
    int nBits;  //1-��ֵͼ��; 8-�Ҷ�ͼ��; 24-��ɫͼ��
    unsigned char *pData; //ͼ������ 
	int nSize; //�ýṹ��Ĵ�С 
	unsigned char checksum; //����:��������֮��
}NJUST_LS_IMG_TO_MO; 

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  �ָ����뷽ʽ
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//�ָ�����״̬

#endif
