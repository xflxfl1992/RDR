////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, �Ͼ�����ѧ�������ѧ�빤��ѧԺ, ���ܿ�ѧ�뼼��ϵ
//  FileName:  NJUST_PL_proc.h
//  Author: ��ǿ
//  Date:   2015.8.3
//  Description: �滮,�ṩ��Ŀ��ģ��Ľ��뺯��
//  Functions:
//
//           int NJUST_PL_Decode_IP_Data( const void* pIPData,	    //IP����
//				      			          const int nBytes,			//pIPData���ݵ��ֽڸ���
//							              NJUST_FU_TO_PL  **pRes,		//ָ��������;���ǽ������ʱ,ֵΪ��ָ��NULL
//							              NJUST_FU_DBGINFO **pDbg		//ָ�������Ϣ;���ǵ�����Ϣʱ,ֵΪ��ָ��NULL
//						               );
//  �޸ļ�¼
//  Date:   2016.7.27
//          ������
//          ������__cplusplus,ȥ����struct NJUST_PL_DBGINFO.
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_PL_PROC_H_
#define _NJUST_PL_PROC_H_

#include "NJUST_PL_data.h"

#ifdef __cplusplus
extern "C" {
#endif

int NJUST_PL_Decode_IP_Data( const void* pIPData, //IP����
							 const int nBytes,	//pIPData���ݵ��ֽڸ���
							 NJUST_PL_TO_MC  **pRes	//ָ��������;���ǽ������ʱ,ֵΪ��ָ��NULL
 						   );

#ifdef __cplusplus
}
#endif

#endif