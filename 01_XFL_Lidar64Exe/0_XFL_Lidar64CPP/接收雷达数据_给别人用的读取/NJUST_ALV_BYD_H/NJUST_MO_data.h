////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, �Ͼ�����ѧ�������ѧ�빤��ѧԺ, ���ܿ�ѧ�뼼��ϵ
//  FileName:  NJUST_MO_data.h
//  Author: ������
//  Date:   2015.6.6
//  Description: �Կ�ģ�������ģ���������״̬
//  Modification: 
//          2015.7.2, ������
//  Declare:
//          NJUST_FROM_MO_COMMAND
//          NJUST_TO_MO_WORKSTAT
//
//  �޸�
//  ���ڣ�  2016.5.5
//  ���ݣ�����߼����ƽṹ�壨CFG��
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_MO_DATA_H_
#define _NJUST_MO_DATA_H_

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
#define  NJUST_MO_MAX_COMMAND_PARA_LEN          32     //��������������
#define  NJUST_MO_WORK_STAT_ERRMSG_MAXLEN       32     //����״̬������Ϣ����󳤶�
#define  NJUST_MO_DEBUGMSG_MAXLEN              128     //������Ϣ����󳤶�

#define  NJUST_MO_MAX_CFG_NUM               32    //һ��ģ������߼�ָ�����
#define  NJUST_MO_MAX_CFG_PARA_LEN          32     //����߼������������
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ����: �Կط�����ģ��
//
////////////////////////////////////////////////////////////////////////////////////////////////
enum NJUST_MO_COMMAND_TYPE
{
	NJUST_MO_COMMAND_TYPE_NO = 0x00,                  //������
	NJUST_MO_COMMAND_TYPE_COMPUTER_RESTART,           //�������������
	NJUST_MO_COMMAND_TYPE_COMPUTER_SHUTDOWN,          //������ػ�
	NJUST_MO_COMMAND_TYPE_MODULE_RESTART,             //ģ����������
	NJUST_MO_COMMAND_TYPE_MODULE_DEBUG,               //ģ���л�������ģʽ
	NJUST_MO_COMMAND_TYPE_MODULE_RELEASE,             //ģ���л�������ģʽ
    NJUST_MO_COMMAND_TYPE_TOTAL_NUM	                 //�������������
};

enum NJUST_MO_MODULE_ID
{

};

typedef struct tagNJUST_FROM_MO_COMMAND
{   
	int frameID; //������(��0��ʼ)
	NJUST_IP_TIME synTime;  //ϵͳʱ��
    int moduleID;   //ģ��ID
    NJUST_MO_COMMAND_TYPE cmd; //����
    char pPara[NJUST_MO_MAX_COMMAND_PARA_LEN+1]; //�������
	int nPara; //��Ч�������ֽ���
	int nSize; //�ýṹ��Ĵ�С 
}NJUST_FROM_MO_COMMAND;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ģ�鹤��״̬: ��ģ�鷢���Կ�
//
////////////////////////////////////////////////////////////////////////////////////////////////
enum NJUST_MO_MODULE_STATUS
{ 
    NJUST_MO_WORK_STAT_INVAILD = 0x00, //ģ���޽�����
    NJUST_MO_WORK_STAT_VAILD,          //ģ�������Ч
    NJUST_MO_WORK_STAT_TOTAL_NUM	  //���״̬������
};
enum NJUST_MO_MODULE_ERRCODE
{ 
    NJUST_MO_ERRCODE_NOERR = 0x00,   //�޴���
    NJUST_MO_ERRCODE_ERROR,          //����
    NJUST_MO_ERRCODE_TOTAL_NUM	    //��������������
};

