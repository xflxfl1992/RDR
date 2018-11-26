#ifndef XFL_LIDAR64_TOOLS
#define XFL_LIDAR64_TOOLS

#include "NJUST_ALV_BYD_H/ALV_BYD.h"
//���ڳ�ʼ������
void Initialize_ImgInput(bool* isForward,
						 //դ��
	                     int* gridXcm, //դ��ˮƽ����ľ���,��λcm
                         int* gridYcm, //դ��ֱ����ľ���,��λcm
		                 int* widthCM, //դ��ͼ��Ŀ��,��λcm
		                 int* beginGridDistCM, //դ��ͼ�����ʼ����,��λcm
		                 int* endGridDistCM, //դ��ͼ�����ֹ����,��λcm
						 //��·�߽�
						 int* beginBoundaryDistCM, //�߽�դ��ͼ�����ʼ����,��λcm
		                 int* endBoundaryDistCM, //�߽�դ��ͼ�����ֹ����,��λcm
						 int* minRoadWidthCM, //��·����С���
					     int* maxRoadWidthCM, //��·�������
						 //���ϰ�
                         int* minObstacleWCM, //���ϰ�����С���
		                 int* minObstacleHCM,  //���ϰ�����С����
						 //���ϰ�
						 int* minNegativeObstacleWCM, //���ϰ�����С���
		                 int* maxNegativeObstacleWCM,  //���ϰ��������
						 //��·�ڵĴ�������
						 int* beginObstacleDistCM, //դ��ͼ�����ʼ����,��λcm
						 int* endObstacleDistCM //դ��ͼ�����ֹ����,��λcm
						 );

//���ڸ���ֱ�߲��
void InsertPoint(int step_y,//����
 				 int N,//��໭N����
 				 double A,double B,double C,//ֱ�߷��̲���
 				 NJUST_3D64_ROAD_BOUNDARY_TO_FU * p3D64Boundary,
 				 bool isLeft,
 				 int gridXcm,
 				 int gridYcm,
 				 int beginDistCM
 				 );

#endif