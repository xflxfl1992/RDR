////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_MO_proc.h
//  Author: 张重阳
//  Date:   2015.6.26
//  Description: 显控模块和其他模块间的命令和状态
//  Modification: 
//          2015.7.2, 任明武
//  Functions:
//
//           int NJUST_MO_Decode_IP_Data_CMD( const void* pIPData, 
//                                            const int nBytes,
//                                            NJUST_FROM_MO_COMMAND  **pCommand //当不是来自显控的命令时,值为NULL
//						                    );
//
//           void* NJUST_MO_Encode_STA_IP_Data( NJUST_TO_MO_WORKSTAT *pWorkStat,
//                                              int *nBytes,
//						                      );
//

//  修改
//  日期：  2016.5.5
//  内容：int NJUST_MO_Decode_IP_Data_CMD( const void* pIPData,
//                                         const int nBytes,
//	                                       NJUST_FROM_MO_COMMAND   **pCommand; //当不是网络命令数据时,值为NULL
//                                         NJUST_FROM_MO_CFG   **pCFG //当不是网络命令数据时,值为NULL
//	                                     );
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_MO_PROC_H_
#define _NJUST_MO_PROC_H_

#include "NJUST_MO_data.h" 

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  显控提供给目标模块的解码函数
//
////////////////////////////////////////////////////////////////////////////////////////////////
// 20160505 解码函数新增  @param pCFG {NJUST_FROM_MO_CFG **}
/**
 * @brief 提供给目标模块的解码函数
 * @param pIPData {const void*} [in] IP数据,目标模块从网络收到的数据
 * @param nBytes {const int} [in] pIPData数据的字节个数
 * @param pCommand {NJUST_FROM_MO_COMMAND **} [out] 当不是来自显控的命令时,值为空指针NULL
 * @param pCFG {NJUST_FROM_MO_CFG **} [out] 当不是来自显控的命令时,值为空指针NULL
 * @return {int}, 成功返回0, 否则-1
 */
int NJUST_MO_Decode_IP_Data_CMD( const void* pIPData,
                                 const int nBytes,
                                 NJUST_FROM_MO_COMMAND   **pCommand, //当不是网络命令数据时,值为NULL
                                 NJUST_FROM_MO_CFG   **pCFG //当不是网络命令数据时,值为NULL
						       );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  提供给目标模块的编码函数
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief 提供给目标模块的编码函数
 * @param pWorkStat {NJUST_TO_MO_WORKSTAT *} [in] 指向状态数据; 
 * @param nBytes {int *} [out] 返回数据的字节个数
 * @return {void *}, 成功返回数据指针, 否则返回NULL
 */
void* NJUST_MO_Encode_STA_IP_Data( NJUST_TO_MO_WORKSTAT *pWorkStat,
                                   int *nBytes
					             );

#ifdef __cplusplus
}
#endif

#endif