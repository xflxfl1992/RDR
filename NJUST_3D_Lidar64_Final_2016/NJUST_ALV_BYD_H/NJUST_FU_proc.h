////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_FU_proc.h
//  Author: 刘华军
//  Date:   2015.6.26
//  Description: 融合模块,提供给目标模块的解码函数
//  Modification: 
//          2015.7.3, 任明武
//  Functions:
//
//           int NJUST_FU_Decode_IP_Data( const void* pIPData,	    //IP数据
//				      			          const int nBytes,			//pIPData数据的字节个数
//							              NJUST_FU_TO_PL  **pRes,		//指向结果数据;不是结果数据时,值为空指针NULL
//							              NJUST_FU_DBGINFO **pDbg		//指向调试信息;不是调试信息时,值为空指针NULL
//						               );
//
//           int NJUST_FU_Read_offline_data( const char* pFileName, //待读取的数据文件名
//							                 NJUST_FU_TO_PL  **pRes,		//指向结果数据;不是结果数据时,值为空指针NULL
//								             NJUST_FU_DBGINFO **pDbg		//指向调试信息;不是调试信息时,值为空指针NULL
//						                   );
//
//           int NJUST_FU_Write_offline_data(  const void* pIPData,  //待保存的与IP数据结构相同的数据块
//							                   const int   nBytes,   //pIPData数据的字节个数							   
//							  	               const char* pFileName //存储数据的文件名
//                                          );
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_FU_PROC_H_
#define _NJUST_FU_PROC_H_

#include "NJUST_FU_data.h" 

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  提供给目标模块的解码函数
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @brief 提供给目标模块的解码函数
* @param pIPData {const void*} [in] IP数据,目标模块从网络收到的数据
* @param nBytes {const int} [in] pIPData数据的字节个数
* @param pRes {NJUST_FU_TO_PL **} [out] 指向结果数据;不是结果数据时,值为空指针NULL
* @param pDbg {NJUST_FU_DBGINFO **} [out] 指向调试信息; 不是调试信息时,值为空指针NULL
* @return {int}, 成功返回0, 否则-1
*/
int NJUST_FU_Decode_IP_Data( const void* pIPData,	    //IP数据
							 const int nBytes,			//pIPData数据的字节个数
							 NJUST_FU_TO_PL  **pRes,		//指向结果数据;不是结果数据时,值为空指针NULL
							 NJUST_FU_DBGINFO **pDbg		//指向调试信息;不是调试信息时,值为空指针NULL
						   );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  读取离线数据文件
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @brief 读取离线数据文件
* @param pFileName {const char*} [in] 待读取的数据文件名
* @param pRes {NJUST_FU_TO_PL **} [out] 指向结果数据;不是结果数据时,值为空指针NULL
* @param pDbg {NJUST_FU_DBGINFO **} [out] 指向调试信息; 不是调试信息时,值为空指针NULL
* @return {int}, 成功返回0, 否则-1
*/
int NJUST_FU_Read_offline_data( const char* pFileName, //待读取的数据文件名
							    NJUST_FU_TO_PL  **pRes,		//指向结果数据;不是结果数据时,值为空指针NULL
								NJUST_FU_DBGINFO **pDbg		//指向调试信息;不是调试信息时,值为空指针NULL
							  );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  写数据文件
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief 将数据写成文件
 * @param pIPData {const void*} [in] 待保存的与IP数据结构相同的数据块
 * @param nBytes {const int} [in] pIPData数据的字节个数
 * @param pFileName {const char*} [in] 存储数据的文件名
 * @return {int}, 成功返回0, 否则-1
 */
int NJUST_FU_Write_offline_data( const void* pIPData,  //待保存的与IP数据结构相同的数据块
							     const int   nBytes,   //pIPData数据的字节个数							   
							  	 const char* pFileName //存储数据的文件名
						       );

#ifdef __cplusplus
}
#endif

#endif
