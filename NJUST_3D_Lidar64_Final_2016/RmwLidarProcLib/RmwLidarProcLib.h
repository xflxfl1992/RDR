///////////////////////////////////////////////////////////////////////////////////
// 严正声明 ： 
//  1. 本程序属于任明武个人开发，不经授权，不得用于商业目的及应用软件中     
//  2. 由于本程序是个人开发的，不可避免地存在各种各样的缺陷或错误，所以本程序最好
//     仅仅应用于科研研究和教学中。
//  3. 若将本程序应用在实际领域时，由其导致的各种不良后果，本人概不负责。
//     如果您发现错误，可以联系renmingwu@mail.njust.edu.cn, renmingwu@sina.com
//  4. 如果你分发该文件，请保留这些文字
//
///////////////////////////////////////////////////////////////////////////////////
// Caution:
// 1) If you do not get authorization from Mingwu Ren, 
//    this lib CAN NOT be used for commerce.
// 2) It is only developed by individual, so the errors can not be avioded. 
//    I strongly suggest that it should only be used for research and education.
// 3) If you find errors, you can tell me. E-Mail: renmingwu@mail.njust.edu.cn
//    renmingwu@sina.com,
//    you can call me: 13952033136 
// 4) If you redistribute this lib, please keep these words.
///////////////////////////////////////////////////////////////////////////////////
#ifndef RMW_LIDAR_LINE_OBSTACLE_LIB_H
#define RMW_LIDAR_LINE_OBSTACLE_LIB_H

#include "RmwLidarStruct.h"

typedef unsigned char BYTE;

///////////////////////////////////////////////////////////////////////////////////
//
// 左右边界线
//
///////////////////////////////////////////////////////////////////////////////////
//初始化
bool RmwLidarObstacleBoundary_Initialize( 
	                                  //栅格化
	                                  int gridXcm, //栅格水平方向的精度,单位cm
                                      int gridYcm, //栅格垂直方向的精度,单位cm
		                              int widthCM, //栅格图像的宽度,单位cm
		                              int beginGridDistCM, //栅格图像的起始距离,单位cm
		                              int endGridDistCM, //栅格图像的终止距离,单位cm
									  //道路边界
									  int beginBoundaryDistCM, //边界栅格图像的起始距离,单位cm
		                              int endBoundaryDistCM, //边界栅格图像的终止距离,单位cm
								      int minRoadWidthCM, //道路的最小宽度
					                  int maxRoadWidthCM, //道路的最大宽度
									  bool isStructuredRoad, //是否是结构化道路,路边比路面高者(有草或者路牙)尽可以当做结构化道路
									   //正障碍
                                      int minObstacleWCM, //正障碍的最小宽度
		                              int minObstacleHCM,  //正障碍的最小长度
									  //负障碍
									  int minNegativeObstacleWCM, //负障碍的最小宽度
								      int maxNegativeObstacleWCM  //负障碍的最大宽度
				                   );
char *RmwLidarObstacleBoundary_GetErrInfo();
//执行
bool RmwLidarObstacleBoundary_DoNext( RMW_LIDAR_3D_POINT_ORG *p3DData,  //3D数据
				                      int n3DData, //3D数据的个数
				                      int frameID, //帧号
						              bool doDebug
			                        );
//栅格结果
BYTE *RmwLidarGrid_GetScanlineTypeImg(int *width,int *height);  //线号图像
BYTE *RmwLidarGrid_GetObstacleScoreImg(int *width,int *height); //障得分图像
BYTE *RmwLidarGrid_GetObstacleTypeImg(int *width,int *height);  //障碍类型图像
BYTE *RmwLidarGrid_GetZImg( int *width,int *height,
						    int *resolutionZCM,int *offset
						   ); //高度图
//得到障碍的二值图像,用户自己定义阈值
BYTE *RmwLidarGrid_GetObstacleBinImg( int nearThre, //近处阈值大
									  int farThre, //远处阈值小
	                                  int *width,
									  int *height
								    );

//边界结果
void RmwLidarRoadBoundary_GetBoundaryLine( double *A1,double *B1,double *C1,bool *OK1, //左侧直线的方程AX+BY+C=0
					                       double *A2,double *B2,double *C2,bool *OK2  //右侧直线的方程AX+BY+C=0
					                     );
///////////////////////////////////////////////////////////////////////////////////
//
// 负障碍
//
///////////////////////////////////////////////////////////////////////////////////
//执行
int RmwLidarNegativeObstalce_DoNext( int scanlineID, //线号
	                                 int minDeltaHeightCM, //障碍在y方向的最小变化,依线号不同而变化
		                             int maxDeltaHeightCM, //障碍在y方向的最大变化,依线号不同而变化
	                                 int frameID, //帧号
						             bool doDebug
			                       );
