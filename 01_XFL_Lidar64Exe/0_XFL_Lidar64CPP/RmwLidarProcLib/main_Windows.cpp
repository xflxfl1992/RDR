#include "../NJUST_ALV_BYD_H/ALV_BYD.h"

#ifdef _NJUST_OS_USE_WINDOWS_


#define  _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <memory.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../XFL_Lidar64Def.h" 
#include "../XFL_Lidar64Extern.h"
#include "../XFL_Lidar64PreProc.h"
#include "RmwLidarProcLib.h"

#include "bmpFile.h"

/////////////////////////////////////////////////////////////////////////////////////
// 
//   全局变量定义
//
/////////////////////////////////////////////////////////////////////////////////////
int gWidth,gHeight;
static RMW_LIDAR_3D_POINT_ORG  gData[MAX_LIDAR_MAX_POINT_NUM];
char g_packData[LIDAR64_IP_DATA_SIZE];//windows下的一包数据输入
/////////////////////////////////////////////////////////////////////////////////////
// 
//   全局变量定义
//
/////////////////////////////////////////////////////////////////////////////////////
unsigned int gNavID=0;
class XFL_Lidar64PreProc XFL_Preproc;
/////////////////////////////////////////////////////////////////////////////////////
// 
//   读文件
//
/////////////////////////////////////////////////////////////////////////////////////
int RmwReadLidar64Data(char *fileName,RMW_LIDAR_3D_POINT_ORG *pData,int N)
{   //返回有效数据的个数
	FILE *fp;
	int i,k;
		
	// step.0-------------打开文件-------------------------------------------//
	fp=fopen(fileName,"rb");
	if (!fp) return 0;
	// step.1-------------读文件---------------------------------------------//
	i=k=0;
	while (fread(&pData[i],sizeof(RMW_LIDAR_3D_POINT_ORG),1,fp)>0)
	{
		//是否有效
		if ( (pData[i].x!=0)&&(pData[i].y!=0)&&(pData[i].z!=0) )
		{
 			//下一个
			i++;
		}
		else 
		{
	 		k++;
	 	}
		//是否读满了
		if (i>=N) break;
	}
	// step.2-------------结束-----------------------------------------------//
	fclose(fp);
    return i;
}

int XFL_TransformOneFrame(char *fileName)
{
	static bool isFirst=true;
	bool isReady;
	FILE *fp;
	int nIP;

	// step.1-------------初始化---------------------------------------------//
	nIP=0;
	isReady=false;
	if (isFirst)
	{
		XFL_Preproc.Initialize( &gNavID);
		isFirst=false;
	}
	// step.2-------------读文件---------------------------------------------//
	fp=fopen(fileName,"rb");
	if(!fp) return 0;
	nIP=0;
	while (fread(&g_packData,1,LIDAR64_IP_DATA_SIZE,fp)>0)
	{
		isReady=XFL_Preproc.DataTransformLoop();
		nIP++;
		if (isReady) break;
	}
	// step.3-------------返回-----------------------------------------------//
	printf("\nisReady=%d__nIP=%d",isReady,nIP);
	//复位
	XFL_Preproc.Reset();
	//返回
	return nIP;
}
/////////////////////////////////////////////////////////////////////////////////////
// 
//   测试函数
//
/////////////////////////////////////////////////////////////////////////////////////
void testBoundary()
{
	BYTE *pScanlineTypeImg;
	char filePathLidar[100][256];
 	char fileName[256];
	int width,height;
	int i,j,k,N;
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
  		                               18*100, //道路边界的终止距离,单位cm,车(雷达)前18m,防止弯道
								       3*100, //路面最窄1个车道,3m
				                       20*100, //路面最宽5个车道,20m
									   //正障碍
                                       30, //正障碍的最小宽度,5cm*6个栅格=30cm
							           12, //正障碍的最小长度,2cm*6个栅格=12cm
									   //负障碍
								       40, //负障碍的最小宽度,40cm
								       40*5 //负障碍的最大宽度,200cm
				                   );
	//step.2---------------------执行----------------------------------------//
	k=15;
	for(i=begin[k];i<=end[k];i++)
	{
		//k=10,i=1334;
		//k=14;i=3894;
		//k=11,i=3431;
		//i=755; //侧面
		//i=1007; //侧面
		//i=1027; //侧面
		//i=1135; //正面

		//k=1,i=9716;
		//k=9,i=742;
	  
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
		bool isLeftBst; //是否左边界线是最佳的
    	RmwLidarRoadBoundary_GetBoundaryLine( &A1,&B1,&C1,&OK1,
		                                      &A2,&B2,&C2,&OK2,
											  &isLeftBst
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
		for(int scanlineID=6;scanlineID<30;scanlineID++)
		{
			//scanlineID=29;
			RmwLidarNegativeObstalce_DoNext( scanlineID, //线号
				                             5, //障碍在y方向的最小变化,依线号不同而变化,10cm
		                                     120, //障碍在y方向的最大变化,依线号不同而变化,120cm
											 i, 
				                             false
				                             //true
				                           );
			//break;
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
		int pLeft[10];
		int pRight[10];
		int pTop[10];
		int pBottom[10];
		int nTarget;
	    t1=clock();
        //执行
		RmwLidarTarge_DoNext( i,
			                  //true
							  false
			                );
		//结果
        nTarget=RmwLidarTarge_GetTargetPos(pLeft,pRight,pTop,pBottom,10);
     	t2=clock();
		printf("\n%dms",t2-t1);   
		//////////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//调试
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
        pScanlineTypeImg=RmwLidarGrid_GetScanlineTypeImg(&width,&height);
		//道边-最佳红色,其他绿色
		if (OK1)
		{
			if (isLeftBst)
			{
				RmwDrawABCLine(pScanlineTypeImg,width,height,A1,B1,C1,255);
			}
			else
			{
				RmwDrawABCLine(pScanlineTypeImg,width,height,A1,B1,C1,254);
			}
		}
		if (OK2)
		{
			if (!isLeftBst)
			{
				RmwDrawABCLine(pScanlineTypeImg,width,height,A2,B2,C2,255);
			}
			else
			{
				RmwDrawABCLine(pScanlineTypeImg,width,height,A2,B2,C2,254);
			}
		}
		//侦察 
		for(j=0;j<nTarget;j++)
		{
			RmwRectFill( pScanlineTypeImg,
				         width,height,
					     pLeft[j]-2,
					     pRight[j]+2,
					     pTop[j]-2,
					     pBottom[j]+2,
					     255 //红色
				      );
		}
		//保存
		sprintf(fileName,"D:\\tmp\\res_RmwRoadBoundary_frameID=%010d_1.bmp",i);
		Rmw_Write8BitImg2BmpFileMark254Green255Red( pScanlineTypeImg,
							                        width,height,
													fileName
											      );
		//得到障碍的二值图像,用户自己定义阈值
        BYTE *pObstacleBinImg=RmwLidarGrid_GetObstacleBinImg( 5, //近处阈值大
									                          3, //远处阈值小
	                                                          &width,
									                          &height
								                           );
		sprintf(fileName,"D:\\tmp\\res_RmwRoadBoundary_frameID=%010d_1_Obs.bmp",i);
		Rmw_Write8BitImg2BmpFileMark254Green255Red( pObstacleBinImg,
							                        width,height,
													fileName
											      );
		//////////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//下一帧
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
		//break;
	}
	printf("\nPress any key to continue....");
	getchar();
	return;
}

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
	k=10;
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
					              true
								  //false
					            );
		t2=clock();
		printf("\n%dms",t2-t1);
 		//结束
		//break;
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
#endif