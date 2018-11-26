#include "NJUST_ALV_BYD_H/ALV_BYD.h"

#include <stdio.h>
#include <fstream>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////
// 
//
//   执行平台--LINUX
//
//
/////////////////////////////////////////////////////////////////////////////////////
#ifdef _NJUST_OS_USE_LINUX_

#include "XFL_Lidar64Def.h"

#define LOCAL_MODEL "3D64" //本地模块


/////////////////////////////////////////////////////////////////////////////////////
// 
//
//   线程定义
//
//
/////////////////////////////////////////////////////////////////////////////////////
extern pthread_t gThread_ReceiveData;//  create a thread to receive the udp data
extern pthread_t gThread_PreProc; // create a thread to pre-process data;
extern pthread_t gThread_ProcFrame;//create a thread to process one whole frame;
extern void StartUdpCommunication(unsigned int *pNavID);

//---------------------全局变量定义，用于接收数据----------------//
unsigned int gMcID = 0;
unsigned int gNavIDStart = 0;
unsigned int gNavIDEnd = 1;
static unsigned int gNavID = 0;  
static bool isFirst = true;
NJUST_MC_NAV_INFO	gNavInfo;
int gPL_CMD = 0;//PL发来的前进后退命令
bool gIsObjDetect = false;
bool gUseNegativeObsDetect = true;
//----------------------------------------------------------------//

static char timebuf[24];
bool _gDoDebug = true;
pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gMutex_MC_CALLBACK = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gMutex_PL_CALLBACK = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gMutex_MO_CALLBACK = PTHREAD_MUTEX_INITIALIZER;



//---------------------获取系统时间------------------------------//
char* LocalModel_GetTime()
{
	NJUST_IP_TIME time = NJUST_IP_get_time();
	NJUST_IP_get_timeStr(time,timebuf);
	return timebuf;
}
//---------------------------------------------------------------//


//-------------------------获取_gDoDebug的状态--------------------//
bool IsDebug()
{
	bool b;
	pthread_mutex_lock(&gMutex);
	b = _gDoDebug;
	pthread_mutex_unlock(&gMutex);
	return b;
}
//---------------------------------------------------------------//

//------------------------将_gDoDebug置为true---------------------//
void SetDebug()
{
	pthread_mutex_lock(&gMutex);
	_gDoDebug = true;
	pthread_mutex_unlock(&gMutex);
}
//----------------------------------------------------------------//

//------------------------将_gDoDebug置为false---------------------//
void SetRelease()
{
	pthread_mutex_lock(&gMutex);
	_gDoDebug = false;
	pthread_mutex_unlock(&gMutex);
}
//-----------------------------------------------------------------//


////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  NJUST_IP_COMM通信库选择-旧库
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef NJUST_IP_COMM_USE_OLD_BEFORE_20160826

//-------------------MC回调函数------------------------------------//
int MCCallBack(void* mc_to_3D,size_t size,void* args)
{
	NJUST_MC_STATE_INFO *pState;
	NJUST_MC_NAV_INFO	*pNav;
	NJUST_MC_DRIVE_INFO	*pDrive;
	NJUST_MC_Decode_IP_Data(mc_to_3D,size,&pState,&pNav,&pDrive);
	if (pNav)
	{
		pthread_mutex_lock(&gMutex_MC_CALLBACK);
		gNavID = pNav->navID;
		if (isFirst)
		{
			gNavIDStart = pNav->navID;
			gNavIDEnd = pNav->navID;
			isFirst = false;
		}
		else
		{
			gNavIDEnd = pNav->navID;
		}
		gMcID++;
		gNavInfo = *pNav;
		pthread_mutex_unlock(&gMutex_MC_CALLBACK);
	}
	return 0;
}

//-----------------------------------------------------------------//