//结果
int RmwLidarNegativeObstalce_GetObstacleRegions(RMW_LIDAR_3D_OBSTACLE *pRegions,int N);
//取数据
int RmwLidarNegativeObstalce_Get3DPointRes( int scanlineID,
										    int beginPointID,int endPointID,
											RMW_LIDAR_3D_POINT_RES *p3DData,
											int N
									      );
///////////////////////////////////////////////////////////////////////////////////
//
// 目标侦察
//
///////////////////////////////////////////////////////////////////////////////////
//执行
bool RmwLidarTarget_DoNext( int frameID, //帧号 
				            bool doDebug
			              );
//结果
int RmwLidarTarget_GetTargetPos( int *pLeft,
								 int *pRight,
								 int *pTop,
								 int *pBottom,
								 int N
							   );
///////////////////////////////////////////////////////////////////////////////////
//
// 道路岔口
//
///////////////////////////////////////////////////////////////////////////////////
//初始化
bool RmwLidarRoadCross_Initialize( int gridXcm, //栅格水平方向的精度,单位cm
                                   int gridYcm, //栅格垂直方向的精度,单位cm
		                           int widthCM, //栅格图像的宽度,单位cm
		                           int beginObstacleDistCM, //栅格图像的起始距离,单位cm
		                           int endObstacleDistCM, //栅格图像的终止距离,单位cm
								   int minObstacleWCM, //障碍的最小宽度
		                           int minObstacleHCM  //障碍的最小长度
				                 );
//执行
bool RmwLidarRoadCross_DoNext( RMW_LIDAR_3D_POINT_ORG *p3DData,  //3D数据
				               int n3DData, //3D数据的个数
				               int frameID, //帧号
						       bool doDebug
			                 );
//结果-栅格图像
BYTE *RmwLidarRoadCross_GetScanlineTypeImg(int *width,int *height);
BYTE *RmwLidarRoadCross_GetObstacleTypeImg(int *width,int *height);
//结果:最佳导引点
bool RmwLidarCross_GetBestPoint(int *x,int *y);
//结果:最佳直线的方程,在有最佳导引点时返回false
bool RmwLidarCross_GetBestLine( double *A,double *B,double *C);
//结果:岔路的掩模,在有最佳导引点时返回NULL
BYTE *RmwLidarCross_GetCrossMskImg(int *width,int *height);
////////////////////////////////////////////////////////////////////////////////
//
// 壕沟检测
//
////////////////////////////////////////////////////////////////////////////////
//执行
bool RmwLidarRoadDitch_DoNext( BYTE *pObstalceBinImg, //障碍图像
				               int frameID, //帧号 
				               bool doDebug
			                );
//结果
bool RmwLidarRoadDitch_GetDitchPos( int *nearX1,int *nearX2,int *nearY1,int *nearY2,
		                            int *farX1,int *farX2,int *farY1,int *farY2
		                          );
bool RmwLidarRoadDitch_GetMendPos( int *mendX1,int *mendX2,int *mendY1,int *mendY2);
////////////////////////////////////////////////////////////////////////////////
//
// 水体检测
//
///////////////////////////////////////////////////////////////////////////////////
//初始化
bool RmwLidarWater_Initialize( int width, //栅格图像的宽度
		                       int height, //栅格图像的高度
		                       int minObstacleW, //障碍的最小宽度
		                       int minObstacleH, //障碍的最小高度
					           int nShrinkY //y方向缩小的倍数,提高速度
				             );
//执行
int RmwLidarWater_DoNext( BYTE *pGryImg,
					      int frameID,
						  bool doDebug
						);
//结果
int RmwLidarWater_GetWaterRegions(RMW_LIDAR_2D_OBSTACLE *pRegions,int N);
///////////////////////////////////////////////////////////////////////////////////
//
//  工具
//
///////////////////////////////////////////////////////////////////////////////////
void RmwDrawABCLine( BYTE *pGryImg,int width,int height,
					 double A,double B,double C,
					 BYTE Color
				   );
void Rmw2PLine(BYTE *pGryImg,int width,int x0,int y0,int x1,int y1,int color);
void RmwRectDraw(BYTE *pGryImg,int width,int height,int x1,int x2,int y1,int y2,int Color);
void RmwRectFill(BYTE *pGryImg,int width,int height,int x1,int x2,int y1,int y2,int Color);

#endif
 