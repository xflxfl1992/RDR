////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, �Ͼ�����ѧ�������ѧ�빤��ѧԺ, ���ܿ�ѧ�뼼��ϵ
//  FileName:  NJUST_IR_proc.h
//  Author: ������
//  Date:   2017.7.15
//  Description: ���������ǵ����ݴ���ģ��
//  Ŀǰ�����
//          ��Ȼ����,ˮ��
//  Modification: 
//          2016.7.15, ������
//  Functions:
//
//           int NJUST_IR_Decode_IP_Data( const void* pIPData, const int nBytes,
//                                        NJUST_IR_RES_TO_FU  **pRes, //�����ǽ������ʱ,ֵΪNULL
//							              NJUST_IR_INF_TO_MO  **pInf, //�����ǵ�����Ϣʱ,ֵΪNULL
//							              NJUST_IR_IMG_TO_MO  **pImg  //������ͼ������ʱ,ֵΪNULL
//						                );
//
//           int NJUST_IR_Read_offline_data( const char* pFileName, //����ȡ�������ļ���
//                                           NJUST_IR_RES_TO_FU  **pRes, //���������������ʱ,ֵΪNULL
//							                 NJUST_IR_INF_TO_MO  **pInf, //��������������Ϣʱ,ֵΪNULL
//							                 NJUST_IR_IMG_TO_MO  **pImg  //��������ͼ������ʱ,ֵΪNULL
//						                   );
//
//           int NJUST_IR_Write_offline_data( const void* pIPData,  //���������IP���ݽṹ��ͬ�����ݿ�
//							                  const int   nBytes,   //pIPData���ݵ��ֽڸ���							   
//							  	              const char* pFileName //�洢���ݵ��ļ���
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
//  �ṩ��Ŀ��ģ��Ľ��뺯��
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief �ṩ��Ŀ��ģ��Ľ��뺯��
 * @param pIPData {const void*} [in] IP����,Ŀ��ģ��������յ�������
 * @param nBytes {const int} [in] pIPData���ݵ��ֽڸ���
 * @param pRes {NJUST_IR_RES_TO_FU **} [out] ָ��������; ���ǽ������ʱ,ֵΪ��ָ��NULL
 * @param pInf {NJUST_IR_INF_TO_MO **} [out] ָ�������Ϣ; ���ǵ�����Ϣʱ,ֵΪ��ָ��NULL
 * @param pImg {NJUST_IR_IMG_TO_MO **} [out] ָ��ͼ������; ����ͼ������ʱ,ֵΪ��ָ��NULL
 * @return {int}, �ɹ�����0, ����-1
 */
int NJUST_IR_Decode_IP_Data( const void* pIPData,  //IP����,Ŀ��ģ��������յ�������
							 const int nBytes,     //pIPData���ݵ��ֽڸ���
                             NJUST_IR_RES_TO_FU  **pRes, //�����ǽ������ʱ,ֵΪNULL
							 NJUST_IR_INF_TO_MO  **pInf, //�����ǵ�����Ϣʱ,ֵΪNULL
							 NJUST_IR_IMG_TO_MO  **pImg  //������ͼ������ʱ,ֵΪNULL
						   );
						   
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ��ȡ���������ļ�
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ��ȡ���������ļ�
 * @param pFileName {const char*} [in] ����ȡ�������ļ���
 * @param pRes {NJUST_IR_RES_TO_FU **} [out] ָ��������; ���������������ʱ,ֵΪ��ָ��NULL
 * @param pInf {NJUST_IR_INF_TO_MO **} [out] ָ�������Ϣ; ��������������Ϣʱ,ֵΪ��ָ��NULL
 * @param pImg {NJUST_IR_IMG_TO_MO **} [out] ָ��ͼ������; ��������ͼ������ʱ,ֵΪ��ָ��NULL
 * @return {int}, �ɹ�����0, ����-1
 */
int NJUST_IR_Read_offline_data( const char* pFileName, //����ȡ�������ļ���
                                NJUST_IR_RES_TO_FU  **pRes, //���������������ʱ,ֵΪNULL
							    NJUST_IR_INF_TO_MO  **pInf, //��������������Ϣʱ,ֵΪNULL
							    NJUST_IR_IMG_TO_MO  **pImg  //��������ͼ������ʱ,ֵΪNULL
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
int NJUST_IR_Write_offline_data( const void* pIPData,  //���������IP���ݽṹ��ͬ�����ݿ�
							     const int   nBytes,   //pIPData���ݵ��ֽڸ���							   
							  	 const char* pFileName //�洢���ݵ��ļ���
 						       );
#ifdef __cplusplus
}
#endif

#endif
