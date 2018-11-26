////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_PL_proc.h
//  Author: 李强
//  Date:   2015.8.3
//  Description: 规划,提供给目标模块的解码函数
//  Functions:
//
//           int NJUST_PL_Decode_IP_Data( const void* pIPData,	    //IP数据
//				      			          const int nBytes,			//pIPData数据的字节个数
//							              NJUST_FU_TO_PL  **pRes,		//指向结果数据;不是结果数据时,值为空指针NULL
//							              NJUST_FU_DBGINFO **pDbg		//指向调试信息;不是调试信息时,值为空指针NULL
//						               );
//  修改记录
//  Date:   2016.7.27
//          任明武
//          增加了__cplusplus,去掉了struct NJUST_PL_DBGINFO.
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_PL_PROC_H_
#define _NJUST_PL_PROC_H_

#include "NJUST_PL_data.h"

#ifdef __cplusplus
extern "C" {
#endif

int NJUST_PL_Decode_IP_Data( const void* pIPData, //IP数据
							 const int nBytes,	//pIPData数据的字节个数
							 NJUST_PL_TO_MC  **pRes	//指向结果数据;不是结果数据时,值为空指针NULL
 						   );

#ifdef __cplusplus
}
#endif

#endif