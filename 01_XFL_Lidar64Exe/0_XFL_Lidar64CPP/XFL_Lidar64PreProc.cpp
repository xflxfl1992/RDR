#include "NJUST_ALV_BYD_H/ALV_BYD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "XFL_Lidar64PreProc.h"
#include "XFL_Lidar64Extern.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// WINDOWS下需要的全局定义
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _NJUST_OS_USE_WINDOWS_
   #include "XFL_Lidar64ProcFrame.h"
   extern char g_packData[LIDAR64_IP_DATA_SIZE];//windows下的一包数据输入
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 全局对象
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static XFL_Lidar64DataTransform _XFLDataTransform;//栈比较小，放不下这个类，放到全局里来
/////////////////////////////////////////////////////////////////////////////////////
// 
//   XFL_Lidar64PreProc构造析构函数
//
/////////////////////////////////////////////////////////////////////////////////////
XFL_Lidar64PreProc::XFL_Lidar64PreProc()
{
	m_frameNum=0;
	m_pSaveFile=NULL;
}

XFL_Lidar64PreProc::~XFL_Lidar64PreProc()
{
	if (m_pSaveFile) { fclose(m_pSaveFile); m_pSaveFile=NULL; }
}
/////////////////////////////////////////////////////////////////////////////////////
//
//   初始化
//
/////////////////////////////////////////////////////////////////////////////////////	
bool XFL_Lidar64PreProc::Initialize( unsigned int *pNavID)
{
	bool ret;
	int navID;
	//step.0---------------保存惯导数据地址-----------------------//
	#ifdef _NJUST_OS_USE_LINUX_
	pthread_mutex_lock(&gMutex_MC_CALLBACK);
	m_pNavID=pNavID;
	navID=*m_pNavID;
	pthread_mutex_unlock(&gMutex_MC_CALLBACK);
	#endif 
	#ifdef _NJUST_OS_USE_WINDOWS_
	m_pNavID=pNavID;
	navID=*m_pNavID;
	#endif
	m_frameNum=0;
	//step.1---------------创建保存文件---------------------------//
	if (m_pSaveFile!=NULL) { fclose(m_pSaveFile); m_pSaveFile=NULL; }
	if (ISDEBUG)
	{
		if (_USE_NAV_ID_COUNT_)
		{
			sprintf(m_fileName,"bin/3D64_%010d.bin",navID);
		}
		else 
		{
			sprintf(m_fileName,"bin/3D64_%010d.bin",m_frameNum);
		}
		m_pSaveFile = fopen(m_fileName,"w+b");
	}
	//step.2---------------初始化数据转换-------------------------//
	//不同的环境输入不同的参数
	EnvironmentSet();
	ret=_XFLDataTransform.Initialize(m_pCalibrateFileName,
									 &m_ExternalParaMatrix[0][0], //外参矩阵
									 m_isDeletedScanlineLut //抽线表
									 );
	//step.3---------------返回-----------------------------------//
	return ret;
}
/////////////////////////////////////////////////////////////////////////////////////
//
//   雷达数据转换
//
/////////////////////////////////////////////////////////////////////////////////////	
#ifdef _NJUST_OS_USE_LINUX_
bool XFL_Lidar64PreProc::DataTransformLoop()
{	
	bool isFrameReady;
	int nReceived;
	int nTransformed;
	int idx;
	int nBytes,N;

    //清零-临界区
	pthread_mutex_lock(&gMutex_NewFrame);
	_gTotal3DDataFrame=0;
	pthread_mutex_unlock(&gMutex_NewFrame);
	//已经转换的个数
	nTransformed=0;
	//接收到的数据长度
	nBytes = 0;
	//进行转换
	while(true)
	{
		//step.1---------------取新包数据-----------------------------//
		//加锁
		pthread_mutex_lock(&gMutex_NewIPData);	
		//总共接收到的IP包数
		nReceived=_gTotalIpDataReceivedNum;
		//解锁
		pthread_mutex_unlock(&gMutex_NewIPData);
		//判断是否有新包,无新包时,等待
		if (nTransformed==nReceived) //已经是最新了
		{  //按雷达10HZ,100ms/348包=0.3ms,300us/包,所以设计延迟100us.
			usleep(100); 
			continue;
		}
		//step.2---------------取新包数据-----------------------------//
		//加锁
		pthread_mutex_lock(&gMutex_NewIPData);	
		//取数据
		idx=nTransformed % LIDAR64_MAX_IP_DATA_NUM_ONE_FRAME;
  		nBytes = _gBytesIP[idx];
		memcpy(m_pIPData,&(_gOneFrameBuf[idx][0]),nBytes);
		//解锁
		pthread_mutex_unlock(&gMutex_NewIPData);
		//累计
		nTransformed++;
		//step.3---------------转换1包数据----------------------------//
		isFrameReady = _XFLDataTransform.DoNext(m_pIPData,nBytes); 
		//step.4---------------保存雷达原始数据-----------------------//
		SaveLidar64OrgData(m_pIPData,nBytes,isFrameReady);
		//step.5---------------满帧的处理-----------------------------//
		if (isFrameReady)
		{
			//step.5.0----调试－------------//
			m_frameNum++;
			//step.5.1----获取整帧数据------------//
			pData = _XFLDataTransform.GetFrame(&N);
			//step.5.2----加锁传递----------------//
			//加锁
			pthread_mutex_lock(&gMutex_NewFrame);
			//复制
			memcpy(_g3DDataNewFrame,pData,sizeof(RMW_LIDAR_3D_POINT_ORG)*XFL_3D64_MAX_POINT_NUM);
			//个数
			_g3DDataNumInNewFrame=N;
			//累计
			_gTotal3DDataFrame++;
			//解锁
 			pthread_mutex_unlock(&gMutex_NewFrame);
			//step.5.3----从最新的一包数据开始---//
			pthread_mutex_lock(&gMutex_NewIPData);	
		    nTransformed=_gTotalIpDataReceivedNum;
		    pthread_mutex_unlock(&gMutex_NewIPData);
		}
	}
	return isFrameReady;
}
#endif

