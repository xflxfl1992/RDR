#ifndef XFL_LIDAR64_PRE_PROC_H
#define XFL_LIDAR64_PRE_PROC_H

#include "NJUST_ALV_BYD_H/ALV_BYD.h"
#include "XFL_Lidar64Def.h" 

#include "XFL_Lidar64DataTransform.h" 
 
class XFL_Lidar64PreProc
{

public:
	XFL_Lidar64PreProc();
	~XFL_Lidar64PreProc();

public:	
	//初始化
    bool Initialize( unsigned int *pNavID);
	//雷达数据转换
    bool DataTransformLoop();
	//复位,在Windows下读文件时,每读完一个文件,需要Reset()一下
    #ifdef _NJUST_OS_USE_WINDOWS_
	void Reset();
    #endif

private:
	//环境设置
	void EnvironmentSet();
	//雷达数据保存
	#ifdef _NJUST_OS_USE_LINUX_
    void SaveLidar64OrgData(char *pData,int nData,bool isFrameReady);
	#endif
	#ifdef _NJUST_OS_USE_WINDOWS_	
	void SaveLidar64OrgData(RMW_LIDAR_3D_POINT_ORG *pData,int nData,bool isFrameReady);
	#endif
private:
	//惯导数据的地址
	unsigned int *m_pNavID;
	//原始数据缓冲区
	char m_pIPData[LIDAR64_MAX_IP_DATA_SIZE]; 	
	//虚指针
	RMW_LIDAR_3D_POINT_ORG *pData;//解码后的数据保存在这里
	//不同环境选择下的参数
	char m_pCalibrateFileName[100];
	int m_ExternalParaMatrix[3][4];
	bool m_isDeletedScanlineLut[XFL_3D64_HEAD_NUM];
	//调试
	int m_frameNum;
	FILE *m_pSaveFile;
	char m_fileName[256];
};

#endif
