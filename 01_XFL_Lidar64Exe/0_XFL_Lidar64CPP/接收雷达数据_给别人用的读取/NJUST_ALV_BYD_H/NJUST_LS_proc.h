////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_LS_proc.h
//  Author: 张重阳
//  Date:   2015.6.26
//  Description: 感知灯光模块和交通标识模块,提供给目标模块的解码函数
//  Modification: 
//          2015.6.26, 任明武
//  Functions:
//
//           int NJUST_LS_Decode_IP_Data( const void* pIPData,
//                                        const int nBytes,
//                                        NJUST_LS_SIGN_RES_TO_FU   **pSign, //当不是交通标识数据时,值为NULL
//                                        NJUST_LS_LIGHT_RES_TO_FU  **pLight, //当不是交通灯数据时,值为NULL
//							              NJUST_LS_INF_TO_MO   **pInf, //当不是调试信息时,值为NULL
//							              NJUST_LS_IMG_TO_MO   **pImg  //当不是图像数据时,值为NULL
//						                );
//
//           int NJUST_LS_Read_offline_data( const char* pFileName, //待读取的数据文件名
//                                           NJUST_LS_SIGN_RES_TO_FU   **pSign, //当不包含交通标识数据时,值为NULL
//                                           NJUST_LS_LIGHT_RES_TO_FU  **pLight, //当不包含交通灯数据时,值为NULL
//							                 NJUST_LS_INF_TO_MO   **pInf, //当不包含调试信息时,值为NULL
//							                 NJUST_LS_IMG_TO_MO   **pImg  //当不包含图像数据时,值为NULL
//						                   );
//
//           int NJUST_LS_Write_offline_data(  const void* pIPData,  //待保存的与IP数据结构相同的数据块
//							                   const int   nBytes,   //pIPData数据的字节个数							   
//							  	               const char* pFileName //存储数据的文件名
//                                          );
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_LS_PROC_H_
#define _NJUST_LS_PROC_H_

#include "NJUST_LS_data.h" 

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
 * @param pSign {NJUST_LS_SIGN_RES_TO_FU **} [out] 指向交通标识数据; 当不是交通标识数据时,值为空指针NULL
 * @param pLight {NJUST_LS_LIGHT_RES_TO_FU **} [out] 指向交通灯数据;当不是交通灯数据时,值为空指针NULL
 * @param pInf {NJUST_LS_INF_TO_MO **} [out] 指向调试信息; 不是调试信息时,值为空指针NULL
 * @param pImg {NJUST_LS_IMG_TO_MO **} [out] 指向图像数据; 不是图像数据时,值为空指针NULL
 * @return {int}, 成功返回0, 否则-1
 */
int NJUST_LS_Decode_IP_Data( const void* pIPData, const int nBytes,
                             NJUST_LS_SIGN_RES_TO_FU   **pSign, //当不是交通标识数据时,值为NULL
                             NJUST_LS_LIGHT_RES_TO_FU  **pLight, //当不是交通灯数据时,值为NULL
							 NJUST_LS_INF_TO_MO   **pInf, //当不是调试信息时,值为NULL
							 NJUST_LS_IMG_TO_MO   **pImg  //当不是图像数据时,值为NULL
						   );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  读取离线数据文件
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief 读取离线数据文件
 * @param pFileName {const char*} [in] 待读取的数据文件名
 * @param nBytes {const int} [in] pIPData数据的字节个数
 * @param pSign {NJUST_LS_SIGN_RES_TO_FU **} [out] 指向交通标识数据; 当不包含交通标识数据时,值为空指针NULL
 * @param pLight {NJUST_LS_LIGHT_RES_TO_FU **} [out] 指向交通灯数据;当不包含交通灯数据时,值为空指针NULL
 * @param pInf {NJUST_LS_INF_TO_MO **} [out] 指向调试信息; 当不包含调试信息时,值为空指针NULL
 * @param pImg {NJUST_LS_IMG_TO_MO **} [out] 指向图像数据; 当不包含图像数据时,值为空指针NULL
 * @return {int}, 成功返回0, 否则-1
 */
int NJUST_LS_Read_offline_data( const char* pFileName, //待读取的数据文件名
                                NJUST_LS_SIGN_RES_TO_FU   **pSign, //当不包含交通标识数据时,值为NULL
                                NJUST_LS_LIGHT_RES_TO_FU  **pLight, //当不包含交通灯数据时,值为NULL
							    NJUST_LS_INF_TO_MO   **pInf, //当不包含调试信息时,值为NULL
							    NJUST_LS_IMG_TO_MO   **pImg  //当不包含图像数据时,值为NULL
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
int NJUST_LS_Write_offline_data( const void* pIPData,  //待保存的与IP数据结构相同的数据块
							     const int   nBytes,   //pIPData数据的字节个数							   
							  	 const char* pFileName //存储数据的文件名
						       );

#ifdef __cplusplus
}
#endif

#endif
