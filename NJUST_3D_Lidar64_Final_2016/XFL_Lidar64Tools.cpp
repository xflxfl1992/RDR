#include "XFL_Lidar64Tools.h"

/////////////////////////////////////////////////////////////////////////////////////
// 
//   初始化pPosImg的参数(XFL_3D64_DataHold)
//
/////////////////////////////////////////////////////////////////////////////////////
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
						 )
{
	if (* isForward)
	{
		* gridXcm = 5; //栅格水平方向的精度,单位cm,5cm以保持左右连接和合适的处理速度
		* gridYcm = 2; //栅格垂直方向的精度,单位cm,2cm以保持行能分开
		* widthCM = 40*100; //栅格图像的宽度,单位cm,左右各20m
		* beginGridDistCM= -6*100; //栅格图像的起始距离,单位cm,车(雷达)前1m,盲区
		* endGridDistCM = 45*100; //栅格图像的终止距离,单位cm,单位cm,车(雷达)前18m
		
		* beginBoundaryDistCM = 3*100 ; //边界栅格图像的起始距离,单位cm
		* endBoundaryDistCM = 23*100 ; //边界栅格图像的终止距离,单位cm
		* minRoadWidthCM = 3*100; //道路的最小宽度
		* maxRoadWidthCM = 15*100; //道路的最大宽度

		* minObstacleWCM = 30; //障碍的最小宽度,单位cm
		* minObstacleHCM = 12; //障碍的最小高度,单位cm
		
		* minNegativeObstacleWCM = 80 ; //负障碍的最小宽度
		* maxNegativeObstacleWCM = 180; //负障碍的最大宽度
	}
	if (!(* isForward))
	{
		* gridXcm = 5; //栅格水平方向的精度,单位cm
		* gridYcm = 2; //栅格垂直方向的精度,单位cm
		* widthCM = 40*100; //栅格图像的宽度,单位cm
		* beginObstacleDistCM= -6*100; //栅格图像的起始距离,单位cm
		* endObstacleDistCM = 10*100; //栅格图像的终止距离,单位cm
		* minObstacleWCM = 30; //障碍的最小宽度,单位cm
		* minObstacleHCM = 12; //障碍的最小长度,单位cm
	}
	
	
	return ;
}


/////////////////////////////////////////////////////////////////////////////////////
// 
//   按直线插入点
//
/////////////////////////////////////////////////////////////////////////////////////
void InsertPoint(int step_y,//步长
 				 int N,//最多画N个点
 				 double A,double B,double C,//直线方程参数
 				 NJUST_3D64_ROAD_BOUNDARY_TO_FU * p3D64Boundary,
 				 bool isLeft,
 				 int gridXcm,
 				 int gridYcm,
 				 int beginDistCM
 				 )
 {
 	//step.1----------------求出原图上的对应坐标点---------------------//
 	int x = 0;
 	int y = step_y*5-beginDistCM/gridYcm;//当前图像坐标系下的x,y
 
 	for(int i = 0; i< N ; i++)
 	{
 		y += step_y;
 		if ((A<0.001)&&(A>-0.001)) continue;
 		x = int((-C-B*y)/A);//车体图像中的x坐标
 		x = x*gridXcm -2000;//转到车体实际坐标，单位为cm
 		if(x<2000&&x>-2000)
 		{
 			p3D64Boundary->Point[p3D64Boundary->nPoint].x_cm = x;
 			p3D64Boundary->Point[p3D64Boundary->nPoint].y_cm = y*gridYcm + beginDistCM;//转成车体实际坐标
 			if(isLeft) p3D64Boundary->Type[p3D64Boundary->nPoint] = NJUST_3D64_ROAD_POINT_TYPE_LEFT;
 			else p3D64Boundary->Type[p3D64Boundary->nPoint] = NJUST_3D64_ROAD_POINT_TYPE_RIGHT;
 			(p3D64Boundary->nPoint) ++;
 		}			
 	}
 	return;
 }