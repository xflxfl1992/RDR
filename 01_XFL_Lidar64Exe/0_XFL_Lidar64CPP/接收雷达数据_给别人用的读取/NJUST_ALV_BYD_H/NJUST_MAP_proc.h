////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, �Ͼ�����ѧ�������ѧ�빤��ѧԺ, ���ܿ�ѧ�뼼��ϵ
//  FileName:  NJUST_MAP_proc.h
//  Author: ���Ʒ�
//  Date:   2015.6.6
//  Description: ��ͼ����ģ���ṩ��Ŀ��ģ��Ľ��뺯��
//           int NJUST_MAP_Decode_IP_Data( const void* pIPData, const int nBytes,
//                                         NJUST_MAP_INFO_ROAD  **pRoad, //�����ǵ�·����ʱ,ֵΪNULL
//							               NJUST_MAP_INFO_NODE  **pNode, //������·����Ϣʱ,ֵΪNULL
//                                         NJUST_MAP_INFO_DIRECTION      **pProprity//�����Ƿ�����ʱ��ֵΪNULL
//						                 );
//
//          int NJUST_MAP_Read_offline_data( const char* pFileName, //����ȡ�������ļ���
//                                           NJUST_MAP_INFO_ROAD  **pRoad, //����������·����ʱ,ֵΪNULL
//							                 NJUST_MAP_INFO_NODE  **pNode, //��������·����Ϣʱ,ֵΪNULL
//                                           NJUST_MAP_INFO_DIRECTION      **pProprity//�����Ƿ�����ʱ��ֵΪNULL
//						                   );
//
//           int NJUST_MAP_Write_offline_data( const void* pIPData,  //���������IP���ݽṹ��ͬ�����ݿ�
//							                   const int   nBytes,   //pIPData���ݵ��ֽڸ���
//							  	               const char* pFileName //�洢���ݵ��ļ���
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
	//  �ṩ��Ŀ��ģ��Ľ��뺯��
	//
	////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	* @brief �ṩ��Ŀ��ģ��Ľ��뺯��
	* @param pIPData {const void*} [in] IP����,Ŀ��ģ��������յ�������
	* @param nBytes {const int} [in] pIPData���ݵ��ֽڸ���
	* @param pRoad {NJUST_MAP_INFO_ROAD *} [out] ָ��������; ���ǵ�·����ʱ,ֵΪ��ָ��NULL
	* @param pNode {NJUST_MAP_INFO_NODE *} [out] ָ�������Ϣ; ����·����Ϣʱ,ֵΪ��ָ��NULL
	* @return {int}, �ɹ�����0, ����-1
	*/
	int NJUST_MAP_Decode_IP_Data( const void* pIPData, const int nBytes,
		NJUST_MAP_INFO_ROAD  **pRoad, //�����ǵ�·����ʱ,ֵΪNULL
		NJUST_MAP_INFO_NODE  **pNode, //������·����Ϣʱ,ֵΪNULL
		NJUST_MAP_INFO_DIRECTION      **pProprity//�����Ƿ�����ʱ��ֵΪNULL
		);
	////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//  ��ȡ���������ļ�
	//
	////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	* @brief ��ȡ���������ļ�
	* @param pFileName {const char*} [in] ����ȡ�������ļ���
	* @param pRoad {NJUST_MAP_INFO_ROAD *} [out] ָ��������; ����������·����ʱ,ֵΪ��ָ��NULL
	* @param pNode {NJUST_MAP_INFO_NODE *} [out] ָ�������Ϣ; ��������·����Ϣʱ,ֵΪ��ָ��NULL
	* @return {int}, �ɹ�����0, ����-1
	*/
	int NJUST_MAP_Read_offline_data( const char* pFileName,       //����ȡ�������ļ���
		NJUST_MAP_INFO_ROAD  **pRoad, //����������·����ʱ,ֵΪNULL
		NJUST_MAP_INFO_NODE  **pNode, //��������·����Ϣʱ,ֵΪNULL
		NJUST_MAP_INFO_DIRECTION      **pProprity//�����Ƿ�����ʱ��ֵΪNULL
		);

	////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//  д�����ļ�
	//
	////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	* @brief ������д���ļ�
	* @param pIPData {const void*} [in] ���������IP���ݽṹ��ͬ�����ݿ�
	* @param nBytes {const int} [in] pIPData���ݵ��ֽڸ���
	* @param pFileName {const char*} [in] �洢���ݵ��ļ���
	* @return {int}, �ɹ�����0, ����-1
	*/
	int NJUST_MAP_Write_offline_data( const void* pIPData,  //���������IP���ݽṹ��ͬ�����ݿ�
		const int   nBytes,   //pIPData���ݵ��ֽڸ���
		const char* pFileName //�洢���ݵ��ļ���
		);

 #ifdef __cplusplus
 }
 #endif

#endif
