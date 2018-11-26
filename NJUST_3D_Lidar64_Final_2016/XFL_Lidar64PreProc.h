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
	//��ʼ��
    bool Initialize( unsigned int *pNavID);
	//�״�����ת��
    bool DataTransformLoop();
	//��λ,��Windows�¶��ļ�ʱ,ÿ����һ���ļ�,��ҪReset()һ��
    #ifdef _NJUST_OS_USE_WINDOWS_
	void Reset();
    #endif

private:
	//��������
	void EnvironmentSet();
	//�״����ݱ���
	#ifdef _NJUST_OS_USE_LINUX_
    void SaveLidar64OrgData(char *pData,int nData,bool isFrameReady);
	#endif
	#ifdef _NJUST_OS_USE_WINDOWS_	
	void SaveLidar64OrgData(RMW_LIDAR_3D_POINT_ORG *pData,int nData,bool isFrameReady);
	#endif
private:
	//�ߵ����ݵĵ�ַ
	unsigned int *m_pNavID;
	//ԭʼ���ݻ�����
	char m_pIPData[LIDAR64_MAX_IP_DATA_SIZE]; 	
	//��ָ��
	RMW_LIDAR_3D_POINT_ORG *pData;//���������ݱ���������
	//��ͬ����ѡ���µĲ���
	char m_pCalibrateFileName[100];
	int m_ExternalParaMatrix[3][4];
	bool m_isDeletedScanlineLut[XFL_3D64_HEAD_NUM];
	//����
	int m_frameNum;
	FILE *m_pSaveFile;
	char m_fileName[256];
};

#endif
