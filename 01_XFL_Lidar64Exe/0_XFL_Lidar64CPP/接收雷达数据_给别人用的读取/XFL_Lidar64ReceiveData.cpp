////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//���״�ȡ�õ�ԭʼ���ݣ�ת��Ϊ���õĽṹ�壬���洢���ṹ��������
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "NJUST_ALV_BYD_H/ALV_BYD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "XFL_Lidar64ReceiveData.h"
#include "XFL_Lidar64Extern.h"

char DebugBuff[100];
extern void XFL_DebugFile(int i,char *pStr);
 ////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����/��������
//
////////////////////////////////////////////////////////////////////////////////////////////////////
XFL_Lidar64ReceiveData::XFL_Lidar64ReceiveData()
{  	
	#ifdef _NJUST_OS_USE_LINUX_
	//��ʼ��
	m_isInitedOK=false; //��δ��ʼ��
	m_isFirstInit = true; //�Ƿ��һ�γ�ʼ��
	m_sockReceive = INVALID_SOCKET; //��ЧSOCKET
	//����-�ٽ���
	pthread_mutex_lock(&gMutex_NewIPData);
	_gTotalIpDataReceivedNum=0; //�ܵĽӰ�����
	pthread_mutex_unlock(&gMutex_NewIPData);
	#endif
}
	
XFL_Lidar64ReceiveData::~XFL_Lidar64ReceiveData()
{	
	#ifdef _NJUST_OS_USE_LINUX_
	if (m_sockReceive != INVALID_SOCKET)
	{
		close(m_sockReceive);
		m_sockReceive = INVALID_SOCKET;
	}
	#endif
}
/////////////////////////////////////////////////////////////////////////////////////
//
//   ������������
//
/////////////////////////////////////////////////////////////////////////////////////	
bool XFL_Lidar64ReceiveData::Initialize(void)
{
    #ifdef _NJUST_OS_USE_LINUX_
	sockaddr_in  addr;
	int flag;
	int ret;
	int nZero,nNetTimeout;

	//step.0---------------��ʼ��---------------------------------//
	m_isInitedOK=false;
	//��������ǵ�һ�γ�ʼ�����򱣳�ԭ�ٽ�������
	if(m_isFirstInit)
	{
		//����-�ٽ���
		pthread_mutex_lock(&gMutex_NewIPData);
		_gTotalIpDataReceivedNum=0; //�ܵĽӰ�����
		pthread_mutex_unlock(&gMutex_NewIPData);
		m_isFirstInit = false;
	}
	
	//step.1---------------����һ���׽���-------------------------//
	if (m_sockReceive != INVALID_SOCKET)
	{
		close(m_sockReceive);
		m_sockReceive = INVALID_SOCKET;
	}
	m_sockReceive = socket(AF_INET, SOCK_DGRAM, 0);//UDP
	if (m_sockReceive == INVALID_SOCKET)
	{
		if (_gDoDebug)
		{
			printf("XFL_Lidar64ReceiveData() bind is Err!\n");
		}
		return false;
	}
	//step.2---------------�����׽�������-------------------------//
	flag = 1;
	//�׽ӿ�����Ϊ�������ñ��ص�ַ�Ͷ˿� 
	ret = setsockopt(m_sockReceive,SOL_SOCKET,SO_REUSEADDR,(char *) &flag,sizeof(flag));
	if (ret==SOCKET_ERROR) 
	{
		close(m_sockReceive);
		m_sockReceive = INVALID_SOCKET;
		if (_gDoDebug)
		{
			printf("XFL_Lidar64ReceiveData() bind is Err!\n");
		}
		return false;
	}
	//step.3---------------��IP�Ͷ˿�---------------------------//
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port        = htons(UDP_LIDAR_PORT);
	ret = bind(m_sockReceive, (struct sockaddr*) &addr, sizeof(addr));
	if (!ret) 
	{
		if (_gDoDebug)
		{
			printf("XFL_Lidar64ReceiveData() bind is successful!\n");
		}
	}
	else if (ret == SOCKET_ERROR) 
	{
		close(m_sockReceive);
		m_sockReceive = INVALID_SOCKET;
		return false;
	}
	//step.4---------------�趨��ʱ�ͻ�������С-------------------//
	//ϵͳĬ�ϵ�״̬���ͺͽ���һ��Ϊ8688�ֽ�(ԼΪ8.5K)����ʵ�ʵĹ����з������ݺͽ����������Ƚϴ󣬿�������socket����������������send(),recv()���ϵ�ѭ���շ�
	//�������ڽ������ݵ�ʱ��ϣ����������socket��������ϵͳ�������Ŀ�����Ӱ���������ܣ��������8.5K����Ҫ���û�����
	nZero=0;
	setsockopt( m_sockReceive,SOL_SOCKET,
		        SO_RCVBUF,
			    (char *)&nZero,
			    sizeof(int)
			  );
	nNetTimeout=80;//���ý���ʱ��,��λ����
	setsockopt( m_sockReceive, 
		        SOL_SOCKET,SO_RCVTIMEO,
				(char *)&nNetTimeout,
				sizeof(int)
			  );
    #endif 
	//step.5---------------����----------------------------------//
	m_isInitedOK=true;
	return m_isInitedOK;
}
/////////////////////////////////////////////////////////////////////////////////////
//
//   �������ݽ���
//
/////////////////////////////////////////////////////////////////////////////////////	
void XFL_Lidar64ReceiveData::ReceiveDataLoop()
{
    #ifdef _NJUST_OS_USE_LINUX_
	struct sockaddr_in  addr_cli;
	unsigned int addr_cli_len;
	int nBytes;
	int idx;
	int SleepTimeNum = 0;//����˯��ʱ�����

 	//����
	while (m_isInitedOK)
	{
		//step.1---------------����һ�������-------------------------//
		addr_cli_len = sizeof(addr_cli);
		nBytes = recvfrom( m_sockReceive, 
			               (char *)m_pBuf,
						   LIDAR64_MAX_IP_DATA_SIZE,
						   0,
						   (struct sockaddr*) &addr_cli,
						   (socklen_t*) &addr_cli_len
						 );
		//step.2---------------��ű�������---------------------------//
		if (nBytes>0)
		{
			//˯�߼�������
			SleepTimeNum = 0;
			//����
			pthread_mutex_lock(&gMutex_NewIPData);
			//����
			idx=_gTotalIpDataReceivedNum % LIDAR64_MAX_IP_DATA_NUM_ONE_FRAME; //%348
			memcpy(&(_gOneFrameBuf[idx][0]),m_pBuf,nBytes);
			_gBytesIP[idx] = nBytes; //�������ֽ���,����1206.
			//�ۼ�
			_gTotalIpDataReceivedNum++;
			//����
			pthread_mutex_unlock(&gMutex_NewIPData);
		}
		//step.3---------------�Ե�,ȡ��һ��--------------------------//
		else //���״�10HZ,100ms/348��=0.3ms,300us/��,��������ӳ�50us.
		{
			usleep(50);
			SleepTimeNum++;
			if (SleepTimeNum>200000)//50*200000us = 10sû�յ����ݾ����³�ʼ��
			{
				m_isInitedOK = false;//���½����ʼ��
			}			
			continue;
		}
	
		//�����������������У����״�Ӳ��������ʱ����Ҫ���³�ʼ��
		if (!m_isInitedOK)
		{
			//��sleep100ms
			usleep(100000);
			//���³�ʼ���˿�
			Initialize();
			//����loop
			ReceiveDataLoop();

		}
	}
	#endif

	return;
}
 
