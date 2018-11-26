#include "NJUST_ALV_BYD_H/ALV_BYD.h"

#include <memory.h>
#include <stdio.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////////
//
//
//---------------------------OBS-------------------------------------------//
//
//
/////////////////////////////////////////////////////////////////////////////
int NJUST_3D64_Encode_Compress_OBS(NJUST_3D64_OBS_TO_FU *p3D64Obs, unsigned char *p3D64Buf ,int maxResBufSize)
{
	int DataType=0;//表示该包为OBS包
	//step1.-----------------------指针空处理--------------------------//
	if (!p3D64Obs||!p3D64Buf)
	{
		return 0; 
	}
	//step.1---------------------初始化-----------------------------//	
	int i;
	int nBytes=0;
	memset(p3D64Buf,0,maxResBufSize*sizeof(BYTE));
	//step.2---------------------DataType---------------------------//
	memcpy(p3D64Buf+nBytes,&DataType,sizeof(int));
	nBytes += sizeof(int);
	//step.3---------------------frameID----------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Obs->frameID,sizeof(int));
	nBytes += sizeof(int);
	//step.4---------------------synTime----------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Obs->synTime,sizeof(NJUST_IP_TIME));
	nBytes += sizeof(NJUST_IP_TIME);
	//step.5---------------------navID------------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Obs->navID,sizeof(unsigned int));
	nBytes += sizeof(unsigned int );
	//step.6---------------------nObj-------------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Obs->nObj,sizeof(int));
	nBytes += sizeof(int);
	//step.7---------------------pObj[i]----------------------------//
	for(i=0;i<p3D64Obs->nObj;i++)
	{
		memcpy(p3D64Buf+nBytes,&p3D64Obs->pObj[i],sizeof(NJUST_3D64_OBS_DATA));
		nBytes += sizeof(NJUST_3D64_OBS_DATA);
	}
	//step.8---------------------nSize------------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Obs->nSize,sizeof(int));
	nBytes += sizeof(int);
	//step.9---------------------checksum---------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Obs->checksum,sizeof(unsigned char));
	nBytes += sizeof(unsigned char);
	//step.10--------------------返回-------------------------------//
	return nBytes;
}

/////////////////////////////////////////////////////////////////////////////
//
//
//---------------------------Boundary--------------------------------------//
//
//
/////////////////////////////////////////////////////////////////////////////
int NJUST_3D64_Encode_Compress_Boundary(NJUST_3D64_ROAD_BOUNDARY_TO_FU *p3D64Boundary, unsigned char *p3D64Buf, int maxResBufSize)
{
	int DataType=1;//表示该包为Boundary包
	//step.1---------------------指针空处理--------------------------//
	if (!p3D64Boundary||!p3D64Buf)
	{
		return 0; 
	}
	//step.2----------------------初始化-----------------------------//	
	int i;
	int nBytes=0;
	memset(p3D64Buf,0,maxResBufSize*sizeof(BYTE));
	//step.3-----------------------DataType-------------------------//
	memcpy(p3D64Buf+nBytes,&DataType,sizeof(int));
	nBytes += sizeof(int);
	//step.4-----------------------frameID--------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Boundary->frameID,sizeof(int));
	nBytes += sizeof(int);
	//step.5-----------------------synTime--------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Boundary->synTime,sizeof(NJUST_IP_TIME));
	nBytes += sizeof(NJUST_IP_TIME);
	//step.6-----------------------navID----------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Boundary->navID,sizeof(unsigned int));
	nBytes += sizeof(unsigned int );
	//step.7-----------------------nPoint---------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Boundary->nPoint,sizeof(int));
	nBytes += sizeof(int);
	//step.8-----------------------Point[i]-------------------------//
	for(i=0;i<p3D64Boundary->nPoint;i++)
	{
		memcpy(p3D64Buf+nBytes,&p3D64Boundary->Point[i],sizeof(NJUST_3D64_POINT_2D));
		nBytes += sizeof(NJUST_3D64_POINT_2D);
	}
	//step.9-----------------------Type[i]--------------------------//
	for(i=0;i<p3D64Boundary->nPoint;i++)
	{
		memcpy(p3D64Buf+nBytes,&p3D64Boundary->Type[i],sizeof(NJUST_3D64_ROAD_POINT_TYPE));
		nBytes += sizeof(NJUST_3D64_ROAD_POINT_TYPE);
	}
	//step.10----------------------nSize----------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Boundary->nSize,sizeof(int));
	nBytes += sizeof(int);
	//step.11----------------------checksum-------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Boundary->checksum,sizeof(unsigned char));
	nBytes += sizeof(unsigned char);
	//step.12----------------------判断-----------------------------//
	return nBytes;
}

