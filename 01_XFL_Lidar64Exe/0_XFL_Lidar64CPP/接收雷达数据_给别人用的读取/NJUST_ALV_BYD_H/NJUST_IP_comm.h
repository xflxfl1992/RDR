////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, �Ͼ�����ѧ�������ѧ�빤��ѧԺ, �������ϵ
//  FileName:  NJUST_IP_comm.h
//  Author: ����֡
//  Date:   2015.5.14
//  Description: ��ģ��֮�������ͨ�š�������
//  Modification: 
//          2015.6.26, ������
//  Modification: 
//          2015.8.5, ����
//  Modification: 
//          2015.8.8, ����֡ ����
//  Modification: 
//          2015.9.2, ����
//  Functions: 
//          ע��:
//          int NJUST_IP_set_moduleName( const char *pModuleName );
//          int NJUST_IP_moduleName_exist( const char* pModuleName );
//          ʱ��:
//          NJUST_IP_TIME NJUST_IP_get_time();
//          int NJUST_IP_get_timeStr( NJUST_IP_TIME t, char* pStr );
//			unsigned long long NJUST_IP_get_time_GAP_ms(NJUST_IP_TIME t1, NJUST_IP_TIME t2);     //add by zqc 2015/08/05
//          ͨ��:
//          int NJUST_IP_tcp_send_to( const char* pModuleName, const void* pData, const int nBytes );
//          int NJUST_IP_udp_send_to( const char* pModuleName, const void* pData, const int nBytes );
//          ����:
//          int NJUST_IP_set_tcp_callBack( const char* pModuleName, func_t func, void* arg );
//          int NJUST_IP_set_udp_callBack( const char* pModuleName, func_t func, void* arg );
//          int NJUST_IP_set_broadcast_callBack( func_t func, void* arg );
//			����
//			int NJUST_IP_req_pc_reboot();
//			int NJUST_IP_req_pc_poweroff();
//			int NJUST_IP_req_mod_reboot();
//			ѯ�ʣ�
//          int NJUST_IP_IsMemAvailable(const char *fileName);
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_IP_COMM_H_
#define _NJUST_IP_COMM_H_

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  ���ö��뷽ʽ
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push) //�������״̬
#pragma pack(1)    //��1�ֽڶ���,������ڵ�һ��

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  �ϰ汾
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef NJUST_IP_COMM_USE_OLD_BEFORE_20160826           //�ϰ汾

/** ģ������󳤶� */
#define NJUST_IP_MAX_MODULE_NAME_LEN          7

/** �ص��������� */
typedef int ( *func_t )( void*, size_t, void* );

/** ʱ������ */
typedef struct tagNJUST_IP_TIME
{
	int  ws;         //��λ����
	int  ms;        //����(��֤��������)
}NJUST_IP_TIME;


