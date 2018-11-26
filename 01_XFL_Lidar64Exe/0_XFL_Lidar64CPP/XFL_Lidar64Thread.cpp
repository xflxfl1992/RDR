#include "NJUST_ALV_BYD_H/ALV_BYD.h"

#include <stdio.h>
#include <fstream>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "XFL_Lidar64Def.h"
#include "XFL_Lidar64ReceiveData.h"
#include "XFL_Lidar64PreProc.h"
#include "XFL_Lidar64ProcFrame.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 全局变量
//
////////////////////////////////////////////////////////////////////////////////////////////////////
extern bool _gDoDebug; //是否打印调试信息
int _gBytesIP[LIDAR64_MAX_IP_DATA_NUM_ONE_FRAME];   //记录每个IPData的字节数
char _gOneFrameBuf[LIDAR64_MAX_IP_DATA_NUM_ONE_FRAME][LIDAR64_MAX_IP_DATA_SIZE];//一帧数据,按348网络包,每包一行存.
int _gTotalIpDataReceivedNum=0; //总共接收到的IP包数
RMW_LIDAR_3D_POINT_ORG _g3DDataNewFrame[XFL_3D64_MAX_POINT_NUM];//当前新帧数据
int _g3DDataNumInNewFrame=0;//当前新帧数据的个数
int _gTotal3DDataFrame=0; //总共生成的新帧数
unsigned int *_gpNavID;//虚指针,来自惯导的解码函数
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 全局对象
//
////////////////////////////////////////////////////////////////////////////////////////////////////
XFL_Lidar64ReceiveData _gReceiveData;
XFL_Lidar64PreProc _gPreProc;
XFL_Lidar64ProcFrame _gpProcFrame;
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// LINUX下:
// 
// 多线程同步问题，不仅要有互斥变量，还需要有同步信号量来通知另一个线程开始的条件
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _NJUST_OS_USE_LINUX_
   pthread_mutex_t gMutex_NewIPData = PTHREAD_MUTEX_INITIALIZER;
   pthread_mutex_t gMutex_NewFrame = PTHREAD_MUTEX_INITIALIZER;
#endif
/////////////////////////////////////////////////////////////////////////////////////
// 
//
//   线程定义
//
//
/////////////////////////////////////////////////////////////////////////////////////
#ifdef _NJUST_OS_USE_LINUX_
pthread_t gThread_ReceiveData;//  create a thread to receive the udp data
pthread_t gThread_PreProc; // create a thread to pre-process data;
pthread_t gThread_ProcFrame;//create a thread to process one whole frame;
#endif
/////////////////////////////////////////////////////////////////////////////////////
// 
//
//   创建三个线程
//
//
/////////////////////////////////////////////////////////////////////////////////////
void *ReceivingThrd(void *arg)
{
    #ifdef _NJUST_OS_USE_LINUX_
	if (_gReceiveData.Initialize())
	{
		if (_gDoDebug)
		{
			printf("XFL_ReceiveData Thread Starts!\n");
		}

	}
	_gReceiveData.ReceiveDataLoop();
    #endif
	return arg;
}

void *PreProcThrd(void *arg)
{
    #ifdef _NJUST_OS_USE_LINUX_
	if (_gPreProc.Initialize(_gpNavID))
	{
		if (_gDoDebug)
		{
			printf("XFL_Lidar64PreProc Thread Starts!\n");
		}
	}
	_gPreProc.DataTransformLoop();
    #endif
	return arg;
}

void *ProcFrameThrd(void *arg)
{
    #ifdef _NJUST_OS_USE_LINUX_
	if (_gpProcFrame.Initialize(_gpNavID))
	{
		if (_gDoDebug)
		{
			printf("XFL_ProcFrame Thread Starts!\n");
		}
	}
	_gpProcFrame.ProcFrameLoop();
    #endif
	return arg;
}

void StartUdpCommunication(unsigned int *pNavID)
{   //创建三个线程
	//------------------传递navID指针--------------------------//
	_gpNavID = pNavID;
 	//------------------数据接收线程---------------------------//
	#ifdef _NJUST_OS_USE_LINUX_
	int err;
	err = pthread_create(&gThread_ReceiveData,NULL,ReceivingThrd,NULL);
	if (!err)
	{
		if (_gDoDebug)
		{
			printf("Create RevLidar thread is successful!\n");
		}
	}
    #endif
	//------------------数据预处理线程-------------------------//
	#ifdef _NJUST_OS_USE_LINUX_
	err = pthread_create(&gThread_PreProc,NULL,PreProcThrd,NULL);
	if (!err)
	{
		if (_gDoDebug)
		{
			printf("Create PreProc thread is successful!\n");
		}
	}
    #endif
	//------------------数据处理线程---------------------------//
	#ifdef _NJUST_OS_USE_LINUX_
	err = pthread_create(&gThread_ProcFrame,NULL,ProcFrameThrd,NULL);
    if (!err)
	{
		if (_gDoDebug)
		{
			printf("Create ProcFrame thread is successful!\n");
		}
	}    
    #endif	
	//------------------返回-----------------------------------//
	return;
}
