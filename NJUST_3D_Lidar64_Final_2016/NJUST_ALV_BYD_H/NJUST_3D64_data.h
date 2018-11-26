////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, �Ͼ�����ѧ�������ѧ�빤��ѧԺ, ���ܿ�ѧ�뼼��ϵ
//  FileName:  NJUST_3D64_data.h
//  Author: ���Ʒ�
//  Date:   2015.6.27
//  Description: 3Dģ������ݶ���
//
//  �޸ļ�¼
//  Date:   2016.5.5,
//          ����դ������
//  Date:   2016.7.15
//          ������
//          �����˲�·����,��NJUST_3D64_CROSS_TO_FU
//  Date:   2016.8.25
//          �����
//          �������ṹ��
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_3D64_DATA_H_
#define _NJUST_3D64_DATA_H_

#include "NJUST_Global_Def.h"
#include "NJUST_IP_comm.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  ���ö��뷽ʽ
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push) //�������״̬
#pragma pack(1)    //��1�ֽڶ���,������ڵ�һ��

////////////////////////////////////////////////////////////////////////////////////////////
//
//  ��������
//
////////////////////////////////////////////////////////////////////////////////////////////
#define NJUST_3D64_MAX_OBS_POINT_NUM               4             //һ���ϰ�������ö��ٵ��ʾ
#define NJUST_3D64_MAX_OBS_NUM		             150             //�ϰ����������
#define NJUST_3D64_MAX_ROAD_POINT_NUM	         200             //���ߵ�������Ŀ
#define MAX_OBJ_NUM								 8               //��������Ͱ��������
////////////////////////////////////////////////////////////////////////////////////////////
//
//  64���״�
//
////////////////////////////////////////////////////////////////////////////////////////////
//����Χ
#define NJUST_LIDAR64_VERTICAL_DISTANCE_CM       8000           //64���״�Ĵ�ֱ����8000cm,��ǰ6000cm,����2000cm            
#define NJUST_LIDAR64_HORIZONTAL_DISTANCE_CM     4000           //64���״��ˮƽ����4000cm,����2000cm,����2000cm            
//�ֱ���
#define NJUST_LIDAR64_VERTICAL_RESOLUTION_CM      20            //64���״�Ĵ�ֱ�ֱ���20cm
#define NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM    20            //64���״��ˮƽ�ֱ���20cm
//��ֱդ����200��,���뱣֤��8�ı���
#define NJUST_LIDAR64_VERTICAL_GRID_NUM          (NJUST_LIDAR64_VERTICAL_DISTANCE_CM/NJUST_LIDAR64_VERTICAL_RESOLUTION_CM)           
//ˮƽդ����400��,���뱣֤��8�ı���
#define NJUST_LIDAR64_HORIZONTAL_GRID_NUM        (NJUST_LIDAR64_HORIZONTAL_DISTANCE_CM/NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM)
////////////////////////////////////////////////////////////////////////////////////////////
//
//  ���Ͷ���
//
////////////////////////////////////////////////////////////////////////////////////////////
//��������  
enum  NJUST_3D64_ROAD_POINT_TYPE
{
	NJUST_3D64_ROAD_POINT_TYPE_LEFT = 0x00, //��߽�
	NJUST_3D64_ROAD_POINT_TYPE_RIGHT,       //�ұ߽�
   	NJUST_3D64_ROAD_POINT_TYPE_TOTAL_NUM    //�߽���������
};               

//�¶�����
enum  NJUST_3D64_ROAD_SLOPE_TYPE
{
	NJUST_3D64_ROAD_SLOPE_TYPE_FLAT = 0x00, //ƽ��
	NJUST_3D64_ROAD_SLOPE_TYPE_UP,          //����
	NJUST_3D64_ROAD_SLOPE_TYPE_DOWN,        //����
	NJUST_3D64_ROAD_SLOPE_TYPE_TOTAL_NUM	 //��������Ŀ
};

//3Dģ���еĵ㶨��
struct NJUST_3D64_POINT_2D
{
	int      x_cm;                               //x���굥λ cm
	int      y_cm;                               //y���굥λ cm
};
//����Ͱ�����߽磨դ������кţ�
struct NJUST_3D64_RECT_2D
{
	int      left_j;                             //����Ͱ�������߽磨դ����кţ�
	int      right_j;                            //����Ͱ������ұ߽磨դ����кţ�
	int      up_i;                               //����Ͱ������ϱ߽磨դ����кţ�
	int      low_i;                              //����Ͱ������±߽磨դ����кţ�
};

