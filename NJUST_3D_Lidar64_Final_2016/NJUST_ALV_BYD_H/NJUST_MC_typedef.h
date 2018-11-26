////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, �Ͼ�����ѧ�������ѧ�빤��ѧԺ, ���ܿ�ѧ�뼼��ϵ
//  FileName:  NJUST_DATA_typedef.h
//  Date:   2015.6.6
//  Description: ALV����������,���Բ�ͬ����ϵͳƽ̨ʱ�Ķ���
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_DATA_TYPEDEF_H_
#define _NJUST_DATA_TYPEDEF_H_

#include "NJUST_Global_Def.h"

//WINDOW�µ��������Ͷ���
#ifdef _NJUST_OS_USE_WINDOWS_
typedef unsigned int UINT32; /*!< 32 bits, unsigned integer. */
typedef signed int INT32;  /*!< 32 bits, signed integer. */
typedef unsigned short UINT16; /*!< 16 bits, unsigned short integer. */
typedef signed short INT16;  /*!< 16 bits, signed short integer. */
typedef unsigned char UINT8;  /*!<  8 bits, unsigned char integer. */
typedef signed char INT8;   /*!<  8 bits, signed char integer. */
typedef unsigned char BYTE;   /*!<  8 bits, signed char integer. */
#endif

//LINUX�µ��������Ͷ���
#ifdef _NJUST_OS_USE_LINUX_
typedef unsigned int UINT32; /*!< 32 bits, unsigned integer. */
typedef signed int INT32;  /*!< 32 bits, signed integer. */
typedef unsigned short UINT16; /*!< 16 bits, unsigned short integer. */
typedef signed short INT16;  /*!< 16 bits, signed short integer. */
typedef unsigned char UINT8;  /*!<  8 bits, unsigned char integer. */
typedef signed char INT8;   /*!<  8 bits, signed char integer. */
typedef unsigned char BYTE;   /*!<  8 bits, signed char integer. */
#endif

//ARM�µ��������Ͷ���
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