//----------------------MO回调函数---------------------------------//
int MOCallBack( void* mo_to_3D, size_t size, void* args)
{
	if (IsDebug()) printf("%s:3D -----Receive MO TO 3D Command form MO!\n",LocalModel_GetTime());
	NJUST_FROM_MO_COMMAND *pCmd=NULL;
	NJUST_FROM_MO_CFG *pCfg=NULL;
	NJUST_MO_Decode_IP_Data_CMD(mo_to_3D,size,&pCmd,&pCfg);
	if(pCmd!=NULL)
	{
		if (pCmd->cmd ==  NJUST_MO_COMMAND_TYPE_COMPUTER_RESTART)
		{
			if (IsDebug()) printf("%s:3D Receive Restart Command form MO!\n",LocalModel_GetTime());
			NJUST_IP_req_pc_reboot();
		}
		else if (pCmd->cmd == NJUST_MO_COMMAND_TYPE_COMPUTER_SHUTDOWN)
		{
			if (IsDebug()) printf("%s:3D Receive Shutdown Command form MO!\n",LocalModel_GetTime());
			NJUST_IP_req_pc_poweroff();
		}
		else if (pCmd->cmd == NJUST_MO_COMMAND_TYPE_MODULE_RESTART)
		{
			if (IsDebug()) printf("%s:3D Receive Shutdown Command form MO!\n",LocalModel_GetTime());
			NJUST_IP_req_mod_reboot();
		}
		else if (pCmd->cmd == NJUST_MO_COMMAND_TYPE_MODULE_DEBUG)
		{
			if (IsDebug()) printf("%s:3D Receive Debug Command form MO!\n",LocalModel_GetTime());
			pthread_mutex_lock(&gMutex_MO_CALLBACK);
			gIsObjDetect = true;
			pthread_mutex_unlock(&gMutex_MO_CALLBACK);
		}
		else if (pCmd->cmd == NJUST_MO_COMMAND_TYPE_MODULE_RELEASE)
		{
			if (IsDebug()) printf("%s:3D Receive Release Command form MO!\n",LocalModel_GetTime());
			pthread_mutex_lock(&gMutex_MO_CALLBACK);
			gUseNegativeObsDetect = false;
			pthread_mutex_unlock(&gMutex_MO_CALLBACK);
		}
	}
	if(pCfg!=NULL)
	{

	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////
// 
//   接收PLCallBack函数(PL回调函数)
//
/////////////////////////////////////////////////////////////////////////////////////
int PLCallBack(void* PL_to_3D,size_t size,void* args)
{
	int cmd=0;
	memcpy(&cmd,(void*)PL_to_3D,size);
	if (cmd)
	{
		pthread_mutex_lock(&gMutex_PL_CALLBACK);
		gPL_CMD = cmd;
		pthread_mutex_unlock(&gMutex_PL_CALLBACK);
	}
	return 0;
}
//-----------------------------------------------------------------------------------//

int main()
{

	//------------------step1.在守护进程中注册当前模块---------------------------//
	int ret;
	ret = NJUST_IP_set_moduleName(LOCAL_MODEL,1);//是否托管
	if (ret == -1)
	{
		if (IsDebug())
		{
			printf("3D cannot create!\n");
		}

		return 0;
	}

	//------------------step2.切换版本------------------------------------------//
	SetRelease(); 
	//------------------step3.对守护进程声明callBack------------------------------//
	if (NJUST_IP_set_tcp_callBack(MO_MODEL,MOCallBack,NULL)==-1)
	{
		if (IsDebug())
		{
			printf("Cannot set MOCallback!\n");
		}
		return 0;
	}
	if (NJUST_IP_set_broadcast_callBack(MCCallBack,NULL)==-1)
	{
		if (IsDebug())
		{
			printf("Cannot set MCCallback!\n");
		}
		return 0;
	}
	if(NJUST_IP_set_tcp_callBack(PL_MODEL,PLCallBack,NULL)==-1)
	{
		if (IsDebug())
		{
			printf("Cannot set PLCallback!\n");
		}
		return 0;
	}
	//----------------step4.创建线程----------------------------------------------//
	StartUdpCommunication(&gNavID);
	//printf("--------------------start-----------------------------------\n");
	//----------------step5.主线程会一直等待直到等待的线程结束自己才结束----------//
	pthread_join(gThread_ReceiveData,NULL);
	pthread_join(gThread_PreProc,NULL);
	pthread_join(gThread_ProcFrame,NULL);
	//----------------step6.返回--------------------------------------------------//
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  NJUST_IP_COMM通信库选择-新库
//
////////////////////////////////////////////////////////////////////////////////////////////////
#else
//-------------------MC回调函数------------------------------------//
i32 MCCallBack(void* mc_to_3D,u32 size,void* args)
{
	NJUST_MC_STATE_INFO *pState;
	NJUST_MC_NAV_INFO	*pNav;
	NJUST_MC_DRIVE_INFO	*pDrive;
	NJUST_MC_Decode_IP_Data(mc_to_3D,size,&pState,&pNav,&pDrive);
	if (pNav)
	{
		pthread_mutex_lock(&gMutex_MC_CALLBACK);
		gNavID = pNav->navID;
		if (isFirst)
		{
			gNavIDStart = pNav->navID;
			gNavIDEnd = pNav->navID;
			isFirst = false;
		}
		else
		{
			gNavIDEnd = pNav->navID;
		}
		gMcID++;
		gNavInfo = *pNav;
		pthread_mutex_unlock(&gMutex_MC_CALLBACK);
	}
	return 0;
}

//-----------------------------------------------------------------//


//----------------------MO回调函数---------------------------------//
i32 MOCallBack( void* mo_to_3D, u32 size, void* args)
{
	if (IsDebug()) printf("%s:3D -----Receive MO TO 3D Command form MO!\n",LocalModel_GetTime());
	NJUST_FROM_MO_COMMAND *pCmd=NULL;
	NJUST_FROM_MO_CFG *pCfg=NULL;
	NJUST_MO_Decode_IP_Data_CMD(mo_to_3D,size,&pCmd,&pCfg);
	if(pCmd!=NULL)
	{
		if (pCmd->cmd ==  NJUST_MO_COMMAND_TYPE_COMPUTER_RESTART)
		{
			if (IsDebug()) printf("%s:3D Receive Restart Command form MO!\n",LocalModel_GetTime());
			NJUST_IP_req_pc_reboot();
		}
		else if (pCmd->cmd == NJUST_MO_COMMAND_TYPE_COMPUTER_SHUTDOWN)
		{
			if (IsDebug()) printf("%s:3D Receive Shutdown Command form MO!\n",LocalModel_GetTime());
			NJUST_IP_req_pc_poweroff();
		}
		else if (pCmd->cmd == NJUST_MO_COMMAND_TYPE_MODULE_RESTART)
		{
			if (IsDebug()) printf("%s:3D Receive Shutdown Command form MO!\n",LocalModel_GetTime());
			NJUST_IP_req_mod_reboot();
		}
		else if (pCmd->cmd == NJUST_MO_COMMAND_TYPE_MODULE_DEBUG)
		{
			if (IsDebug()) printf("%s:3D Receive Debug Command form MO!\n",LocalModel_GetTime());
			pthread_mutex_lock(&gMutex_MO_CALLBACK);
			gIsObjDetect = true;
			pthread_mutex_unlock(&gMutex_MO_CALLBACK);
		}
		else if (pCmd->cmd == NJUST_MO_COMMAND_TYPE_MODULE_RELEASE)
		{
			if (IsDebug()) printf("%s:3D Receive Release Command form MO!\n",LocalModel_GetTime());
			pthread_mutex_lock(&gMutex_MO_CALLBACK);
			gUseNegativeObsDetect = false;
			pthread_mutex_unlock(&gMutex_MO_CALLBACK);
		}
	}
	if(pCfg!=NULL)
	{

	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////
// 
//   接收PLCallBack函数(PL回调函数)
//
/////////////////////////////////////////////////////////////////////////////////////
i32 PLCallBack(void* PL_to_3D,u32 size,void* args)
{
	int cmd=0;
	memcpy(&cmd,(void*)PL_to_3D,size);
	if (cmd)
	{
		pthread_mutex_lock(&gMutex_PL_CALLBACK);
		gPL_CMD = cmd;
		pthread_mutex_unlock(&gMutex_PL_CALLBACK);
	}
	return 0;
}
//-----------------------------------------------------------------------------------//

i32 main()
{

	//------------------step1.在守护进程中注册当前模块---------------------------//
	int ret;
	ret = NJUST_IP_set_moduleName(LOCAL_MODEL,1,200);//是否托管
	if (ret == -1)
	{
		if (IsDebug())
		{
			printf("3D cannot create!\n");
		}

		return 0;
	}

	//------------------step2.切换版本------------------------------------------//
	SetRelease();
	//------------------step3.对守护进程声明callBack------------------------------//
	if (NJUST_IP_set_callBack(MO_MODEL,MOCallBack,NULL)==-1)
	{
		if (IsDebug())
		{
			printf("Cannot set MOCallback!\n");
		}
		return 0;
	}
	if (NJUST_IP_set_brd_callBack(MCCallBack,NULL)==-1)
	{
		if (IsDebug())
		{
			printf("Cannot set MCCallback!\n");
		}
		return 0;
	}
	if(NJUST_IP_set_callBack(PL_MODEL,PLCallBack,NULL)==-1)
	{
		if (IsDebug())
		{
			printf("Cannot set PLCallback!\n");
		}
		return 0;
	}
	//----------------step4.创建线程----------------------------------------------//
	StartUdpCommunication(&gNavID);
	//printf("--------------------start-----------------------------------\n");
	//----------------step5.主线程会一直等待直到等待的线程结束自己才结束----------//
	pthread_join(gThread_ReceiveData,NULL);
	pthread_join(gThread_PreProc,NULL);
	pthread_join(gThread_ProcFrame,NULL);
	//----------------step6.返回--------------------------------------------------//
	return 0;
}

#endif

#endif

 