#ifdef __cplusplus
extern "C" {
#endif
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  �򱾻��ػ����̷���ע��
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief �򱾻��ػ����̷���ע��
 * @param pModuleName {const char*} [in] ģ����
 * @param reboot {int} [in] ����ģ�� 
 * @return {int}, �ɹ�����0, ����-1
 * @note �ú��������ڱ�ͨ�ſ����������ǰ����, ���ú����ٵ���fork()
 * @note ģ�����NJUST_IP_MAX_MODULE_NAME_LEN
 * @note �ػ����̶Գ���״̬���м�⣬��rebootΪ0��������쳣ʱ����������������쳣ʱ��������ģ�顣
 * @see NJUST_IP_moduleName_exist
 */
int NJUST_IP_set_moduleName( const char *pModuleName , int reboot);
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ��ѯģ���Ƿ����
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ��ѯģ���Ƿ����
 * @param pModuleName {int} [in] ģ����
 * @return {int}, ���ڷ���0, ����-1
 * @see NJUST_IP_set_moduleName
 */
int NJUST_IP_moduleName_exist( const char* pModuleName );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ��ȡʱ���
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ��ȡʱ���
 */
NJUST_IP_TIME NJUST_IP_get_time();
/**
 * @brief ��ʱ���ת�����ַ���
 * @param t {NJUST_IP_TIME} [in] ʱ���
 * @param pStr[24] {char} [out] �ַ���
*/
int NJUST_IP_get_timeStr( NJUST_IP_TIME t, char pStr[24] );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ͨ��TCPЭ�齫pDataָ���nBytes���ֽڷ�����ģ��pModuleName
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ͨ��TCPЭ�齫pDataָ���nBytes���ֽڷ�����ģ��pModuleName
 * @param pModuleName {const char*} [in] ģ����
 * @param pData {const void*} [in] ����ָ��
 * @param nBytes {const int} [in] ���ݳ���
 * @return {int}, �ɹ����ط����ֽ���, ����-1
 * @note ��pModuleNameΪ��, ֱ�ӷ���-1
 * @see NJUST_IP_udp_send_to
 */
int NJUST_IP_tcp_send_to( const char* pModuleName, const void* pData, const int nBytes );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ͨ��UDPЭ�齫pDataָ���nBytes���ֽڷ�����ģ��pModuleName
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ��pModuleNameΪ�ǿ� ��ͨ��UDPЭ�齫pDataָ���nBytes���ֽڷ�����ģ��pModuleName
 * @brief ��pModuleNameΪ��  ��ͨ���㲥Э�齫pDataָ���nBytes���ֽڷ�����ģ��pModuleName
 * @param pModuleName {const char*} [in] ģ��
 * @param pData {const void*} [in] ����ָ��
 * @param nBytes {const int} [in] ���ݳ���
 * @return {int}, �ɹ����ط����ֽ���, ʧ�ܷ���-1
 * @note ��pModuleNameΪ��, ֱ�ӷ��͹㲥��
 * @see NJUST_IP_tcp_send_to
 */
int NJUST_IP_udp_send_to( const char* pModuleName, const void* pData, const int nBytes );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ָ����ǰģ�����յ�pModuleNameͨ��TCP�ʹ�����ʱ�Ĵ�����func
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ָ����ǰģ�����յ�pModuleNameͨ��TCP�ʹ�����ʱ�Ĵ�����func
 * @param pModuleName {const char*} [in] ģ����
 * @param func {func_t} [in] ���ڴ������ݵĺ���
 * @param arg {void*} [in, out] �û�����ָ��
 * @return {int}, ����ʧ�ܷ���-1 ���óɹ�����ֵ���ڵ���0
 * @note �ú������´���һ���߳�, �������ĵ��������߳��н���, �����д����߳�ͬ��
 * @note ��δ���øú���, �Է�ģ���ڷ���ʱ��ֱ�ӷ��ش���
 * @see NJUST_IP_set_udp_callBack
 */
int NJUST_IP_set_tcp_callBack( const char* pModuleName, func_t func, void* arg );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ָ����ǰģ�����յ�pModuleNameͨ��UDP�ʹ�����ʱ�Ĵ�����func
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ָ����ǰģ�����յ�pModuleNameͨ��UDP�ʹ�����ʱ�Ĵ�����func
 * @param pModuleName {const char*} [in] ģ����
 * @param func {func_t} [in] ���ڴ������ݵĺ���
 * @param arg {void*} [in, out] �û�����ָ��
 * @return {int}, ����ʧ�ܷ���-1 ���óɹ�����ֵ���ڵ���0
 * @note �ú������´���һ���߳�, �������ĵ��������߳��н���, �����д����߳�ͬ��
 * @note ��δ���øú���, �Է�ģ���ڷ���ʱ��ֱ�ӷ��ش���
 * @see NJUST_IP_set_tcp_callBack
 */
int NJUST_IP_set_udp_callBack( const char* pModuleName, func_t func, void* arg );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ָ����ǰģ�����յ��㲥ʱ�Ĵ�����
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ָ����ǰģ�����յ��㲥��ʱ�Ĵ�����
 * @param func {func_t} [in] ���ڴ������ݵĺ���
 * @param arg {void*} [in, out] �û�����ָ��
 * @return {int}, ����ʧ�ܷ���-1,���򷵻�0
 * @note �ú������´���һ���߳�, �������ĵ��������߳��н���, �����д����߳�ͬ��
 * @note ��δ���øú���, �㲥��Ȼ���յ�, ��ֱ�Ӷ���
 * @see NJUST_IP_set_tcp_callBack,NJUST_IP_set_udp_callBack
 */
int NJUST_IP_set_broadcast_callBack( func_t func, void* arg );

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ��������ʱ����  
//  Add by ����   Date:2015/08/05
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ��������ʱ����
 * @param t1 {NJUST_IP_TIME} [in] ʱ��ṹ�� ����¼��1970/01/01 ����ǰʱ�̵ĺ�������
 * @param t2 {NJUST_IP_TIME} [in] ʱ��ṹ�� ����¼��1970/01/01 ����ǰʱ�̵ĺ�������
 * @return {unsigned long long}, ���غ�����֮�����ֵ��
 * @note �ú����᷵�غ�����֮�� �Ǿ���ֵ  
 */
unsigned long long NJUST_IP_get_time_GAP_ms(NJUST_IP_TIME t1, NJUST_IP_TIME t2);

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ģ��������������  
//  Add by ����֡   Date:2015/08/08
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief  ģ��������������
 * @return {int},�ɹ�����0
 */
int NJUST_IP_req_pc_reboot();

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ģ�����������ػ�
//  Add by ����֡   Date:2015/08/08
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief  ģ�����������ػ�
 * @return {int},�ɹ�����0
 */
int NJUST_IP_req_pc_poweroff();
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ģ����������
//  Add by ����֡   Date:2015/08/08
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief  ģ����������  
 * @return {int},�ɹ�����0
 */

int NJUST_IP_req_mod_reboot();
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ģ��ѯ���Ƿ��д����
//  Add by ����   Date:2015/09/02
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief  ģ��ѯ���Ƿ��д����  
 * @return {int},����0��ʾ��д   ���򷵻�1 ��ʾ����д
 * @example:     NJUST_IP_IsMemAvailable("/home"); �� ��home�� ���ÿռ����10Gʱ �᷵��0 ��ʾ��д
 * ���� ���� /home�´��������ļ� ��������д����
 */
int NJUST_IP_IsMemAvailable(const char *fileName);
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  �°汾
//
////////////////////////////////////////////////////////////////////////////////////////////////
#else  //�°汾
    #include <limits.h>

    typedef char               i8;
    typedef unsigned char      u8;

    #if USHRT_MAX == 0xffffU
	   typedef short              i16;
       typedef unsigned short     u16;
    #elif UINT_MAX == 0xffffU
       typedef int                i16;
       typedef unsigned int       u16;
    #elif ULONG_MAX == 0xffffU
        typedef long              i16;
        typedef unsigned long     u16;
    #endif

    #if UINT_MAX == 0xffffffffU
        typedef int                i32;
        typedef unsigned int       u32;
    #elif ULONG_MAX == 0xffffffffU
        typedef long               i32;
        typedef unsigned long      u32;
    #elif ULLONG_MAX == 0xffffffffU
        typedef long long          i32;
        typedef unsigned long long u32;
    #endif

    #if ULONG_MAX == 0xffffffffffffffffUL
        typedef long               i64;
        typedef unsigned long      u64;
    #elif ULLONG_MAX == 0xffffffffffffffffUL
        typedef long long          i64;
        typedef unsigned long long u64;
    #endif

/** ģ������󳤶� */
#define NJUST_IP_MAX_MODULE_NAME_LEN          7

/** �ص��������� */
typedef i32 ( *func_t )( void*, u32, void* );

/** ʱ������ */
typedef struct tagNJUST_IP_TIME
{
	i32  s;         //��λ����
	i32  ms;        //����(��֤��������)
} NJUST_IP_TIME;


#ifdef __cplusplus
extern "C" {
#endif
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  �򱾻��ػ����̷���ע��
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief �򱾻��ػ����̷���ע��
 * @param pModuleName {const i8*} [in] ģ����
 * @param reboot {i32} [in] ����ģ��
 * @param msec {i32} [in] �����ӳ�ʱ�䣨��λ��ms��
 * @return {i32}, �ɹ�����0, ����-1
 * @note �ú��������ڱ�ͨ�ſ����������ǰ����, ���ú����ٵ���fork()
 * @note ģ�����NJUST_IP_MAX_MODULE_NAME_LEN
 * @note �ػ����̶Գ���״̬���м�⣬��rebootΪ0��������쳣ʱ����������������쳣ʱ��������ģ�顣
 * @note ����ģ��ʱ���ӳ�ʱ�䣬������Ϊ��100~200ms֮�䣩,��reboot=0����ò���û���κ�����
 */
i32 NJUST_IP_set_moduleName( const i8* pModuleName, i32 reboot, i32 msec );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ��ѯģ���Ƿ����
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ��ѯģ���Ƿ����
 * @param pModuleName {const i8*} [in] ģ����
 * @return {i32}, ���ڷ���0, ����-1
 * @note һ������²����������
 */
i32 NJUST_IP_moduleName_exist( const i8* pModuleName );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ��ȡʱ���
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ��ȡʱ���
 */
NJUST_IP_TIME NJUST_IP_get_time();
/**
 * @brief ��ʱ���ת�����ַ���
 * @param t {NJUST_IP_TIME} [in] ʱ���
 * @param pStr[24] {i8} [out] �ַ���
 * @return {int}, �ɹ�����0 
*/
i32 NJUST_IP_get_timeStr( NJUST_IP_TIME t, i8 pStr[24] );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ��������ʱ����  
//  
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ��������ʱ����
 * @param t1 {NJUST_IP_TIME} [in] ʱ��ṹ�� ����¼��1970/01/01 ����ǰʱ�̵ĺ�������
 * @param t2 {NJUST_IP_TIME} [in] ʱ��ṹ�� ����¼��1970/01/01 ����ǰʱ�̵ĺ�������
 * @return {unsigned long long}, ���غ�����֮�����ֵ��
 * @note �ú����᷵�غ�����֮�� �Ǿ���ֵ  
 */
u64 NJUST_IP_get_time_GAP_ms( NJUST_IP_TIME t1, NJUST_IP_TIME t2 );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ��pDataָ���nBytes���ֽڷ�����ģ��pModuleName
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ��pDataָ���nBytes���ֽڷ�����ģ��pModuleName
 * @param pModuleName {const i8*} [in] ģ����
 * @param pData {const void*} [in] ����ָ��
 * @param nBytes {const u32} [in] ���ݳ���
 * @return {i32} �ɹ��򷵻ط��͵��ֽ�����ʧ���򷵻ظ�ֵ��
 * @note ��pModuleName=null��*pModuleName=0, ���͹㲥�����return 0.
 */
i32 NJUST_IP_send_to( const i8* pModuleName, const void* pData, const u32 nBytes );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ָ����ǰģ�����յ�pModuleName�ʹ�����ʱ�Ĵ�����func
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ָ����ǰģ�����յ�pModuleName�ʹ�����ʱ�Ĵ�����func
 * @param pModuleName {const i8*} [in] ģ����
 * @param func {func_t} [in] ���ڴ������ݵĺ���
 * @param arg {void*} [in] ���������Ĳ���
 * @return {i32}, ����ʧ�ܷ���-1 ���óɹ�����ֵ����0
 * @note �ú������´���һ���߳�, �������ĵ��������߳��н���, �����д����߳�ͬ��
 * @note ��δ���øú���, �Է�ģ���ڷ���ʱ��ֱ�ӷ��ش���
 */
i32 NJUST_IP_set_callBack( const i8* pModuleName, func_t func, void* arg );

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ָ����ǰģ�����յ��㲥ʱ�Ĵ�����
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief ָ����ǰģ�����յ��㲥��ʱ�Ĵ�����
 * @param func {func_t} [in] ���ڴ������ݵĺ���
 * @param arg {void*} [in, out] �û�����ָ��
 * @return {i32}, ����ʧ�ܷ���-1,���򷵻�0
 * @note �ú������´���һ���߳�, �������ĵ��������߳��н���, �����д����߳�ͬ��
 * @note ��δ���øú���, �㲥��Ȼ���յ�, ��ֱ�Ӷ���
 */
i32 NJUST_IP_set_brd_callBack( func_t func, void* arg );
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ģ��������������  
//  
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief  ģ��������������
 * @return {i32},�ɹ�����0
 */
i32 NJUST_IP_req_pc_reboot();
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ģ�����������ػ�
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief  ģ�����������ػ�
 * @return {i32},�ɹ�����0
 */
i32 NJUST_IP_req_pc_poweroff();
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ģ����������
//
////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief  ģ����������  
 * @return {i32},�ɹ�����0
 */
i32 NJUST_IP_req_mod_reboot();

#endif

#ifdef __cplusplus
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  �ָ����뷽ʽ
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//�ָ�����״̬


#endif /** _NJUST_IP_COMM_H_ */


