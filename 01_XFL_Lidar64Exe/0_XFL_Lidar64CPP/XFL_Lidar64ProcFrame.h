#ifndef _XFL_LIDAR64_PROC_FRAME_H_
#define _XFL_LIDAR64_PROC_FRAME_H_

#include "NJUST_ALV_BYD_H/ALV_BYD.h"
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "./RmwLidarProcLib/RmwLidarProcLib.h"

#include "NJUST_3D64_Encode.h"
#include "XFL_Lidar64Def.h"

using namespace std;

class XFL_Lidar64ProcFrame
{

public:
	XFL_Lidar64ProcFrame();
	~XFL_Lidar64ProcFrame();
public:	
/////////////////////////////////////////////////////////////////////////////////////
// 
//   �״��㷨�����̣߳�ProcFrameThrd��
//
/////////////////////////////////////////////////////////////////////////////////////	
	//�㷨�����̵߳���ѭ��
	void ProcFrameLoop();	
	//��ʼ��
	bool Initialize(unsigned int *pNavID);
	//����һ֡դ������
	void ProcFrameLidar64Data();
	//ǰ��״̬�����㷨
	void XFLLidar_Forward();
	//����״̬�����㷨
	void XFLLidar_Backward();
	//�����ѹ�����벢����
	void CodeAndSend();	

public:
	unsigned int lid_pack_id;
	unsigned int *m_pNavID;
	unsigned int m_navID;
	NJUST_TO_MO_WORKSTAT workstat;
	NJUST_IP_TIME t1,t2;

	bool m_isForward;//flag,��ǰ��״̬���Ǻ���״̬
	bool m_isForward_StatusChange;//������ʻ��ʻ״̬�����ı��flag
	bool m_isObjDetect;//�Ƿ�������ģʽ,Ĭ��Ϊfalse
	bool m_UseNegativeObsDetect;//�Ƿ�Ҫʹ�ø��ϰ���Ĭ��Ϊtrue
	bool m_HasObstacle;//�ڼ�⸺�ϰ�ʱ�����ر��뺬�����ϰ��ı�ʶ
	int m_DataNum_InOneFrame;

	//���ò���
	//դ��
	int m_gridXcm ; //դ��ˮƽ����ľ���,��λcm
	int m_gridYcm ; //դ��ֱ����ľ���,��λcm
	int m_widthCM ; //դ��ͼ��Ŀ��,��λcm
	int m_beginGridDistCM; //դ��ͼ�����ʼ����,��λcm
	int m_endGridDistCM; //դ��ͼ�����ֹ����,��λcm
	//��·�߽�
	int m_beginBoundaryDistCM; 
	int m_endBoundaryDistCM; 
	int m_minRoadWidthCM; //��·����С���
	int m_maxRoadWidthCM; //��·�������
	bool m_isStructuredRoad; //�Ƿ��ǽṹ����·,·�߱�·�����(�вݻ���·��)�����Ե����ṹ����·
	//���ϰ�
	int m_minObstacleWCM; //���ϰ�����С���
	int m_minObstacleHCM; //���ϰ�����С����
	//���ϰ�
	int m_minNegativeObstacleWCM; //���ϰ�����С���
	int m_maxNegativeObstacleWCM; //���ϰ��������
	//��·�ڵĴ�������
	int m_beginObstacleDistCM; //դ��ͼ�����ʼ����,��λcm
	int m_endObstacleDistCM ; //դ��ͼ�����ֹ����,��λcm

	//��Ҫ���͵Ľṹ��
	NJUST_3D64_OBS_TO_FU m_3D64Obs;
	NJUST_3D64_ROAD_BOUNDARY_TO_FU m_3D64Boundary;
	NJUST_3D64_GRID_TO_FU m_3D64Grid;
	NJUST_3D64_CROSS_TO_FU m_3D64Cross;
	NJUST_3D64_TARGET_TO_2D m_3D64TargetDetect;

	//�ϰ���ӳ�����
	int m_num[NJUST_LIDAR64_VERTICAL_GRID_NUM*NJUST_LIDAR64_HORIZONTAL_GRID_NUM];

	//�洢���ϰ��ṹ��
	RMW_LIDAR_3D_OBSTACLE m_NegativeRegions[MAX_NEGATIVE_OBASTACLE_NUM];

	//�洢�Ӻ�����ȡ�����ϰ���ͼ��ָ��
	BYTE *pObstacleTypeImg;


};


#endif