#ifdef _NJUST_OS_USE_WINDOWS_
bool XFL_Lidar64PreProc::DataTransformLoop()
{	
	bool isFrameReady;
	int nBytes,N;

	//接收到的数据长度
	nBytes = 0;
	//step.1---------------离线条件下取数据-----------------------//
	nBytes = LIDAR64_IP_DATA_SIZE;
	memcpy(m_pIPData,g_packData,nBytes);
	isFrameReady = _XFLDataTransform.DoNext(m_pIPData,nBytes);
	//step.2---------------满帧的处理-----------------------------//
	if (isFrameReady)
	{
		//step.2.1----获取整帧数据------------//
		pData = _XFLDataTransform.GetFrame(&N);
		//step.2.2----传递--------------------//
		_g3DDataNumInNewFrame=N;
		memcpy(_g3DDataNewFrame,pData,sizeof(RMW_LIDAR_3D_POINT_ORG)*N);
		//step.2.3----累计--------------------//
		_gTotal3DDataFrame++;
		//step.2.4----保存结构体--------------//
		SaveLidar64OrgData(pData,N,isFrameReady);
	}
	//step.3----------------返回----------------------------------//
	return isFrameReady;
}

//复位,在Windows下读文件时,每读完一个文件,需要Reset()一下
void XFL_Lidar64PreProc::Reset()
{
	_XFLDataTransform.Reset();
}
#endif

