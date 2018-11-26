#ifndef XFL_LIDAR64_RECEIVE_DATA_H
#define XFL_LIDAR64_RECEIVE_DATA_H

#include "NJUST_ALV_BYD_H/ALV_BYD.h"
#include "XFL_Lidar64Def.h"

//��
class XFL_Lidar64ReceiveData
{
public:
	//����/����
	XFL_Lidar64ReceiveData();
	~XFL_Lidar64ReceiveData();
    //��ʼ��
    bool Initialize();
	//������ѭ��
	void ReceiveDataLoop();
private:
	bool m_isInitedOK;
	int m_sockReceive;
	bool m_isFirstInit;
	char m_pBuf[LIDAR64_MAX_IP_DATA_SIZE];
};
#endif
