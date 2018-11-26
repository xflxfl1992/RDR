////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//将雷达取得的原始数据，转化为可用的结构体，并存储到结构体数组中
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
// 构造/析构函数
//
////////////////////////////////////////////////////////////////////////////////////////////////////
XFL_Lidar64ReceiveData::XFL_Lidar64ReceiveData()
{  	
	#ifdef _NJUST_OS_USE_LINUX_
	//初始化
	m_isInitedOK=false; //尚未初始化
	m_isFirstInit = true; //是否第一次初始化
	m_sockReceive = INVALID_SOCKET; //无效SOCKET
	//清零-临界区
	pthread_mutex_lock(&gMutex_NewIPData);
	_gTotalIpDataReceivedNum=0; //总的接包个数
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
//   创建网络连接
//
/////////////////////////////////////////////////////////////////////////////////////	
bool XFL_Lidar64ReceiveData::Initialize(void)
{
    #ifdef _NJUST_OS_USE_LINUX_
	sockaddr_in  addr;
	int flag;
	int ret;
	int nZero,nNetTimeout;

	//step.0---------------初始化---------------------------------//
	m_isInitedOK=false;
	//如果不是是第一次初始化，则保持原临界区计数
	if(m_isFirstInit)
	{
		//清零-临界区
		pthread_mutex_lock(&gMutex_NewIPData);
		_gTotalIpDataReceivedNum=0; //总的接包个数
		pthread_mutex_unlock(&gMutex_NewIPData);
		m_isFirstInit = false;
	}
	
	//step.1---------------创建一个套接字-------------------------//
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
	//step.2---------------设置套接字属性-------------------------//
	flag = 1;
	//套接口设置为允许重用本地地址和端口 
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
	//step.3---------------绑定IP和端口---------------------------//
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
	//step.4---------------设定超时和缓冲区大小-------------------//
	//系统默认的状态发送和接收一次为8688字节(约为8.5K)；在实际的过程中发送数据和接收数据量比较大，可以设置socket缓冲区，而避免了send(),recv()不断的循环收发
	//本程序在接收数据的时，希望不经历由socket缓冲区到系统缓冲区的拷贝而影响程序的性能，如果超过8.5K，需要设置缓冲区
	nZero=0;
	setsockopt( m_sockReceive,SOL_SOCKET,
		        SO_RCVBUF,
			    (char *)&nZero,
			    sizeof(int)
			  );
	nNetTimeout=80;//设置接收时限,单位毫秒
	setsockopt( m_sockReceive, 
		        SOL_SOCKET,SO_RCVTIMEO,
				(char *)&nNetTimeout,
				sizeof(int)
			  );
    #endif 
	//step.5---------------返回----------------------------------//
	m_isInitedOK=true;
	return m_isInitedOK;
}
/////////////////////////////////////////////////////////////////////////////////////
//
//   网络数据接收
//
/////////////////////////////////////////////////////////////////////////////////////	
void XFL_Lidar64ReceiveData::ReceiveDataLoop()
{
    #ifdef _NJUST_OS_USE_LINUX_
	struct sockaddr_in  addr_cli;
	unsigned int addr_cli_len;
	int nBytes;
	int idx;
	int SleepTimeNum = 0;//连续睡眠时间计数

 	//接收
	while (m_isInitedOK)
	{
		//step.1---------------接收一个网络包-------------------------//
		addr_cli_len = sizeof(addr_cli);
		nBytes = recvfrom( m_sockReceive, 
			               (char *)m_pBuf,
						   LIDAR64_MAX_IP_DATA_SIZE,
						   0,
						   (struct sockaddr*) &addr_cli,
						   (socklen_t*) &addr_cli_len
						 );
		//step.2---------------存放本包数据---------------------------//
		if (nBytes>0)
		{
			//睡眠计数清零
			SleepTimeNum = 0;
			//加锁
			pthread_mutex_lock(&gMutex_NewIPData);
			//复制
			idx=_gTotalIpDataReceivedNum % LIDAR64_MAX_IP_DATA_NUM_ONE_FRAME; //%348
			memcpy(&(_gOneFrameBuf[idx][0]),m_pBuf,nBytes);
			_gBytesIP[idx] = nBytes; //本包的字节数,都是1206.
			//累计
			_gTotalIpDataReceivedNum++;
			//解锁
			pthread_mutex_unlock(&gMutex_NewIPData);
		}
		//step.3---------------稍等,取下一包--------------------------//
		else //按雷达10HZ,100ms/348包=0.3ms,300us/包,所以设计延迟50us.
		{
			usleep(50);
			SleepTimeNum++;
			if (SleepTimeNum>200000)//50*200000us = 10s没收到数据就重新初始化
			{
				m_isInitedOK = false;//重新进入初始化
			}			
			continue;
		}
	
		//保护。当程序运行中，但雷达硬件被重启时，需要重新初始化
		if (!m_isInitedOK)
		{
			//先sleep100ms
			usleep(100000);
			//重新初始化端口
			Initialize();
			//进入loop
			ReceiveDataLoop();

		}
	}
	#endif

	return;
}
 