/////////////////////////////////////////////////////////////////////////////
//
//
//---------------------------Grid------------------------------------------//
//
//
/////////////////////////////////////////////////////////////////////////////
int NJUST_3D64_Encode_Compress_Grid(NJUST_3D64_GRID_TO_FU *p3D64Grid, unsigned char *p3D64Buf ,int maxResBufSize)
{
	int DataType=2;//表示该包为Grid包
	//step.1--------------------指针空处理--------------------------//
	if (!p3D64Grid||!p3D64Buf)
	{
		return 0; 
	}
	//step.2--------------------初始化------------------------------//	
	int i;
	int nBytes=0;
	memset(p3D64Buf,0,maxResBufSize*sizeof(BYTE));
	//step.3--------------------DataType----------------------------//
	memcpy(p3D64Buf+nBytes,&DataType,sizeof(int));
	nBytes += sizeof(int);
	//step.4--------------------frameID-----------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Grid->frameID,sizeof(int));
	nBytes += sizeof(int);
	//step.5--------------------synTime-----------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Grid->synTime,sizeof(NJUST_IP_TIME));
	nBytes += sizeof(NJUST_IP_TIME);

	//step.6--------------------navID-------------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Grid->navID,sizeof(unsigned int));
	nBytes += sizeof(unsigned int );

	//step.7--------------------gridMsk-----------------------------//
	//RL编码
	BYTE temp;
	BYTE num;
	temp = p3D64Grid->gridMsk[0];
	num = 1;
	for (i = 1; i < NJUST_LIDAR64_VERTICAL_GRID_NUM*NJUST_LIDAR64_HORIZONTAL_GRID_NUM; i++)
	{
		if ((p3D64Grid->gridMsk[i]==p3D64Grid->gridMsk[i-1])&&
			(num<255)
			)
		{
			num++;
			continue;
		}
		else
		{
			//将二元组第一个参数记录
			memcpy(p3D64Buf+nBytes,&temp,sizeof(BYTE));
			nBytes += sizeof(BYTE);
			//将二元组第二个参数记录，即temp中的数重复了多少次
			memcpy(p3D64Buf+nBytes,&num,sizeof(BYTE));
			nBytes += sizeof(BYTE);
			//将新的数字放入temp,并将num清为1
			temp = p3D64Grid->gridMsk[i];
			num = 1;
		}
	}
	//将最后一个二元组写进表内
	//将二元组第一个参数记录
	memcpy(p3D64Buf+nBytes,&temp,sizeof(BYTE));
	nBytes += sizeof(BYTE);
	//将二元组第二个参数记录，即temp中的数重复了多少次
	memcpy(p3D64Buf+nBytes,&num,sizeof(BYTE));
	nBytes += sizeof(BYTE);
	//step.8---------------------返回-------------------------------//
	return nBytes;
}

