#ifndef XFL_LIDAR64_TOOLS
#define XFL_LIDAR64_TOOLS

#include "NJUST_ALV_BYD_H/ALV_BYD.h"
//用于初始化参数
void Initialize_ImgInput(bool* isForward,
						 //栅格化
	                     int* gridXcm, //栅格水平方向的精度,单位cm
                         int* gridYcm, //栅格垂直方向的精度,单位cm
		                 int* widthCM, //栅格图像的宽度,单位cm
		                 int* beginGridDistCM, //栅格图像的起始距离,单位cm
		                 int* endGridDistCM, //栅格图像的终止距离,单位cm
						 //道路边界
						 int* beginBoundaryDistCM, //边界栅格图像的起始距离,单位cm
		                 int* endBoundaryDistCM, //边界栅格图像的终止距离,单位cm
						 int* minRoadWidthCM, //道路的最小宽度
					     int* maxRoadWidthCM, //道路的最大宽度
						 //正障碍
                         int* minObstacleWCM, //正障碍的最小宽度
		                 int* minObstacleHCM,  //正障碍的最小长度
						 //负障碍
						 int* minNegativeObstacleWCM, //负障碍的最小宽度
		                 int* maxNegativeObstacleWCM,  //负障碍的最大宽度
						 //岔路口的处理区域
						 int* beginObstacleDistCM, //栅格图像的起始距离,单位cm
						 int* endObstacleDistCM //栅格图像的终止距离,单位cm
						 );

//用于根据直线插点
void InsertPoint(int step_y,//步长
 				 int N,//最多画N个点
 				 double A,double B,double C,//直线方程参数
 				 NJUST_3D64_ROAD_BOUNDARY_TO_FU * p3D64Boundary,
 				 bool isLeft,
 				 int gridXcm,
 				 int gridYcm,
 				 int beginDistCM
 				 );

#endif