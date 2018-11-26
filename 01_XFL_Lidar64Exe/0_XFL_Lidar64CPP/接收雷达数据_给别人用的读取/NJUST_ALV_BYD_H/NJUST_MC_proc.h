////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_MC_proc.h
//  Author: 夏青元
//  Date:   2015.6.26
//  Description: 车体底层控制模块的数据的函数
//  Modification: 
//          2015.7.3, 任明武
//  Functions:
//
//           int NJUST_MC_Decode_IP_Data( const void* pIPData, const int nBytes,
//                                        NJUST_MC_STATE_INFO  **pState, //当不是状态数据时,值为NULL
//							              NJUST_MC_NAV_INFO  **pNav, //当不是导航信息时,值为NULL
//							              NJUST_MC_DRIVE_INFO  **pDrive  //当不是执行数据时,值为NULL
//						                );
//
//           int NJUST_MC_Read_offline_data( const char* pFileName, //待读取的数据文件名
//                                           NJUST_MC_STATE_INFO  **pState, //当不是状态数据时,值为NULL
//							                 NJUST_MC_NAV_INFO  **pNav, //当不是导航信息时,值为NULL
//							                 NJUST_MC_DRIVE_INFO  **pDrive  //当不是执行数据时,值为NULL
//						                   );
//
//           int NJUST_MC_Write_offline_data( const void* pIPData,  //待保存的与IP数据结构相同的数据块
//							                  const int   nBytes,   //pIPData数据的字节个数							   
//							  	              const char* pFileName //存储数据的文件名
//						                    );
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_MC_PROC_H_
#define _NJUST_MC_PROC_H_

#include "NJUST_MC_data.h" 

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
 * @param pState {NJUST_MC_STATE_INFO **} [out] 指向状态数据; 不是状态数据时,值为空指针NULL
 * @param pNav {NJUST_MC_NAV_INFO **} [out] 指向导航信息; 不是导航信息时,值为空指针NULL
 * @param pDrive {NJUST_MC_DRIVE_INFO **} [out] 指向执行数据; 不是执行数据时,值为空指针NULL
 * @return {int}, 成功返回0, 否则-1
 */
int NJUST_MC_Decode_IP_Data( const void* pIPData,  //IP数据,目标模块从网络收到的数据
							 const int nBytes,     //pIPData数据的字节个数
                             NJUST_MC_STATE_INFO  **pState, //当不是状态数据时,值为NULL
				             NJUST_MC_NAV_INFO  **pNav, //当不是导航信息时,值为NULL
							 NJUST_MC_DRIVE_INFO  **pDrive  //当不是执行数据时,值为NULL
						   );
						   
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  读取离线数据文件
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief 读取离线数据文件
 * @param pFileName {const char*} [in] 待读取的数据文件名
 * @param pState {NJUST_MC_STATE_INFO **} [out] 指向状态数据; 不是状态数据时,值为空指针NULL
 * @param pNav {NJUST_MC_NAV_INFO **} [out] 指向导航信息; 不是导航信息时,值为空指针NULL
 * @param pDrive {NJUST_MC_DRIVE_INFO **} [out] 指向执行数据; 不是执行数据时,值为空指针NULL
 * @return {int}, 成功返回0, 否则-1
 */
int NJUST_MC_Read_offline_data( const char* pFileName, //待读取的数据文件名
                                NJUST_MC_STATE_INFO  **pState, //当不是状态数据时,值为NULL
				                NJUST_MC_NAV_INFO  **pNav, //当不是导航信息时,值为NULL
							    NJUST_MC_DRIVE_INFO  **pDrive  //当不是执行数据时,值为NULL
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
int NJUST_MC_Write_offline_data( const void* pIPData,  //待保存的与IP数据结构相同的数据块
							     const int   nBytes,   //pIPData数据的字节个数							   
							  	 const char* pFileName //存储数据的文件名
 						       );

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  提供经纬度坐标转地面平面坐标
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief 提供经纬度坐标转地面平面坐标函数
 * @param Longitude_degree {double} [in] 输入的经度 单位 度
 * @param Latitude_degree  {double} [in] 输入的纬度 单位 度 
 * @param EarthRefCoord_x_m{double *} [out] 转换后的平面坐标值 东向位置坐标值 单位 米
 * @param EarthRefCoord_y_m{double *} [out] 转换后的平面坐标值 北向位置坐标值 单位 米
 * @return {int}, 成功返回0, 否则-1
 */
int Conver_E_N(double Longitude_degree,
			   double Latitude_degree,
			   double *EarthRefCoord_x_m,
			   double *EarthRefCoord_y_m);

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  提供地面平面坐标转车载坐标系
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief 提供地面平面坐标转车载坐标系函数
 * @param in_yaw_rad {double *} [in] 输入转换时的航向角 单位 弧度
 * @param TargetEarthCoord_x_m {double} [in] 目标点地面坐标x值(东向位置坐标值) 单位 米
 * @param TargetEarthCoord_y_m {double} [in] 目标点地面坐标y值(北向位置坐标值) 单位 米
 * @param CarEarthCoord_x_m {double} [in] 车地面坐标x值(东向位置坐标值) 单位 米
 * @param CarEarthCoord_y_m {double} [in] 车地面坐标y值(北向位置坐标值) 单位 米
 * @param out_Coord_B_x_cm {double *} [out] 转换后的车载坐标x值 右向车载坐标值 单位 厘米
 * @param out_Coord_B_y_cm {double *} [out] 转换后的车载坐标y值 前方位置坐标值 单位 厘米						
 * @return {int}, 成功返回0, 否则-1
 */
int Conver_N_B(double in_yaw_rad,
			   double TargetEarthCoord_x_m,
			   double TargetEarthCoord_y_m,
			   double CarEarthCoord_x_m,
			   double CarEarthCoord_y_m,
			   int *out_Coord_B_x_cm,
			   int *out_Coord_B_y_cm);




#ifdef __cplusplus
}
#endif

#endif
