////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, �Ͼ�����ѧ�������ѧ�빤��ѧԺ, ���ܿ�ѧ�뼼��ϵ
//  FileName:  NJUST_LS_proc.h
//  Author: ������
//  Date:   2015.6.26
//  Description: ��֪�ƹ�ģ��ͽ�ͨ��ʶģ��,�ṩ��Ŀ��ģ��Ľ��뺯��
//  Modification: 
//          2015.6.26, ������
//  Functions:
//
//           int NJUST_LS_Decode_IP_Data( const void* pIPData,
//                                        const int nBytes,
//                                        NJUST_LS_SIGN_RES_TO_FU   **pSign, //�����ǽ�ͨ��ʶ����ʱ,ֵΪNULL
//                                        NJUST_LS_LIGHT_RES_TO_FU  **pLight, //�����ǽ�ͨ������ʱ,ֵΪNULL
//							              NJUST_LS_INF_TO_MO   **pInf, //�����ǵ�����Ϣʱ,ֵΪNULL
//							              NJUST_LS_IMG_TO_MO   **pImg  //������ͼ������ʱ,ֵΪNULL
//						                );
//
//           int NJUST_LS_Read_offline_data( const char* pFileName, //����ȡ�������ļ���
//                                           NJUST_LS_SIGN_RES_TO_FU   **pSign, //����������ͨ��ʶ����ʱ,ֵΪNULL
//                                           NJUST_LS_LIGHT_RES_TO_FU  **pLight, //����������ͨ������ʱ,ֵΪNULL
//							                 NJUST_LS_INF_TO_MO   **pInf, //��������������Ϣʱ,ֵΪNULL
//							                 NJUST_LS_IMG_TO_MO   **pImg  //��������ͼ������ʱ,ֵΪNULL
//						                   );
//
//           int NJUST_LS_Write_offline_data(  const void* pIPData,  //���������IP���ݽṹ��ͬ�����ݿ�
//							                   const int   nBytes,   //pIPData���ݵ��ֽڸ���							   
//							  	               const char* pFileName //�洢���ݵ��ļ���
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
//  �ṩ��Ŀ��ģ��Ľ��뺯��
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief �ṩ��Ŀ��ģ��Ľ��뺯��
 * @param pIPData {const void*} [in] IP����,Ŀ��ģ��������յ�������
 * @param nBytes {const int} [in] pIPData���ݵ��ֽڸ���
 * @param pSign {NJUST_LS_SIGN_RES_TO_FU **} [out] ָ��ͨ��ʶ����; �����ǽ�ͨ��ʶ����ʱ,ֵΪ��ָ��NULL
 * @param pLight {NJUST_LS_LIGHT_RES_TO_FU **} [out] ָ��ͨ������;�����ǽ�ͨ������ʱ,ֵΪ��ָ��NULL
 * @param pInf {NJUST_LS_INF_TO_MO **} [out] ָ�������Ϣ; ���ǵ�����Ϣʱ,ֵΪ��ָ��NULL
 * @param pImg {NJUST_LS_IMG_TO_MO **} [out] ָ��ͼ������; ����ͼ������ʱ,ֵΪ��ָ��NULL
 * @return {int}, �ɹ�����0, ����-1
 */
int NJUST_LS_Decode_IP_Data( const void* pIPData, const int nBytes,
                             NJUST_LS_SIGN_RES_TO_FU   **pSign, //�����ǽ�ͨ��ʶ����ʱ,ֵΪNULL
                             NJUST_LS_LIGHT_RES_TO_FU  **pLight, //�����ǽ�ͨ������ʱ,ֵΪNULL
							 NJUST_LS_INF_TO_MO   **pInf, //�����ǵ�����Ϣʱ,ֵΪNULL
							 NJUST_LS_IMG_TO_MO   **pImg  //������ͼ������ʱ,ֵΪNULL
						   );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ��ȡ���������ļ�
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ��ȡ���������ļ�
 * @param pFileName {const char*} [in] ����ȡ�������ļ���
 * @param nBytes {const int} [in] pIPData���ݵ��ֽڸ���
 * @param pSign {NJUST_LS_SIGN_RES_TO_FU **} [out] ָ��ͨ��ʶ����; ����������ͨ��ʶ����ʱ,ֵΪ��ָ��NULL
 * @param pLight {NJUST_LS_LIGHT_RES_TO_FU **} [out] ָ��ͨ������;����������ͨ������ʱ,ֵΪ��ָ��NULL
 * @param pInf {NJUST_LS_INF_TO_MO **} [out] ָ�������Ϣ; ��������������Ϣʱ,ֵΪ��ָ��NULL
 * @param pImg {NJUST_LS_IMG_TO_MO **} [out] ָ��ͼ������; ��������ͼ������ʱ,ֵΪ��ָ��NULL
 * @return {int}, �ɹ�����0, ����-1
 */
int NJUST_LS_Read_offline_data( const char* pFileName, //����ȡ�������ļ���
                                NJUST_LS_SIGN_RES_TO_FU   **pSign, //����������ͨ��ʶ����ʱ,ֵΪNULL
                                NJUST_LS_LIGHT_RES_TO_FU  **pLight, //����������ͨ������ʱ,ֵΪNULL
							    NJUST_LS_INF_TO_MO   **pInf, //��������������Ϣʱ,ֵΪNULL
							    NJUST_LS_IMG_TO_MO   **pImg  //��������ͼ������ʱ,ֵΪNULL
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
int NJUST_LS_Write_offline_data( const void* pIPData,  //���������IP���ݽṹ��ͬ�����ݿ�
							     const int   nBytes,   //pIPData���ݵ��ֽڸ���							   
							  	 const char* pFileName //�洢���ݵ��ļ���
						       );

#ifdef __cplusplus
}
#endif

#endif