//��ģ����ԿصĹ���״̬����
typedef struct tagNJUST_TO_MO_WORKSTAT
{
	NJUST_IP_TIME synTime;  //ϵͳʱ��
    int moduleID;   //ģ��ID
	int myselfTimeOutMS; //ģ��ĳ�ʱ,��λ:ms;���ܿ��ڸ�ʱ����û���յ�״̬����,����Ϊ��ģ�鲻����
    NJUST_MO_MODULE_STATUS stat;    // ����״̬
	int PELR; //��ǧ�ֱȱ�ʾ�Ķ�����
	int timeConsumingMS; //��֡�������õ�ʱ��,��λms
	NJUST_MO_MODULE_ERRCODE errCode;     //������
    char pErrMsg[NJUST_MO_WORK_STAT_ERRMSG_MAXLEN+1];   //������Ϣ
}NJUST_TO_MO_WORKSTAT;


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  �߼��������ã��Կط�����ģ��
//
////////////////////////////////////////////////////////////////////////////////////////////////
enum NJUST_MO_CFG_TYPE
{
	NJUST_MO_CFG_TYPE_NO = 0x00,                 //������ 
	NJUST_MO_CFG_TYPE_USE_MAPINFO,               //ʹ��MAP��Ϣ
	NJUST_MO_CFG_TYPE_PL_ZEBRA_SLOW,             //PL�������к���߼���
	NJUST_MO_CFG_TYPE_MAP_SEND_MANMADE_OBS,      //MAP�����˹���ǵ��ϰ�
	NJUST_MO_CFG_TYPE_2D_3D_CONFIRM_MANMADE_OBS, //������֪�Ƿ��ٴα����˹����õ��˹��ϰ�
	NJUST_MO_CFG_TYPE_FU_OBS_REMAIN_TIME,        //FU���ϰ��ı���֡��
	NJUST_MO_CFG_TYPE_PL_FOLLOW_GPS,             //PL�Ƿ����GPS��
	NJUST_MO_CFG_TYPE_FU_USE_2DINF,              //FU�Ƿ�ʹ��2D��Ϣ
	NJUST_MO_CFG_TYPE_FU_USE_LSINF,              //FU�Ƿ�ʹ�ý�ͨ�ƣ���ͨ��־��Ϣ
	NJUST_MO_CFG_TYPE_FU_USE_3D64GRIDOBSINF,     //FU�Ƿ�ʹ��64��դ���ϰ�
	NJUST_MO_CFG_TYPE_FU_USE_3D64RECTOBSINF,     //FU�Ƿ�ʹ��64�߾����ϰ�
	NJUST_MO_CFG_TYPE_FU_USE_3D4INF,             //FU�Ƿ�ʹ��4���״�
	NJUST_MO_CFG_TYPE_FU_USE_3DESRINF,           //FU�Ƿ�ʹ�ú��ײ�
	NJUST_MO_CFG_TYPE_2D_ROADSTRPRO,             //2D��·�ṹ������
	NJUST_MO_CFG_TYPE_2D_STRAIGHT_DETOUR_BYROAD, //2Dѡ��ֱ·����·����·
	NJUST_MO_CFG_TYPE_2D_EXECUTE_ALGORITHM,      //2D�Ƿ�ִ���㷨
	NJUST_MO_CFG_TYPE_2D_IMAGE_SAVE_FRAME_RATE,  //2Dͼ�񱣴�֡��
	NJUST_MO_CFG_TYPE_TOTAL_NUM	  		         //�������������
};

typedef struct tagNJUST_MO_CFG
{
	NJUST_MO_CFG_TYPE cfg; //���õ�����
	int value; //���õ�ֵ,0-��Ч��1-��Ч,����-��ֵ
}NJUST_MO_CFG;

typedef struct tagNJUST_FROM_MO_CFG
{
	int frameID; //������(��0��ʼ)
	NJUST_IP_TIME synTime;  //ϵͳʱ��
	NJUST_MO_CFG pCFG[NJUST_MO_MAX_CFG_NUM]; //������
	int nCFG; //��Ч���õĸ���
	int nSize; //�ýṹ��Ĵ�С
	unsigned char checksum; //����:��������֮��
}NJUST_FROM_MO_CFG;

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  �ָ����뷽ʽ
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//�ָ�����״̬

#endif

