////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, �Ͼ�����ѧ�������ѧ�빤��ѧԺ, ���ܿ�ѧ�뼼��ϵ
//  FileName:  NJUST_MC_proc.h
//  Author: ����Ԫ
//  Date:   2015.6.26
//  Description: ����ײ����ģ������ݵĺ���
//  Modification: 
//          2015.7.3, ������
//  Functions:
//
//           int NJUST_MC_Decode_IP_Data( const void* pIPData, const int nBytes,
//                                        NJUST_MC_STATE_INFO  **pState, //������״̬����ʱ,ֵΪNULL
//							              NJUST_MC_NAV_INFO  **pNav, //�����ǵ�����Ϣʱ,ֵΪNULL
//							              NJUST_MC_DRIVE_INFO  **pDrive  //������ִ������ʱ,ֵΪNULL
//						                );
//
//           int NJUST_MC_Read_offline_data( const char* pFileName, //����ȡ�������ļ���
//                                           NJUST_MC_STATE_INFO  **pState, //������״̬����ʱ,ֵΪNULL
//							                 NJUST_MC_NAV_INFO  **pNav, //�����ǵ�����Ϣʱ,ֵΪNULL
//							                 NJUST_MC_DRIVE_INFO  **pDrive  //������ִ������ʱ,ֵΪNULL
//						                   );
//
//           int NJUST_MC_Write_offline_data( const void* pIPData,  //���������IP���ݽṹ��ͬ�����ݿ�
//							                  const int   nBytes,   //pIPData���ݵ��ֽڸ���							   
//							  	              const char* pFileName //�洢���ݵ��ļ���
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
//  �ṩ��Ŀ��ģ��Ľ��뺯��
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief �ṩ��Ŀ��ģ��Ľ��뺯��
 * @param pIPData {const void*} [in] IP����,Ŀ��ģ��������յ�������
 * @param nBytes {const int} [in] pIPData���ݵ��ֽڸ���
 * @param pState {NJUST_MC_STATE_INFO **} [out] ָ��״̬����; ����״̬����ʱ,ֵΪ��ָ��NULL
 * @param pNav {NJUST_MC_NAV_INFO **} [out] ָ�򵼺���Ϣ; ���ǵ�����Ϣʱ,ֵΪ��ָ��NULL
 * @param pDrive {NJUST_MC_DRIVE_INFO **} [out] ָ��ִ������; ����ִ������ʱ,ֵΪ��ָ��NULL
 * @return {int}, �ɹ�����0, ����-1
 */
int NJUST_MC_Decode_IP_Data( const void* pIPData,  //IP����,Ŀ��ģ��������յ�������
							 const int nBytes,     //pIPData���ݵ��ֽڸ���
                             NJUST_MC_STATE_INFO  **pState, //������״̬����ʱ,ֵΪNULL
				             NJUST_MC_NAV_INFO  **pNav, //�����ǵ�����Ϣʱ,ֵΪNULL
							 NJUST_MC_DRIVE_INFO  **pDrive  //������ִ������ʱ,ֵΪNULL
						   );
						   
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ��ȡ���������ļ�
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ��ȡ���������ļ�
 * @param pFileName {const char*} [in] ����ȡ�������ļ���
 * @param pState {NJUST_MC_STATE_INFO **} [out] ָ��״̬����; ����״̬����ʱ,ֵΪ��ָ��NULL
 * @param pNav {NJUST_MC_NAV_INFO **} [out] ָ�򵼺���Ϣ; ���ǵ�����Ϣʱ,ֵΪ��ָ��NULL
 * @param pDrive {NJUST_MC_DRIVE_INFO **} [out] ָ��ִ������; ����ִ������ʱ,ֵΪ��ָ��NULL
 * @return {int}, �ɹ�����0, ����-1
 */
int NJUST_MC_Read_offline_data( const char* pFileName, //����ȡ�������ļ���
                                NJUST_MC_STATE_INFO  **pState, //������״̬����ʱ,ֵΪNULL
				                NJUST_MC_NAV_INFO  **pNav, //�����ǵ�����Ϣʱ,ֵΪNULL
							    NJUST_MC_DRIVE_INFO  **pDrive  //������ִ������ʱ,ֵΪNULL
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
int NJUST_MC_Write_offline_data( const void* pIPData,  //���������IP���ݽṹ��ͬ�����ݿ�
							     const int   nBytes,   //pIPData���ݵ��ֽڸ���							   
							  	 const char* pFileName //�洢���ݵ��ļ���
 						       );

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  �ṩ��γ������ת����ƽ������
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief �ṩ��γ������ת����ƽ�����꺯��
 * @param Longitude_degree {double} [in] ����ľ��� ��λ ��
 * @param Latitude_degree  {double} [in] �����γ�� ��λ �� 
 * @param EarthRefCoord_x_m{double *} [out] ת�����ƽ������ֵ ����λ������ֵ ��λ ��
 * @param EarthRefCoord_y_m{double *} [out] ת�����ƽ������ֵ ����λ������ֵ ��λ ��
 * @return {int}, �ɹ�����0, ����-1
 */
int Conver_E_N(double Longitude_degree,
			   double Latitude_degree,
			   double *EarthRefCoord_x_m,
			   double *EarthRefCoord_y_m);

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  �ṩ����ƽ������ת��������ϵ
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief �ṩ����ƽ������ת��������ϵ����
 * @param in_yaw_rad {double *} [in] ����ת��ʱ�ĺ���� ��λ ����
 * @param TargetEarthCoord_x_m {double} [in] Ŀ����������xֵ(����λ������ֵ) ��λ ��
 * @param TargetEarthCoord_y_m {double} [in] Ŀ����������yֵ(����λ������ֵ) ��λ ��
 * @param CarEarthCoord_x_m {double} [in] ����������xֵ(����λ������ֵ) ��λ ��
 * @param CarEarthCoord_y_m {double} [in] ����������yֵ(����λ������ֵ) ��λ ��
 * @param out_Coord_B_x_cm {double *} [out] ת����ĳ�������xֵ ����������ֵ ��λ ����
 * @param out_Coord_B_y_cm {double *} [out] ת����ĳ�������yֵ ǰ��λ������ֵ ��λ ����						
 * @return {int}, �ɹ�����0, ����-1
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
