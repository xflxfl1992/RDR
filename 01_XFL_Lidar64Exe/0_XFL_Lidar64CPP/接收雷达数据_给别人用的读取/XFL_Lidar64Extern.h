#ifndef XFL_LIDAR64_EXTERN_H
#define XFL_LIDAR64_EXTERN_H
 
#include "XFL_Lidar64Def.h"
#include "./RmwLidarProcLib/RmwLidarStruct.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 全局变量
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _NJUST_OS_USE_LINUX_
   extern bool _gDoDebug; //是否打印调试信息
#endif
extern unsigned int gMcID ;
extern unsigned int gNavIDStart ;
extern unsigned int gNavIDEnd ;
extern NJUST_MC_NAV_INFO	gNavInfo;
extern int gPL_CMD;//PL发来的前进后退命令
extern bool gIsObjDetect;//切换侦查状态
extern bool gUseNegativeObsDetect;//是否要使用负障碍，默认为true

extern int _gBytesIP[LIDAR64_MAX_IP_DATA_NUM_ONE_FRAME];   //记录每个IPData的字节数
extern char _gOneFrameBuf[LIDAR64_MAX_IP_DATA_NUM_ONE_FRAME][LIDAR64_MAX_IP_DATA_SIZE];//一帧数据,按348网络包,每包一行存.
extern int _gTotalIpDataReceivedNum; //总共接收到的IP包数
extern RMW_LIDAR_3D_POINT_ORG _g3DDataNewFrame[XFL_3D64_MAX_POINT_NUM];//当前新帧数据
extern int _g3DDataNumInNewFrame;//当前新帧数据的个数
extern int _gTotal3DDataFrame; //总共生成的新帧数
 ////////////////////////////////////////////////////////////////////////////////////////////////////
//
// LINUX下的锁和信号量
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _NJUST_OS_USE_LINUX_
   extern pthread_mutex_t gMutex_MC_CALLBACK;
   extern pthread_mutex_t gMutex_PL_CALLBACK;
   extern pthread_mutex_t gMutex_MO_CALLBACK;
   extern pthread_mutex_t gMutex_NewIPData;
   extern pthread_mutex_t gMutex_NewFrame;
#endif


#endif