//�ϰ���
struct NJUST_3D64_OBS_DATA  
{
	unsigned int         OBSID;                                  //�ϰ���ID
	NJUST_3D64_POINT_2D  pPoint[NJUST_3D64_MAX_OBS_POINT_NUM];   //Ŀ���ϰ��������һ���ɼ��㣬����Ϊ��ʱ����ת���������ǵ㡣
	int                  nPoint;                                 //�ϰ�����Ч����
	int                  z_cm;                                   //��ͨ���ϰ�����=0����������=1��
    int                  fbSpeed_cmps;                           //�ϰ����ǰ���ٶ��ϵķ���,��λ:����/��
	int                  lrSpeed_cmps;                           //�ϰ�������ҷ����ϵ��ٶȷ���,��λ:����/��
};
////////////////////////////////////////////////////////////////////////////////////////////
//
//  3D���ں�ģ����ϰ�����
//
////////////////////////////////////////////////////////////////////////////////////////////
typedef struct NJUST_3D64_OBS                              
{
	int                    frameID;                               //֡ID(��0��ʼ)
	NJUST_IP_TIME          synTime;                               //ʱ���
	unsigned int           navID;                                 //��ͼ���ȡʱ����ӽ��ĵ������ݱ��(��0��ʼ)
	NJUST_3D64_OBS_DATA    pObj[NJUST_3D64_MAX_OBS_NUM];          //�ϰ�������
	int                    nObj;                                  //�ϰ�����Ч����
	int                    nSize;                                 //�ýṹ��Ĵ�С
	unsigned char		   checksum;                              //����:��������֮��
}NJUST_3D64_OBS_TO_FU;
////////////////////////////////////////////////////////////////////////////////////////////
//
//  3D���ں�ģ��ĵ�·��Ե
//
////////////////////////////////////////////////////////////////////////////////////////////
typedef struct NJUST_3D64_ROAD_BOUNDARY 
{
	int                          frameID;                            //֡ID(��0��ʼ)
	NJUST_IP_TIME                synTime;                            //ʱ���
	unsigned int                 navID;                              //��ͼ���ȡʱ����ӽ��ĵ������ݱ��(��0��ʼ)
	NJUST_3D64_POINT_2D          Point[NJUST_3D64_MAX_ROAD_POINT_NUM]; //���ߵ�����
	int                          nPoint;                             //���ߵ���Ч����
	NJUST_3D64_ROAD_POINT_TYPE   Type[NJUST_3D64_MAX_ROAD_POINT_NUM];  //���ߵ����� ��ߡ��ұߣ����·������һһ��Ӧ
	int                          nSize;                              //�ýṹ��Ĵ�С
	unsigned char                checksum;                           //����:��������֮��
}NJUST_3D64_ROAD_BOUNDARY_TO_FU;
////////////////////////////////////////////////////////////////////////////////////////////
//
//  3D���ں�ģ��ĵ�·�����¶�����
//
////////////////////////////////////////////////////////////////////////////////////////////
typedef struct NJUST_3D64_ROAD_SLOPE
{
	int                                frameID;                  //֡ID(��0��ʼ)
	NJUST_IP_TIME                      synTime;                  //ʱ���
	unsigned int                       navID;                    //��ͼ���ȡʱ����ӽ��ĵ������ݱ��(��0��ʼ)
	NJUST_3D64_ROAD_SLOPE_TYPE    	   type;                     //�¶����ͣ�ƽ�ء����¡�����
	int                                degree;                   //�¶ȣ���λ: ��
	int                                nSize;                    //�ýṹ��Ĵ�С
	unsigned char                      checksum;                 //����:��������֮��
}NJUST_3D64_ROAD_SLOPE_TO_FU;
////////////////////////////////////////////////////////////////////////////////////////////
//
//  3D���ں�ģ�����������
//
////////////////////////////////////////////////////////////////////////////////////////////
//դ������
enum NJUST_3D64_GRID_TYPE
{
	NJUST_3D64_GRID_TYPE_NORMAL = 0x00, //ƽ��
	NJUST_3D64_GRID_TYPE_OBS,           //���ϰ�
	NJUST_3D64_GRID_TYPE_NEGTIVE_OBS,   //���ϰ�
	NJUST_3D64_GRID_TYPE_WATER,         //ˮ
	NJUST_3D64_GRID_TYPE_OBJ_DETECTED,  //���Ŀ��
	NJUST_3D64_GRID_TYPE_HIGH_OBS,		//��Ŀ��
	NJUST_3D64_GRID_TYPE_TOTAL_NUM      //��������,��಻�ܳ���256��.
};
typedef struct tagNJUST_3D64_GRID_TO_FU  
{
	int                    frameID;                               //֡ID(��0��ʼ)
	NJUST_IP_TIME          synTime;                               //ʱ���
	unsigned int           navID;                                 //��ͼ���ȡʱ����ӽ��ĵ������ݱ��(��0��ʼ)
	BYTE                   gridMsk[NJUST_LIDAR64_VERTICAL_GRID_NUM*NJUST_LIDAR64_HORIZONTAL_GRID_NUM];  //��200*��400
}NJUST_3D64_GRID_TO_FU;

////////////////////////////////////////////////////////////////////////////////////////////
//
//  3D���ں�ģ��Ĳ�·����
//
////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_3D64_CROSS_TO_FU  
{
	int                    frameID;                               //֡ID(��0��ʼ)
	NJUST_IP_TIME          synTime;                               //ʱ���
	unsigned int           navID;                                 //��ͼ���ȡʱ����ӽ��ĵ������ݱ��(��0��ʼ)
	double                 A,B,C;                                 //��·�����ߵ�ֱ�߷���
	BYTE                   crossMsk[NJUST_LIDAR64_VERTICAL_GRID_NUM*NJUST_LIDAR64_HORIZONTAL_GRID_NUM];  //��200*��400
}NJUST_3D64_CROSS_TO_FU;
////////////////////////////////////////////////////////////////////////////////////////////
//
//  3D��2Dģ����������
//
////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_3D64_TARGET_TO_2D
{
	int				     frameID;               						//֡ID
	NJUST_IP_TIME        synTime;               						//ʱ���
	unsigned int		 navID;           						        //�ߵ�
	NJUST_3D64_POINT_2D  obj_earth_coord[MAX_OBJ_NUM];	                //ÿ��Ŀ��ľ��Դ������ϵ
	NJUST_3D64_RECT_2D   obj_grid_coord[MAX_OBJ_NUM];		            //ÿ��Ŀ�����Ӿ���
	int				     objNum;                						//Ŀ���ʵ��(��������Ͱ)�ĸ���
	int                  nSize;                                         //�ýṹ��Ĵ�С
	unsigned char        checksum;                                      //����:��������֮��
}NJUST_3D64_TARGET_TO_2D;

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  �ָ����뷽ʽ
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//�ָ�����״̬

#endif
