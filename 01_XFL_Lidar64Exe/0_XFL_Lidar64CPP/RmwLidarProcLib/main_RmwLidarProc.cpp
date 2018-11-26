#define  _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <memory.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "bmpFile.h"

#include "../../3_XFL_Lidar64CPP/RmwRead64LidarFile.h"

#include "../../5_RmwLidarProcCPP/Rmw3DdataGrid.h"
#include "../../5_RmwLidarProcCPP/RmwLidarProcLib.h"

/////////////////////////////////////////////////////////////////////////////////////
// 
//   全局变量定义
//
/////////////////////////////////////////////////////////////////////////////////////
int gWidth,gHeight;
static RMW_LIDAR_3D_POINT_ORG  gData[MAX_LIDAR_MAX_POINT_NUM];


void testCross()
{
	char filePathLidar[100][256];
 	char fileName[256];
	int i,k,N;
	clock_t t1,t2;
	int begin[100],end[100];

	//step.0---------------------数据集--------------------------------------//
	//数据集01
	begin[1]=9570; 
	end[1]=9756;
	strcpy(filePathLidar[1],"D:\\018_3DProc\\0_Data\\64_LIDAR\\01_负障碍\\64");
	//数据集02
	begin[2]=8900; 
	end[2]=9115;
	strcpy(filePathLidar[2],"D:\\018_3DProc\\0_Data\\64_LIDAR\\02_连续弯\\64");
	//数据集03
	begin[3]=7050; 
	end[3]=7300;
	strcpy(filePathLidar[3],"D:\\018_3DProc\\0_Data\\64_LIDAR\\03_土路\\64");
	//数据集04
	begin[4]=5500; 
	end[4]=5640;
	strcpy(filePathLidar[4],"D:\\018_3DProc\\0_Data\\64_LIDAR\\04_窄桥\\64");
	//数据集05
	begin[5]=9300; 
	end[5]=9500;
	strcpy(filePathLidar[5],"D:\\018_3DProc\\0_Data\\64_LIDAR\\05_长桥\\64");
	//数据集06
	begin[6]=5350; 
	end[6]=5420;
	strcpy(filePathLidar[6],"D:\\018_3DProc\\0_Data\\64_LIDAR\\06_岔路口1\\64");
	//数据集07
	begin[7]=6200; 
	end[7]=6360;
	strcpy(filePathLidar[7],"D:\\018_3DProc\\0_Data\\64_LIDAR\\07_岔路口2\\64");
	//数据集08
	begin[8]=950; 
	end[8]=1050;
	strcpy(filePathLidar[8],"D:\\018_3DProc\\0_Data\\64_LIDAR\\08_负障碍_0815\\64");
	//数据集09
	begin[9]=720; 
	end[9]=780;
	strcpy(filePathLidar[9],"C:\\水桶_0820\\03_油桶无遮挡_直行\\64");
	//数据集10
	begin[10]=900; 
	end[10]=1500;
	strcpy(filePathLidar[10],"C:\\水桶_0820\\01_油桶无遮挡_各角度侦查\\64");
	//step.1---------------------初始化--------------------------------------//
    RmwLidarRoadCross_Initialize( 5,//栅格水平方向的精度,单位cm,5cm以保持左右连接和合适的处理速度
		                          2, //栅格垂直方向的精度,单位cm,2cm以保持行能分开
		                          40*100, //栅格图像的宽度,单位cm,左右各20m
		                          -6*100, //栅格图像的起始距离,单位cm,雷达后6m
  		                          10*100,  //栅格图像的终止距离,单位cm,雷达前10m
							      30, //障碍的最小宽度,5cm*6个栅格=30cm
							      12 //障碍的最小长度,2cm*6个栅格=12cm
				                );
	//step.2---------------------执行----------------------------------------//
	k=6;
	for(i=begin[k];i<=end[k];i++)
	{
		//i=6250;
		//读取雷达数据
		sprintf(fileName,"%s\\3D64_%010d.bin",filePathLidar[k],i);
		N=RmwReadLidar64Data(fileName,gData,MAX_LIDAR_MAX_POINT_NUM);
		if (N==0)
		{
			//printf("\n%s read err!",fileName);
			continue;
		}
		printf("\n%s read ok(%d)!",fileName,N);
		//执行
		t1=clock();
		RmwLidarRoadCross_DoNext( gData,
					              N,
					              i,
					              //true
								  false
					            );
		t2=clock();
		printf("\n%dms",t2-t1);
	}
	printf("\nPress any key to continue....");
	getchar();
	return;
}

