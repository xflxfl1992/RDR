#ifndef XFL_LIDAR64_EXTERN_H
#define XFL_LIDAR64_EXTERN_H
 
#include "XFL_Lidar64Def.h"
#include "./RmwLidarProcLib/RmwLidarStruct.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ȫ�ֱ���
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _NJUST_OS_USE_LINUX_
   extern bool _gDoDebug; //�Ƿ��ӡ������Ϣ
#endif
extern unsigned int gMcID ;
extern unsigned int gNavIDStart ;
extern unsigned int gNavIDEnd ;
extern NJUST_MC_NAV_INFO	gNavInfo;
extern int gPL_CMD;//PL������ǰ����������
extern bool gIsObjDetect;//�л����״̬
extern bool gUseNegativeObsDetect;//�Ƿ�Ҫʹ�ø��ϰ���Ĭ��Ϊtrue

extern int _gBytesIP[LIDAR64_MAX_IP_DATA_NUM_ONE_FRAME];   //��¼ÿ��IPData���ֽ���
extern char _gOneFrameBuf[LIDAR64_MAX_IP_DATA_NUM_ONE_FRAME][LIDAR64_MAX_IP_DATA_SIZE];//һ֡����,��348�����,ÿ��һ�д�.
extern int _gTotalIpDataReceivedNum; //�ܹ����յ���IP����
extern RMW_LIDAR_3D_POINT_ORG _g3DDataNewFrame[XFL_3D64_MAX_POINT_NUM];//��ǰ��֡����
extern int _g3DDataNumInNewFrame;//��ǰ��֡���ݵĸ���
extern int _gTotal3DDataFrame; //�ܹ����ɵ���֡��
 ////////////////////////////////////////////////////////////////////////////////////////////////////
//
// LINUX�µ������ź���
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
