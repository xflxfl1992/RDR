////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_DATA_typedef.h
//  Date:   2015.6.6
//  Description: ALV的数据类型,来自不同操作系统平台时的定义
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_DATA_TYPEDEF_H_
#define _NJUST_DATA_TYPEDEF_H_

#include "NJUST_Global_Def.h"

//WINDOW下的数据类型定义
#ifdef _NJUST_OS_USE_WINDOWS_
typedef unsigned int UINT32; /*!< 32 bits, unsigned integer. */
typedef signed int INT32;  /*!< 32 bits, signed integer. */
typedef unsigned short UINT16; /*!< 16 bits, unsigned short integer. */
typedef signed short INT16;  /*!< 16 bits, signed short integer. */
typedef unsigned char UINT8;  /*!<  8 bits, unsigned char integer. */
typedef signed char INT8;   /*!<  8 bits, signed char integer. */
typedef unsigned char BYTE;   /*!<  8 bits, signed char integer. */
#endif

//LINUX下的数据类型定义
#ifdef _NJUST_OS_USE_LINUX_
typedef unsigned int UINT32; /*!< 32 bits, unsigned integer. */
typedef signed int INT32;  /*!< 32 bits, signed integer. */
typedef unsigned short UINT16; /*!< 16 bits, unsigned short integer. */
typedef signed short INT16;  /*!< 16 bits, signed short integer. */
typedef unsigned char UINT8;  /*!<  8 bits, unsigned char integer. */
typedef signed char INT8;   /*!<  8 bits, signed char integer. */
typedef unsigned char BYTE;   /*!<  8 bits, signed char integer. */
#endif

//ARM下的数据类型定义
#ifdef _NJUST_OS_USE_ARM_
typedef unsigned int UINT32; /*!< 32 bits, unsigned integer. */
typedef signed int INT32;  /*!< 32 bits, signed integer. */
typedef unsigned short UINT16; /*!< 16 bits, unsigned short integer. */
typedef signed short INT16;  /*!< 16 bits, signed short integer. */
typedef unsigned char UINT8;  /*!<  8 bits, unsigned char integer. */
typedef signed char INT8;   /*!<  8 bits, signed char integer. */
typedef unsigned char BYTE;   /*!<  8 bits, signed char integer. */
#endif


#endif



