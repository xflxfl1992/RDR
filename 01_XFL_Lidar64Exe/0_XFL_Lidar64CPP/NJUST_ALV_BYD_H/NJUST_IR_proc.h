////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_IR_proc.h
//  Author: 任明武
//  Date:   2017.7.15
//  Description: 红外热像仪的数据处理模块
//  目前输出：
//          自然道边,水坑
//  Modification: 
//          2016.7.15, 任明武
//  Functions:
//
//           int NJUST_IR_Decode_IP_Data( const void* pIPData, const int nBytes,
//                                        NJUST_IR_RES_TO_FU  **pRes, //当不是结果数据时,值为NULL
//							              NJUST_IR_INF_TO_MO  **pInf, //当不是调试信息时,值为NULL
//							              NJUST_IR_IMG_TO_MO  **pImg  //当不是图像数据时,值为NULL
//						                );
//
//           int NJUST_IR_Read_offline_data( const char* pFileName, //待读取的数据文件名
//                                           NJUST_IR_RES_TO_FU  **pRes, //当不包含结果数据时,值为NULL
//							                 NJUST_IR_INF_TO_MO  **pInf, //当不包含调试信息时,值为NULL
//							                 NJUST_IR_IMG_TO_MO  **pImg  //当不包含图像数据时,值为NULL
//						                   );
//
//           int NJUST_IR_Write_offline_data( const void* pIPData,  //待保存的与IP数据结构相同的数据块
//							                  const int   nBytes,   //pIPData数据的字节个数							   
//							  	              const char* pFileName //存储数据的文件名
//						                    );
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_IR_PROC_H_
#define _NJUST_IR_PROC_H_

#include "NJUST_IR_data.h" 

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
 * @param pRes {NJUST_IR_RES_TO_FU **} [out] 指向结果数据; 不是结果数据时,值为空指针NULL
 * @param pInf {NJUST_IR_INF_TO_MO **} [out] 指向调试信息; 不是调试信息时,值为空指针NULL
 * @param pImg {NJUST_IR_IMG_TO_MO **} [out] 指向图像数据; 不是图像数据时,值为空指针NULL
 * @return {int}, 成功返回0, 否则-1
 */
int NJUST_IR_Decode_IP_Data( const void* pIPData,  //IP数据,目标模块从网络收到的数据
							 const int nBytes,     //pIPData数据的字节个数
                             NJUST_IR_RES_TO_FU  **pRes, //当不是结果数据时,值为NULL
							 NJUST_IR_INF_TO_MO  **pInf, //当不是调试信息时,值为NULL
							 NJUST_IR_IMG_TO_MO  **pImg  //当不是图像数据时,值为NULL
						   );
						   
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  读取离线数据文件
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief 读取离线数据文件
 * @param pFileName {const char*} [in] 待读取的数据文件名
 * @param pRes {NJUST_IR_RES_TO_FU **} [out] 指向结果数据; 当不包含结果数据时,值为空指针NULL
 * @param pInf {NJUST_IR_INF_TO_MO **} [out] 指向调试信息; 当不包含调试信息时,值为空指针NULL
 * @param pImg {NJUST_IR_IMG_TO_MO **} [out] 指向图像数据; 当不包含图像数据时,值为空指针NULL
 * @return {int}, 成功返回0, 否则-1
 */
int NJUST_IR_Read_offline_data( const char* pFileName, //待读取的数据文件名
                                NJUST_IR_RES_TO_FU  **pRes, //当不包含结果数据时,值为NULL
							    NJUST_IR_INF_TO_MO  **pInf, //当不包含调试信息时,值为NULL
							    NJUST_IR_IMG_TO_MO  **pImg  //当不包含图像数据时,值为NULL
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
int NJUST_IR_Write_offline_data( const void* pIPData,  //待保存的与IP数据结构相同的数据块
							     const int   nBytes,   //pIPData数据的字节个数							   
							  	 const char* pFileName //存储数据的文件名
 						       );
#ifdef __cplusplus
}
#endif

#endif
