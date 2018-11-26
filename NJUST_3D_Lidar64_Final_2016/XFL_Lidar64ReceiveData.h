#ifndef XFL_LIDAR64_RECEIVE_DATA_H
#define XFL_LIDAR64_RECEIVE_DATA_H

#include "NJUST_ALV_BYD_H/ALV_BYD.h"
#include "XFL_Lidar64Def.h"

//类
class XFL_Lidar64ReceiveData
{
public:
	//构造/析构
	XFL_Lidar64ReceiveData();
	~XFL_Lidar64ReceiveData();
    //初始化
    bool Initialize();
	//接收死循环
	void ReceiveDataLoop();
private:
	bool m_isInitedOK;
	int m_sockReceive;
	bool m_isFirstInit;
	char m_pBuf[LIDAR64_MAX_IP_DATA_SIZE];
};
#endif