/////////////////////////////////////////////////////////////////////////////////////
// 
//   测试函数
//
/////////////////////////////////////////////////////////////////////////////////////
void testBoundary()
{
	BYTE *pDrawImg;
	char filePathLidar[100][256];
 	char fileName[256];
	int width,height;
	int i,j,k,m,N;
	clock_t t1,t2;
	int begin[100],end[100];
	int nLineNum;

	//step.0---------------------数据集--------------------------------------//
	//数据集01
	begin[1]=9570; 
	end[1]=9756;
	strcpy(filePathLidar[1],"D:\\018_3DProc\\0_Data\\64_LIDAR\\01_负障碍\\64");
	//数据集02
	begin[2]=8900; 
	end[2]=9115;
	strcpy(filePathLidar[2],"D:\\018_3DProc\\0_Data\\64_LIDAR\\02_连续弯\\64");
	//数据集03
	begin[3]=7050; 
	end[3]=7300;
	strcpy(filePathLidar[3],"D:\\018_3DProc\\0_Data\\64_LIDAR\\03_土路\\64");
	//数据集04
	begin[4]=5500; 
	end[4]=5640;
	strcpy(filePathLidar[4],"D:\\018_3DProc\\0_Data\\64_LIDAR\\04_窄桥\\64");
	//数据集05
	begin[5]=9300; 
	end[5]=9500;
	strcpy(filePathLidar[5],"D:\\018_3DProc\\0_Data\\64_LIDAR\\05_长桥\\64");
	//数据集06
	begin[6]=5350; 
	end[6]=5420;
	strcpy(filePathLidar[6],"D:\\018_3DProc\\0_Data\\64_LIDAR\\06_岔路口1\\64");
	//数据集07
	begin[7]=6200; 
	end[7]=6360;
	strcpy(filePathLidar[7],"D:\\018_3DProc\\0_Data\\64_LIDAR\\07_岔路口2\\64");
	//数据集08
	begin[8]=950; 
	end[8]=1050;
	strcpy(filePathLidar[8],"D:\\018_3DProc\\0_Data\\64_LIDAR\\08_负障碍_0815\\64");
	//数据集09
	begin[9]=720; 
	end[9]=780;
	strcpy(filePathLidar[9],"C:\\水桶_0820\\03_油桶无遮挡_直行\\64");
	//数据集10
	begin[10]=900; 
	end[10]=1500;
	strcpy(filePathLidar[10],"C:\\水桶_0820\\01_油桶无遮挡_各角度侦查\\64");
	//数据集11
	begin[11]=3430; 
	end[11]=3484;
	strcpy(filePathLidar[11],"C:\\水桶_0820\\05_油桶贴着遮挡_直行\\64");
	//数据集12
	begin[12]=4150; 
	end[12]=4450;
	strcpy(filePathLidar[12],"C:\\水桶_0820\\06_油桶距离遮挡1.5m_环行侦查\\64");
	//数据集13
	begin[13]=4900; 
	end[13]=5000;
	strcpy(filePathLidar[13],"C:\\水桶_0820\\07_油桶距离遮挡1.5m_直行\\64");
	//数据集14
	begin[14]=3715; 
	end[14]=3984;
	strcpy(filePathLidar[14],"C:\\水桶_0820\\04_油桶贴着遮挡_环行侦查\\64");
	//数据集14
	begin[15]=888; 
	end[15]=1208;
	strcpy(filePathLidar[15],"D:\\018_3DProc\\0_Data\\64_LIDAR\\08_侦查");
	//数据集14
	begin[16]=0; 
	end[16]=62;
	strcpy(filePathLidar[16],"D:\\018_3DProc\\0_Data\\64_LIDAR\\10_程序错");

	//数据集14
	begin[17]=151; 
	end[17]=280;
	strcpy(filePathLidar[17],"D:\\018_3DProc\\0_Data\\64_LIDAR\\11_路锥");

	//数据集14
	begin[18]=2400; 
	end[18]=2600;
	strcpy(filePathLidar[18],"D:\\018_3DProc\\0_Data\\64_LIDAR\\12_塔河");
	
	//数据集15
	begin[19]=601; 
	end[19]=1054;
	strcpy(filePathLidar[19],"D:\\018_3DProc\\0_Data\\64_LIDAR\\13_平整路面负障碍");


	//数据集16
	//begin[20]=222; 
	begin[20]=810; 
	//end[20]=810;//1157;
	end[20]=1157;
	strcpy(filePathLidar[20],"F:\\塔河_土路_0905");
	//数据集16
	begin[21]=2900; 
	end[21]=3200;
	strcpy(filePathLidar[21],"H:\\bin_0909");
	strcpy(filePathLidar[21],"D:\\018_3DProc\\0_Data\\64_LIDAR\\14_高路"); //2940用作找两边的沟
	//数据集22
	begin[22]=0; 
	end[22]=1000;
	strcpy(filePathLidar[22],"H:\\Hole_DM_0923");
    strcpy(filePathLidar[22],"H:\\Hole_BYD_0923"); //0,310[366]
	//strcpy(filePathLidar[22],"H:\\JieMu_BYD_0924"); 
	//strcpy(filePathLidar[22],"H:\\DM_0924\\DM_人与桶_0924");  
	//strcpy(filePathLidar[22],"H:\\DM_0924\\DM_水坑加土_0924");  //250,360
	strcpy(filePathLidar[22],"H:\\0909_土路返回");  //250,360
	strcpy(filePathLidar[22],"D:\\018_3DProc\\0_Data\\64_LIDAR\\15_测试");
	strcpy(filePathLidar[22],"H:\\DM_坨里_1013");
	strcpy(filePathLidar[22],"H:\\BYD_负障碍_1015");
	strcpy(filePathLidar[22],"H:\\DM_壕沟_1016");

	//数据集23
	begin[23]=0; 
	end[23]=1600;
	strcpy(filePathLidar[23],"H:\\BYD_弹药箱_1013");
	strcpy(filePathLidar[23],"H:\\BYD_侦查_1014");
	strcpy(filePathLidar[23],"H:\\BYD_花坛_1014");

	//step.1---------------------初始化--------------------------------------//
	RmwLidarObstacleBoundary_Initialize( 
	                                   //栅格化
	                                   5,//栅格水平方向的精度,单位cm,5cm以保持左右连接和合适的处理速度
		                               2, //栅格垂直方向的精度,单位cm,2cm以保持行能分开
		                               40*100, //栅格图像的宽度,单位cm,左右各20m
		                               -6*100, //栅格图像的起始距离,单位cm,雷达后6m
									   30*100,  //栅格图像的终止距离,单位cm,车(雷达)前30m
									   //道路边界
									   6*100, //道路边界的起始距离,单位cm,雷达前6m
  		                               //18*100, //道路边界的终止距离,单位cm,车(雷达)前18m,防止弯道
  		                               25*100, //道路边界的终止距离,单位cm,车(雷达)前18m,防止弯道
								       3*100, //路面最窄1个车道,3m
				                       15*100, //路面最宽5个车道,20m
									   //true, //是否是结构化道路,路边比路面高者(有草或者路牙)尽可以当做结构化道路
				 					   false,
									   //正障碍
                                       30, //正障碍的最小宽度,5cm*6个栅格=30cm
							           12, //正障碍的最小长度,2cm*6个栅格=12cm
									   //负障碍
								       65, //负障碍的最小宽度,75cm
								       150 //负障碍的最大宽度,150cm
				                   );
	//step.2---------------------执行----------------------------------------//
	k=22;
	//for(i=begin[k];i<=end[k];i+=10)
	//for(i=begin[k];i<=end[k];i++)
	for(i=166;i<180;i++)
	{
		//if (i!=165) continue;
		//if (i!=215) continue;
		//if (i!=226) continue;
		//if (i!=264) continue;
		//////////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//读取雷达数据
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
		sprintf(fileName,"%s\\3D64_%010d.bin",filePathLidar[k],i);
		N=RmwReadLidar64Data(fileName,gData,MAX_LIDAR_MAX_POINT_NUM);
		if (N==0)
		{
			//printf("\n%s read err!",fileName);
			continue;
		}
		printf("\n%s read ok(%d)!",fileName,N);

	    //////////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//执行: 道边检测
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
		t1=clock();
		RmwLidarObstacleBoundary_DoNext( gData,
					                     N,
					                     i,
					                     //true
									     false
					                   );
		//道边结果
	    double A1,B1,C1;
	    double A2,B2,C2;
		bool OK1,OK2;
    	RmwLidarRoadBoundary_GetBoundaryLine( &A1,&B1,&C1,&OK1,
		                                      &A2,&B2,&C2,&OK2
		                                    );
		t2=clock();
		printf("\n%dms",t2-t1);
		///////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//执行: 负障碍检测
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
		t1=clock();
		int nNegativeObstalce=0;
		RMW_LIDAR_3D_OBSTACLE pNegativeObstalce[200];
		for(int scanlineID=1;scanlineID<40;scanlineID++)
		{
			//if (scanlineID!=19) continue;

			//if (scanlineID!=31) continue;
			//if (scanlineID!=33) continue;
			RmwLidarNegativeObstalce_DoNext( scanlineID, //线号
				                             50, //障碍在y方向的最小变化,依线号不同而变化,50cm
		                                     150, //障碍在y方向的最大变化,依线号不同而变化,150cm
											 i, 
				                             false
				                             //true
				                           );



			//结果
			nNegativeObstalce += RmwLidarNegativeObstalce_GetObstacleRegions(pNegativeObstalce+nNegativeObstalce,200-nNegativeObstalce);
		}
		t2=clock();
		printf("\n%dms",t2-t1);
		//////////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//执行: 目标侦察
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
		int pTargetLeft[50];
		int pTargetRight[50];
		int pTargetTop[50];
		int pTargetBottom[50];
		int nTarget;
	    t1=clock();
        //执行
		RmwLidarTarget_DoNext( i,
			                  //true
							  false
			                );
		//结果
        nTarget=RmwLidarTarget_GetTargetPos(pTargetLeft,pTargetRight,pTargetTop,pTargetBottom,50);
     	t2=clock();
		printf("\n%dms",t2-t1);   
		//////////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//调试
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
        //pDrawImg=RmwLidarGrid_GetScanlineTypeImg(&width,&height);
		pDrawImg=RmwLidarGrid_GetObstacleTypeImg(&width,&height);
		//道边-最佳黄色,其他蓝色
		nLineNum=0;
		if (OK1)
		{
			nLineNum++;
			RmwDrawABCLine(pDrawImg,width,height,A1,B1,C1,252);
		}
		if (OK2)
		{
			nLineNum++;
			RmwDrawABCLine(pDrawImg,width,height,A2,B2,C2,252);
		}
		//侦察目标
		for(j=0;j<nTarget;j++)
		{
			RmwRectFill( pDrawImg,
				         width,height,
					     pTargetLeft[j]-2,
					     pTargetRight[j]+2,
					     pTargetTop[j]-2,
					     pTargetBottom[j]+2,
					     253 //蓝色
				      );
		}
		//负障碍
		for(j=0;j<nNegativeObstalce;j++)
		{
			for(m=0;m<3;m++)
			RmwRectDraw( pDrawImg,
				         width,height,
						 pNegativeObstalce[j].gridX1-m,
						 pNegativeObstalce[j].gridX2+m,
						 pNegativeObstalce[j].gridY1-m,
						 pNegativeObstalce[j].gridY2+m,
					     252 //黄色
				      );
		}
		//保存
		sprintf( fileName,"D:\\tmp\\res_RmwRoadBoundary_frameID=%010d_1_LineNum=%d_NegativeObstalceNum=%d_targetNum=%d.bmp",
			     i,
				 nLineNum,
				 nNegativeObstalce,
				 nTarget
			   );
		//if (nLineNum==2)
		//Rmw_Write8BitImgMark(pDrawImg,width,height,fileName);
		//得到用户自己定义的障碍图像
		BYTE *pObstacleBinImg=RmwLidarGrid_GetObstacleBinImg( 3, //近处阈值大
									                          2, //远处阈值小
	                                                          &width,
									                          &height
								                           );
		sprintf(fileName,"D:\\tmp\\res_RmwRoadBoundary_frameID=%010d_1_ObstalceMap.bmp",i);
		Rmw_Write8BitImgMark(pObstacleBinImg,width,height,fileName);
		//高度图
		BYTE *pZImg;
		int resolutionZCM,offset;
		pZImg=RmwLidarGrid_GetZImg(&width,&height,&resolutionZCM,&offset); 
		sprintf(fileName,"D:\\tmp\\res_RmwRoadBoundary_frameID=%010d_3_Z_%dcm.bmp",i,resolutionZCM);
		//Rmw_Write8BitImgMark(pZImg,width,height,fileName);
		//////////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//下一帧
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
	}
	printf("\nPress any key to continue....");
	getchar();
	return;
}

void main()
{
	testBoundary();
	//testCross();
	return;
}

  