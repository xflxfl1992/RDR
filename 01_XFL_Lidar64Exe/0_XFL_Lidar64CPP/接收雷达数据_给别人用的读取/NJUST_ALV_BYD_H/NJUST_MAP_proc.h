////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_MAP_proc.h
//  Author: 蔡云飞
//  Date:   2015.6.6
//  Description: 地图数据模块提供给目标模块的解码函数
//           int NJUST_MAP_Decode_IP_Data( const void* pIPData, const int nBytes,
//                                         NJUST_MAP_INFO_ROAD  **pRoad, //当不是道路数据时,值为NULL
//							               NJUST_MAP_INFO_NODE  **pNode, //当不是路口信息时,值为NULL
//                                         NJUST_MAP_INFO_DIRECTION      **pProprity//当不是方向导引时。值为NULL
//						                 );
//
//          int NJUST_MAP_Read_offline_data( const char* pFileName, //待读取的数据文件名
//                                           NJUST_MAP_INFO_ROAD  **pRoad, //当不包含道路数据时,值为NULL
//							                 NJUST_MAP_INFO_NODE  **pNode, //当不包含路口信息时,值为NULL
//                                           NJUST_MAP_INFO_DIRECTION      **pProprity//当不是方向导引时。值为NULL
//						                   );
//
//           int NJUST_MAP_Write_offline_data( const void* pIPData,  //待保存的与IP数据结构相同的数据块
//							                   const int   nBytes,   //pIPData数据的字节个数
//							  	               const char* pFileName //存储数据的文件名
//						                     );
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_MAP_PROC_H_
#define _NJUST_MAP_PROC_H_

#include "NJUST_MAP_data.h"

 #ifdef __cplusplus
 extern "C" {
 #endif
//
	////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//  提供给目标模块的解码函数
	//
	////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	* @brief 提供给目标模块的解码函数
	* @param pIPData {const void*} [in] IP数据,目标模块从网络收到的数据
	* @param nBytes {const int} [in] pIPData数据的字节个数
	* @param pRoad {NJUST_MAP_INFO_ROAD *} [out] 指向结果数据; 不是道路数据时,值为空指针NULL
	* @param pNode {NJUST_MAP_INFO_NODE *} [out] 指向调试信息; 不是路口信息时,值为空指针NULL
	* @return {int}, 成功返回0, 否则-1
	*/
	int NJUST_MAP_Decode_IP_Data( const void* pIPData, const int nBytes,
		NJUST_MAP_INFO_ROAD  **pRoad, //当不是道路数据时,值为NULL
		NJUST_MAP_INFO_NODE  **pNode, //当不是路口信息时,值为NULL
		NJUST_MAP_INFO_DIRECTION      **pProprity//当不是方向导引时。值为NULL
		);
	////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//  读取离线数据文件
	//
	////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	* @brief 读取离线数据文件
	* @param pFileName {const char*} [in] 待读取的数据文件名
	* @param pRoad {NJUST_MAP_INFO_ROAD *} [out] 指向结果数据; 当不包含道路数据时,值为空指针NULL
	* @param pNode {NJUST_MAP_INFO_NODE *} [out] 指向调试信息; 当不包含路口信息时,值为空指针NULL
	* @return {int}, 成功返回0, 否则-1
	*/
	int NJUST_MAP_Read_offline_data( const char* pFileName,       //待读取的数据文件名
		NJUST_MAP_INFO_ROAD  **pRoad, //当不包含道路数据时,值为NULL
		NJUST_MAP_INFO_NODE  **pNode, //当不包含路口信息时,值为NULL
		NJUST_MAP_INFO_DIRECTION      **pProprity//当不是方向导引时。值为NULL
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
	int NJUST_MAP_Write_offline_data( const void* pIPData,  //待保存的与IP数据结构相同的数据块
		const int   nBytes,   //pIPData数据的字节个数
		const char* pFileName //存储数据的文件名
		);

 #ifdef __cplusplus
 }
 #endif

#endif
