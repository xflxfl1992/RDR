#ifndef XFL_LIDAR64_DEF_H
#define XFL_LIDAR64_DEF_H
 
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 开关配置
//
////////////////////////////////////////////////////////////////////////////////////////////////////
// switch for storing data
 #define ISDEBUG false   //debug model or release model 
// whether use NavID count or not
 #define _USE_NAV_ID_COUNT_   true //true:使用惯导号记录，false:使用内部计数器记录
// whether use XFL_DebugFile
// #define _USE_XFL_DEBUG_FILE_
//测试壕沟
//#define _TEST_MEND_

// switch for Environment Para
 #define _USE_BYD_PARAMETERS_ //内参文件开关
// #define _USE_DM_PARAMETERS_ //内参文件开关
//  #define _USE_MMR_PARAMETERS_ //内参文件开关
 
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 端口和模型设置
//
////////////////////////////////////////////////////////////////////////////////////////////////////
 #define PIDIVIDED       0.0174533  //   PI/180
 #define UDP_LIDAR_IP    "192.168.3.43"  //Lidar 获取地址
 #define UDP_LIDAR_PORT  2368   //lidar获取数据端口号

 //#define MODEL_IP "192.168.0.40"//model's IP
 //#define MODEL_PORT 8288//model's port
 #define FU_MODEL "FU"  //FU模块
 #define MO_MODEL "MO"  //MO模块
 #define MC_MODEL "MC"	//MC模块
 #define CAMERA_MODEL "2D" //2D模块
 #define IR_MODEL "IR" //PL模块
 #define PL_MODEL "PL" //PL模块


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 雷达原始数据定义
//
////////////////////////////////////////////////////////////////////////////////////////////////////
//扫描头类型
 #define LIDAR64_LOWER_HEADER_TYPE            0xDDFF //该雷达的下扫描头标记,即56831
 #define LIDAR64_UPPER_HEADER_TYPE            0xEEFF //该雷达的上扫描头标记,即61183   
 //数据包大小
 #define LIDAR64_SCANLINE_NUM_IN_SECTOR       32  //包含上层或下层的32个头的信息
 #define LIDAR64_DATA_SECTOR_SIZE			 (2+2+LIDAR64_SCANLINE_NUM_IN_SECTOR*3) //2字节头信息，2字节旋转角，32*3个字节的距离和辐射强度信息
 #define LIDAR64_IP_DATA_SIZE                 ((12*LIDAR64_DATA_SECTOR_SIZE)+6) //1个IP包的大小,含有12个小数据包，最后6字节为旋转计数和固件信息
 #define LIDAR64_MAX_IP_DATA_NUM_ONE_FRAME    (348+10)  //每转一圈,最多348个IPData.但曾经发现一帧多于350以上,可能是雷达转的慢.
 #define LIDAR64_MAX_IP_DATA_SIZE             (LIDAR64_IP_DATA_SIZE*4) //1个IP包的最大大小
 //线号相关
 #define XFL_3D64_HEAD_NUM                   64  //总共的线数
 #define XFL_SINGLE_LINE_MAX_POINT_NUM       2200 //一根线在一帧中最多打出2200个点
 #define XFL_3D64_MAX_POINT_NUM              (XFL_3D64_HEAD_NUM*XFL_SINGLE_LINE_MAX_POINT_NUM) //一帧图像中有效点最高个数
 //其他
 #define MAX_NEGATIVE_OBASTACLE_NUM          100  //一帧中最多的负障碍数
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// LINUX下
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