/////////////////////////////////////////////////////////////////////////////////////
// 
//   环境参数设置
//
/////////////////////////////////////////////////////////////////////////////////////
void XFL_Lidar64PreProc::EnvironmentSet()
{
	int scanLineID;
	#ifdef _USE_BYD_PARAMETERS_
	//内参文件
	sprintf(m_pCalibrateFileName,"BYD_InternParameters.txt");
	//外参矩阵	
	m_ExternalParaMatrix[0][0] = 1;
	m_ExternalParaMatrix[0][1] = 0;
	m_ExternalParaMatrix[0][2] = 0;
	m_ExternalParaMatrix[0][3] = 0;

	m_ExternalParaMatrix[1][0] = 0;
	m_ExternalParaMatrix[1][1] = 1;
	m_ExternalParaMatrix[1][2] = 0;
	m_ExternalParaMatrix[1][3] = 85;

	m_ExternalParaMatrix[2][0] = 0;
	m_ExternalParaMatrix[2][1] = 0;
	m_ExternalParaMatrix[2][2] = 1;
	m_ExternalParaMatrix[2][3] = 183;
	//抽线表
	for (scanLineID = 0; scanLineID < XFL_3D64_HEAD_NUM; scanLineID++)
	{
		if (//ZYM测试，在BYD上这几条线超边界
			
			/*(scanLineID == 42)||(scanLineID == 43)||(scanLineID == 62)||(scanLineID == 63)||(scanLineID == 56)||
			(scanLineID == 57)||(scanLineID == 52)||(scanLineID == 53)||(scanLineID == 45)||(scanLineID == 60)||
			(scanLineID == 61)||*/
			//XFL抽线，这些线太稠密，造成障碍误报
			(scanLineID == 5)||(scanLineID == 14)||(scanLineID == 4)||(scanLineID == 7)||(scanLineID == 1)||
			(scanLineID == 9)||(scanLineID == 16)||(scanLineID == 10)||(scanLineID == 18)||(scanLineID == 22)||
			(scanLineID == 12)||(scanLineID == 2)||(scanLineID == 20)||(scanLineID == 60)
			//BYD上这些线出现错点
			//||(scanLineID == 49)||(scanLineID == 43)||(scanLineID == 44)
			)
		{
			m_isDeletedScanlineLut[scanLineID] = true;
		}
		else m_isDeletedScanlineLut[scanLineID] = false;
		//test:		
		//if(scanLineID<39) m_isDeletedScanlineLut[scanLineID] = true;
		
	}
	
	#endif
	#ifdef _USE_DM_PARAMETERS_
	//内参文件
	sprintf(m_pCalibrateFileName,"DM_InternParameters.txt");
	//外参矩阵
	m_ExternalParaMatrix[0][0] = 1;
	m_ExternalParaMatrix[0][1] = 0;
	m_ExternalParaMatrix[0][2] = 0;
	m_ExternalParaMatrix[0][3] = 0;

	m_ExternalParaMatrix[1][0] = 0;
	m_ExternalParaMatrix[1][1] = 1;
	m_ExternalParaMatrix[1][2] = 0;
	m_ExternalParaMatrix[1][3] = 70;

	m_ExternalParaMatrix[2][0] = 0;
	m_ExternalParaMatrix[2][1] = 0;
	m_ExternalParaMatrix[2][2] = 1;
	m_ExternalParaMatrix[2][3] = 224;
	//抽线表
	for (scanLineID = 0; scanLineID < XFL_3D64_HEAD_NUM; scanLineID++)
	{
		if (//XFL抽线，这些线太稠密，造成障碍误报
			(scanLineID == 0)||(scanLineID == 2)||(scanLineID == 4)||(scanLineID == 5)||(scanLineID == 7)||
			(scanLineID == 8)||(scanLineID == 10)||(scanLineID == 11)||(scanLineID == 17)||(scanLineID == 1)||
			(scanLineID == 13)||(scanLineID == 15)||(scanLineID == 19)||(scanLineID == 23)||(scanLineID == 25)||
			(scanLineID == 27)||(scanLineID == 35)||(scanLineID == 46)
			)
		{
			m_isDeletedScanlineLut[scanLineID] = true;
		}
		else m_isDeletedScanlineLut[scanLineID] = false;
	}
	#endif

	#ifdef _USE_MMR_PARAMETERS_
	//内参文件
	sprintf(m_pCalibrateFileName,"MMR_InternParameters.txt");
	//sprintf(m_pCalibrateFileName,"DM_InternParameters.txt");
	//外参矩阵
	m_ExternalParaMatrix[0][0] = 1;
	m_ExternalParaMatrix[0][1] = 0;
	m_ExternalParaMatrix[0][2] = 0;
	m_ExternalParaMatrix[0][3] = 0;

	m_ExternalParaMatrix[1][0] = 0;
	m_ExternalParaMatrix[1][1] = 1;
	m_ExternalParaMatrix[1][2] = 0;
	m_ExternalParaMatrix[1][3] = 136;

	m_ExternalParaMatrix[2][0] = 0;
	m_ExternalParaMatrix[2][1] = 0;
	m_ExternalParaMatrix[2][2] = 1;
	m_ExternalParaMatrix[2][3] = 154;
	//抽线表
	
	for (scanLineID = 0; scanLineID < XFL_3D64_HEAD_NUM; scanLineID++)
	{
		if (//XFL抽线，这些线太稠密，造成障碍误报
			(scanLineID == 0)||(scanLineID == 2)||(scanLineID == 4)||(scanLineID == 5)||(scanLineID == 7)||
			(scanLineID == 8)||(scanLineID == 10)||(scanLineID == 11)||(scanLineID == 17)||(scanLineID == 1)||
			(scanLineID == 13)||(scanLineID == 15)||(scanLineID == 18)||(scanLineID == 19)||(scanLineID == 21)||
			(scanLineID == 23)||(scanLineID == 25)||(scanLineID == 33)
			)
		{
			m_isDeletedScanlineLut[scanLineID] = true;
		}
		else m_isDeletedScanlineLut[scanLineID] = false;
	}

	#endif


}
/////////////////////////////////////////////////////////////////////////////////////
// 
//   雷达数据保存
//
/////////////////////////////////////////////////////////////////////////////////////
#ifdef _NJUST_OS_USE_LINUX_
void XFL_Lidar64PreProc::SaveLidar64OrgData(char *pData,int nData,bool isFrameReady)
{
	//step.1---------------保存1包数据----------------------------//
	if (m_pSaveFile!=NULL)
	{	//记录雷达原始数据	
		#ifdef NJUST_IP_COMM_USE_OLD_BEFORE_20160826
		if (!NJUST_IP_IsMemAvailable("/home"))
		#endif
		{ 
			fwrite(pData,1,nData,m_pSaveFile);
		}
	}
	//step.2---------------创建新文件,在满一帧时-----------------//
	if (isFrameReady)
	{
		//先关闭
		if (m_pSaveFile!=NULL)
		{
			fclose(m_pSaveFile);
			m_pSaveFile=NULL;
		}
		//再打开
		if (ISDEBUG)
		{
			if(_USE_NAV_ID_COUNT_)
			{
				int navID;
				pthread_mutex_lock(&gMutex_MC_CALLBACK);
				navID=*m_pNavID;
				pthread_mutex_unlock(&gMutex_MC_CALLBACK);
				sprintf(m_fileName,"bin/3D64_%010d.bin",navID);
			}
			else 	
			{
				sprintf(m_fileName,"bin/3D64_%010d.bin",m_frameNum);
			}

			#ifdef NJUST_IP_COMM_USE_OLD_BEFORE_20160826
			if (!NJUST_IP_IsMemAvailable("/home"))
			#endif
				m_pSaveFile = fopen(m_fileName,"w+b");
		}
	}
	//step.3---------------返回----------------------------------//
	return;
}
#endif
#ifdef _NJUST_OS_USE_WINDOWS_
void XFL_Lidar64PreProc::SaveLidar64OrgData(RMW_LIDAR_3D_POINT_ORG *pData,int nData,bool isFrameReady)
{
	int navID;

	//step.1---------------保存1帧数据----------------------------//
	if (m_pSaveFile!=NULL)
	{	//记录雷达原始数据	
		fwrite(pData,sizeof(RMW_LIDAR_3D_POINT_ORG),nData,m_pSaveFile);
	}
	//step.2---------------创建新文件,在满一帧时-----------------//
	if (isFrameReady)
	{
		//先关闭
		if (m_pSaveFile!=NULL)
		{
			fclose(m_pSaveFile);
			m_pSaveFile=NULL;
		}
		//再打开
		if (ISDEBUG)
		{
			navID=*m_pNavID+1;
			sprintf(m_fileName,"bin/3D64_%010d.bin",navID);
			m_pSaveFile = fopen(m_fileName,"w+b");
		}
	}
	//step.3---------------返回----------------------------------//
	return;
}
#endif