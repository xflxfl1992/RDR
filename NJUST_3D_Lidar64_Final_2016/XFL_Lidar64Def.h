#ifndef XFL_LIDAR64_DEF_H
#define XFL_LIDAR64_DEF_H
 
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��������
//
////////////////////////////////////////////////////////////////////////////////////////////////////
// switch for storing data
 #define ISDEBUG false   //debug model or release model 
// whether use NavID count or not
 #define _USE_NAV_ID_COUNT_   true //true:ʹ�ùߵ��ż�¼��false:ʹ���ڲ���������¼
// whether use XFL_DebugFile
// #define _USE_XFL_DEBUG_FILE_
//���Ժ���
//#define _TEST_MEND_

// switch for Environment Para
// #define _USE_BYD_PARAMETERS_ //�ڲ��ļ�����
 #define _USE_DM_PARAMETERS_ //�ڲ��ļ�����
 
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �˿ں�ģ������
//
////////////////////////////////////////////////////////////////////////////////////////////////////
 #define PIDIVIDED       0.0174533  //   PI/180
 #define UDP_LIDAR_IP    "192.168.3.43"  //Lidar ��ȡ��ַ
 #define UDP_LIDAR_PORT  2368   //lidar��ȡ���ݶ˿ں�

 #define MODEL_IP "192.168.0.40"//model's IP
 #define MODEL_PORT 8288//model's port
 #define FU_MODEL "FU"  //FUģ��
 #define MO_MODEL "MO"  //MOģ��
 #define MC_MODEL "MC"	//MCģ��
 #define CAMERA_MODEL "2D" //2Dģ��
 #define IR_MODEL "IR" //PLģ��
 #define PL_MODEL "PL" //PLģ��


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �״�ԭʼ���ݶ���
//
////////////////////////////////////////////////////////////////////////////////////////////////////
//ɨ��ͷ����
 #define LIDAR64_LOWER_HEADER_TYPE            0xDDFF //���״����ɨ��ͷ���,��56831
 #define LIDAR64_UPPER_HEADER_TYPE            0xEEFF //���״����ɨ��ͷ���,��61183   
 //���ݰ���С
 #define LIDAR64_SCANLINE_NUM_IN_SECTOR       32  //�����ϲ���²��32��ͷ����Ϣ
 #define LIDAR64_DATA_SECTOR_SIZE			 (2+2+LIDAR64_SCANLINE_NUM_IN_SECTOR*3) //2�ֽ�ͷ��Ϣ��2�ֽ���ת�ǣ�32*3���ֽڵľ���ͷ���ǿ����Ϣ
 #define LIDAR64_IP_DATA_SIZE                 ((12*LIDAR64_DATA_SECTOR_SIZE)+6) //1��IP���Ĵ�С,����12��С���ݰ������6�ֽ�Ϊ��ת�����͹̼���Ϣ
 #define LIDAR64_MAX_IP_DATA_NUM_ONE_FRAME    (348+10)  //ÿתһȦ,���348��IPData.����������һ֡����350����,�������״�ת����.
 #define LIDAR64_MAX_IP_DATA_SIZE             (LIDAR64_IP_DATA_SIZE*4) //1��IP��������С
 //�ߺ����
 #define XFL_3D64_HEAD_NUM                   64  //�ܹ�������
 #define XFL_SINGLE_LINE_MAX_POINT_NUM       2200 //һ������һ֡�������2200����
 #define XFL_3D64_MAX_POINT_NUM              (XFL_3D64_HEAD_NUM*XFL_SINGLE_LINE_MAX_POINT_NUM) //һ֡ͼ������Ч����߸���
 //����
 #define MAX_NEGATIVE_OBASTACLE_NUM          100  //һ֡�����ĸ��ϰ���
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// LINUX��
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _NJUST_OS_USE_LINUX_
   #include <unistd.h>
   #include <sys/socket.h>
   #include <netinet/in.h>
   #include <sys/types.h>
   #include <arpa/inet.h>
   #include <linux/string.h> //memset
   #include <fcntl.h>
   #include <sys/time.h>
   #include "unistd.h"
   #include <time.h>
   #include <pthread.h>
   #include <sched.h>
   #define INVALID_SOCKET -1
   #define SOCKET_ERROR -1
#endif

#endif