/////////////////////////////////////////////////////////////////////////////
//
//
//---------------------------Cross-----------------------------------------//
//
//
/////////////////////////////////////////////////////////////////////////////
int NJUST_3D64_Encode_Compress_Cross(NJUST_3D64_CROSS_TO_FU *p3D64Cross, unsigned char *p3D64Buf, int maxResBufSize)
{
	int DataType=3;//表示该包为Cross包
	//step.1--------------------指针空处理--------------------------//
	if (!p3D64Cross||!p3D64Buf)
	{
		return 0; 
	}
	//step.2--------------------初始化------------------------------//	
	int i;
	int nBytes=0;
	memset(p3D64Buf,0,maxResBufSize*sizeof(BYTE));
	//step.3--------------------DataType----------------------------//
	memcpy(p3D64Buf+nBytes,&DataType,sizeof(int));
	nBytes += sizeof(int);
	//step.4--------------------frameID-----------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Cross->frameID,sizeof(int));
	nBytes += sizeof(int);
	//step.5--------------------synTime-----------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Cross->synTime,sizeof(NJUST_IP_TIME));
	nBytes += sizeof(NJUST_IP_TIME);
	//step.6--------------------navID-------------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Cross->navID,sizeof(unsigned int));
	nBytes += sizeof(unsigned int );
	//step.8--------------------A,B,C-------------------------------//
	memcpy(p3D64Buf+nBytes,&p3D64Cross->A,sizeof(double));
	nBytes += sizeof(double );
	memcpy(p3D64Buf+nBytes,&p3D64Cross->B,sizeof(double));
	nBytes += sizeof(double );
	memcpy(p3D64Buf+nBytes,&p3D64Cross->C,sizeof(double));
	nBytes += sizeof(double );

	//step.9--------------------crossMsk-----------------------------//
	//RL编码
	BYTE temp;
	BYTE num;
	temp = p3D64Cross->crossMsk[0];
	num = 1;
	for (i = 1; i < NJUST_LIDAR64_VERTICAL_GRID_NUM*NJUST_LIDAR64_HORIZONTAL_GRID_NUM; i++)
	{
		if ((p3D64Cross->crossMsk[i]==p3D64Cross->crossMsk[i-1])&&
			(num<255)
			)
		{
			num++;
			continue;
		}
		else
		{
			//将二元组第一个参数记录
			memcpy(p3D64Buf+nBytes,&temp,sizeof(BYTE));
			nBytes += sizeof(BYTE);
			//将二元组第二个参数记录，即temp中的数重复了多少次
			memcpy(p3D64Buf+nBytes,&num,sizeof(BYTE));
			nBytes += sizeof(BYTE);
			//将新的数字放入temp,并将num清为1
			temp = p3D64Cross->crossMsk[i];
			num = 1;
		}
	}
	//将最后一个二元组写进表内
	//将二元组第一个参数记录
	memcpy(p3D64Buf+nBytes,&temp,sizeof(BYTE));
	nBytes += sizeof(BYTE);
	//将二元组第二个参数记录，即temp中的数重复了多少次
	memcpy(p3D64Buf+nBytes,&num,sizeof(BYTE));
	nBytes += sizeof(BYTE);
	//step.10--------------------返回-------------------------------//
	return nBytes;
}

int NJUST_3D64_Encode_Compress(  int DataType,//压缩包的参数
								 NJUST_3D64_OBS_TO_FU *p3D64Obs,
								 NJUST_3D64_ROAD_BOUNDARY_TO_FU *p3D64Boundary,
								 NJUST_3D64_GRID_TO_FU *p3D64Grid,
								 NJUST_3D64_CROSS_TO_FU *p3D64Cross,
								 unsigned char *p3D64Buf,
								 int maxResBufSize //暂存器的最大尺寸
							   )
{
	int nBytes;
	nBytes=0;
	switch (DataType)
	{
	//DataType==0,表示该包为OBS包
	case 0: nBytes = NJUST_3D64_Encode_Compress_OBS(p3D64Obs,p3D64Buf,maxResBufSize);
			break;
	
	//DataType==1,表示该包为Boundary包
	case 1:	nBytes = NJUST_3D64_Encode_Compress_Boundary(p3D64Boundary,p3D64Buf,maxResBufSize);
			break;

	//DataType==2,表示该包为Grid包	
	case 2: nBytes = NJUST_3D64_Encode_Compress_Grid(p3D64Grid,p3D64Buf,maxResBufSize);
			break;

	//DataType==3,表示该包为Cross包		
	case 3: nBytes = NJUST_3D64_Encode_Compress_Cross(p3D64Cross,p3D64Buf,maxResBufSize);
			break;

	default:break;
	}
	//-------------------------返回大小---------------------------------//
	return nBytes;
}