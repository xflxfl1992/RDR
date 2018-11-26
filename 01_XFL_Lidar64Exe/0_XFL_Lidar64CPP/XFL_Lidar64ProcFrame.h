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
//   雷达算法处理线程（ProcFrameThrd）
//
/////////////////////////////////////////////////////////////////////////////////////	
	//算法处理线程的死循环
	void ProcFrameLoop();	
	//初始化
	bool Initialize(unsigned int *pNavID);
	//处理一帧栅格数据
	void ProcFrameLidar64Data();
	//前进状态处理算法
	void XFLLidar_Forward();
	//后退状态处理算法
	void XFLLidar_Backward();
	//将结果压缩编码并发送
	void CodeAndSend();	

public:
	unsigned int lid_pack_id;
	unsigned int *m_pNavID;
	unsigned int m_navID;
	NJUST_TO_MO_WORKSTAT workstat;
	NJUST_IP_TIME t1,t2;

	bool m_isForward;//flag,是前进状态还是后退状态
	bool m_isForward_StatusChange;//车辆行驶行驶状态发生改变的flag
	bool m_isObjDetect;//是否进入侦查模式,默认为false
	bool m_UseNegativeObsDetect;//是否要使用负障碍，默认为true
	bool m_HasObstacle;//在检测负障碍时，后沿必须含有正障碍的标识
	int m_DataNum_InOneFrame;

	//配置参数
	//栅格化
	int m_gridXcm ; //栅格水平方向的精度,单位cm
	int m_gridYcm ; //栅格垂直方向的精度,单位cm
	int m_widthCM ; //栅格图像的宽度,单位cm
	int m_beginGridDistCM; //栅格图像的起始距离,单位cm
	int m_endGridDistCM; //栅格图像的终止距离,单位cm
	//道路边界
	int m_beginBoundaryDistCM; 
	int m_endBoundaryDistCM; 
	int m_minRoadWidthCM; //道路的最小宽度
	int m_maxRoadWidthCM; //道路的最大宽度
	bool m_isStructuredRoad; //是否是结构化道路,路边比路面高者(有草或者路牙)尽可以当做结构化道路
	//正障碍
	int m_minObstacleWCM; //正障碍的最小宽度
	int m_minObstacleHCM; //正障碍的最小长度
	//负障碍
	int m_minNegativeObstacleWCM; //负障碍的最小宽度
	int m_maxNegativeObstacleWCM; //负障碍的最大宽度
	//岔路口的处理区域
	int m_beginObstacleDistCM; //栅格图像的起始距离,单位cm
	int m_endObstacleDistCM ; //栅格图像的终止距离,单位cm

	//需要发送的结构体
	NJUST_3D64_OBS_TO_FU m_3D64Obs;
	NJUST_3D64_ROAD_BOUNDARY_TO_FU m_3D64Boundary;
	NJUST_3D64_GRID_TO_FU m_3D64Grid;
	NJUST_3D64_CROSS_TO_FU m_3D64Cross;
	NJUST_3D64_TARGET_TO_2D m_3D64TargetDetect;

	//障碍物映射计数
	int m_num[NJUST_LIDAR64_VERTICAL_GRID_NUM*NJUST_LIDAR64_HORIZONTAL_GRID_NUM];

	//存储负障碍结构体
	RMW_LIDAR_3D_OBSTACLE m_NegativeRegions[MAX_NEGATIVE_OBASTACLE_NUM];

	//存储从函数中取出的障碍物图像指针
	BYTE *pObstacleTypeImg;


};


#endif
