////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_FU_data.h
//  Author: 刘华军
//  Date:   2015.6.6
//  Description: 融合模块给规划模块的数据
//  Modification:
//          2015.7.3, 任明武
//  Declare:
//          NJUST_FU_TO_PL
//          NJUST_FU_DBGINFO
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_FU_DATA_H__
#define _NJUST_FU_DATA_H__

#include "NJUST_Global_Def.h"
#include "NJUST_IP_comm.h"
#include "NJUST_3D64_data.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  设置对齐方式
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push) //保存对齐状态
#pragma pack(1)    //按1字节对齐,必须放在第一句

//融合宏定义
#define NJUST_FU_MAX_POINTS_IN_LINE				20		//一条道边上的最多点数
#define NJUST_FU_MAX_LINE_NUM					8		//单侧道路最多的边数
#define NJUST_FU_MAX_OBJ_NUM					450		//单帧最多障碍数
#define NJUST_FU_MAX_DBG_INFO_LEN				1024	//调试信息最大字节数

#define NJUST_FU_MAX_TRAFFIC_SIGN_NUM			20    //FU交通标志类型总数



/*!< 行道线颜色 */
enum  NJUST_FU_LANE_LINE_COLOR
{
	NJUST_FU_LINE_COLOR_UNKNOWN = 0x00,  //未知
	NJUST_FU_LINE_COLOR_WHITE,	         //白色
	NJUST_FU_LINE_COLOR_YELLOW,          //黄色
	NJUST_FU_LINE_COLOR_RED,             //红色
	NJUST_FU_LANE_LINE_COLOR_NUM	     //类型总数
};

/*!< 行道线类型 */
enum  NJUST_FU_LANE_LINE_TYPE
{
	NJUST_FU_LINE_TYPE_UNKNOWN = 0x00,   //未知
	NJUST_FU_LINE_TYPE_SSOLID,	        //单实线
	NJUST_FU_LINE_TYPE_SDASHED,			//单虚线
	NJUST_FU_LINE_TYPE_DSOLID,			//双实线
	NJUST_FU_LINE_TYPE_DDASHED,			//双虚线
	NJUST_FU_LINE_TYPE_SOLID_DASHED,    //虚实线
	NJUST_FU_LINE_TYPE_NUM      	    //类型总数
};

/*!< 障碍类型 */
enum  NJUST_FU_OBS_TYPE
{
	NJUST_FU_OBS_TYPE_UNKNOWN = 0x00,    	//未知
	NJUST_FU_OBS_TYPE_VEHICLE,	        	//汽车
	NJUST_FU_OBS_TYPE_PEDESTRIAN,			//行人
	NJUST_FU_OBS_TYPE_NEGATIVE_OBS,			//负障碍
	NJUST_FU_OBS_TYPE_WATER_OBS,			//水坑
	NJUST_FU_OBS_TYPE_GENERAL,				//一般通用
	NJUST_FU_OBS_TYPE_NUM         	        //类型总数
};

/*!< 融合程序状态 */
enum NJUST_FU_STATE
{
	NJUST_FU_STATE_WAIT = 0x00,			//就绪
	NJUST_FU_STATE_ROAD_NAV,			//道路自主
	NJUST_FU_STATE_CROSS_NAV,			//路口通行
	NJUST_FU_STATE_STRAIGHT,			//直行
	NJUST_FU_STATE_TURN_LEFT,			//左转
	NJUST_FU_STATE_TURN_RIGHT,			//右转
	NJUST_FU_STATE_U_TURN,				//掉头
	NJUST_FU_STATE_STOP,				//停车
	NJUST_FU_STATE_NUM         	        //类型总数
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  融合---行道线
//
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_FU_POINT
{
	int														x_cm;						/* x 方向位置，单位：cm */
	int														y_cm;						/* y 方向位置，单位：cm */
}NJUST_FU_POINT;

typedef struct tagNJUST_FU_LINE
{
	int														id;							/*!< 标识号. */
	NJUST_FU_POINT											pPoint[NJUST_FU_MAX_POINTS_IN_LINE];		/*!< 构成线的点集. */
	int														nPoint;         			/*!< 点集中实际点数量. */
	NJUST_FU_LANE_LINE_TYPE									type;						/*!< 行道线类型. */
	NJUST_FU_LANE_LINE_COLOR								color;						/*!< 行道线颜色. */
	int														reliability;                /*!< 可信度[0-100]. */
} NJUST_FU_LINE;

typedef struct tagNJUST_FU_LANE
{
	int										    			err;					/*!<	0--无错；1--有错或无效*/
	NJUST_FU_LINE											pLeftLines[NJUST_FU_MAX_LINE_NUM];		/*!< 左侧行道线. */
	NJUST_FU_LINE											pRightLines[NJUST_FU_MAX_LINE_NUM];		/*!< 右侧行道线. */
	int														nLeftLine;			/*!< 左侧实际行道线数量. */
	int														nRightLine;			/*!< 右侧实际行道线数量. */
} NJUST_FU_LANE;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  融合---自然道边
//
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_FU_BOUNDLINE
{
	int 													nPoint;									/*!< 边界线上点数量 */
	NJUST_FU_POINT 											pPoint[NJUST_FU_MAX_POINTS_IN_LINE];	/*!< 边界线上点坐标 */
}NJUST_FU_BOUNDARYLINE;

typedef struct tagNJUST_FU_NATURALBOUNDARY
{
	int											     		err;			        /*!<	0--无错；1-- 有错或无效*/
	NJUST_FU_BOUNDARYLINE									left_boundary;			/*!< 左侧自然道边. */
	NJUST_FU_BOUNDARYLINE									right_boundary;			/*!< 右侧自然道边. */
	int														widthCM;				/*!< 单位：厘米，自然道边宽度. */
	int														left_reliability;	    /*!< 左侧道边可信度[0-100]. */
	int														right_reliability;		/*!< 右侧道边可信度[0-100]. */
	int														width_reliability;		/*!< 宽度可信度[0-100]. */
} NJUST_FU_NATURALBOUNDARY;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  融合---停止线
//
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_FU_STOPLINE{
	int										    	err;				    /*!<	1--有效；0-- 有错或无效*/
	NJUST_FU_POINT									left;					/*!< 停止线左边点. */
	NJUST_FU_POINT									right;					/*!< 停止线右边点. */
}NJUST_FU_STOPLINE;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  融合---斑马线
//
////////////////////////////////////////////////////////////////////////////////////////////////
/*!< 四边形 */
typedef struct tagNJUST_FU_QUAD
{
	NJUST_FU_POINT									left_up;				/*!< 左上角. */
	NJUST_FU_POINT									left_down;				/*!< 左下角. */
	NJUST_FU_POINT									right_up;				/*!< 右上角. */
	NJUST_FU_POINT									right_down;				/*!< 右下角. */
}NJUST_FU_QUAD;

/*!< 斑马线. */
typedef struct tagNJUST_FU_ZEBRALINE
{
	int										    	err;		    /*!<	1--有效；0-- 有错或无效*/
	NJUST_FU_QUAD   								quad;           /*!< 斑马线四个角点. */
}NJUST_FU_ZEBRALINE;


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  融合---前方障碍
//
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_FU_SPEED
{
	int 													x_cmps; 					/* x 方向速度，单位：cm/s */
	int 													y_cmps; 	  				/* y 方向速度，单位：cm/s */
}NJUST_FU_SPEED;

typedef struct tagNJUST_FU_OBS_INFO
{
	int														id;   					/*标识号(从0开始)*/
	NJUST_FU_OBS_TYPE										type;   				/*类型*/
	int														reliability; 	        /*可信度[0-100]*/
	NJUST_FU_QUAD											area;		  			/*目标四边形区域*/
	NJUST_FU_SPEED											speed;					/*目标二维速度*/
}NJUST_FU_OBS_INFO;

typedef struct tagNJUST_FU_OBS
{
	int										    			err;					/*0--无错；非0--有错或无效*/
	NJUST_FU_OBS_INFO										pObj[NJUST_FU_MAX_OBJ_NUM];		/*目标集合*/
	int 													nObj;					/*有效目标数量*/
}NJUST_FU_OBS;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  融合---交通标志
//
////////////////////////////////////////////////////////////////////////////////////////////////
enum NJUST_FU_DRIVABLE_FLAG
{
	UNKOWN = 0x00,
	FORBID,
	PERMIT
};

/*融合，岔路口信息*/
typedef struct tagNJUST_FU_CROSSROAD
{
	NJUST_FU_POINT guide_point;	// 岔路口可通行区域的导引点，世界坐标
	double a, b, c;				// 岔路口可通行区域中心线(直线)参数，a*x + b*y + c = 0，车体坐标
}NJUST_FU_CROSSROAD;


/*!< 交通方向标识 */
typedef struct tagNJUST_FU_TRAFFIC_DIRECTION
{
	int err;
	NJUST_FU_DRIVABLE_FLAG Straight;
	NJUST_FU_DRIVABLE_FLAG Left;
	NJUST_FU_DRIVABLE_FLAG Right;
	NJUST_FU_DRIVABLE_FLAG Uturn;
}NJUST_FU_TRAFFIC_DIRECTION;

/*!< 特殊交通标识牌 */
enum NJUST_FU_TRAFFIC_SIGN
{
	NJUST_FU_SIGN_TYPE_FORBID_DRIVE_INTO = 0x00,    //禁止驶入
	NJUST_FU_SIGN_TYPE_FORBID_STOP,                         //禁止停车
	NJUST_FU_SIGN_TYPE_SLOW_DOWN,                          //减速行驶
	NJUST_FU_SIGN_TYPE_PEDESTRIAN,                           //注意行人
	NJUST_FU_SIGN_TYPE_UNEVEN_ROAD,                       //路面不平
	NJUST_FU_SIGN_TYPE_EMERGENCY_STOP,                  //紧急停车

	NJUST_FU_SIGN_TYPE_ALONG_RIGHT_SIDE,                //沿右侧行驶
	NJUST_FU_SIGN_TYPE_ALONE_LEFT_SIDE,                    //沿左侧行驶
	NJUST_FU_SIGN_TYPE_HOKING,                                  //HOKING
	NJUST_FU_SIGN_TYPE_DRIVE_AROUND_ISLAND,          //沿岛行驶
	NJUST_FU_SIGN_TYPE_CROSSWALK,

	NJUST_FU_SIGN_TYPE_PARKING,                                //停车标志
	NJUST_FU_SIGN_TYPE_TRAFFIC_MARK_CONE,
	NJUST_FU_SIGN_TYPE_LEFT_FORWORD,
	NJUST_FU_SIGN_TYPE_RIGHT_FORWORD,
	NJUST_FU_SIGN_TYPE_CHILD,
	NJUST_FU_SIGN_TYPE_CONSTRUCTION,
	NJUST_FU_SIGN_TYPE_SPEED_LIMIT,
	NJUST_FU_SIGN_TYPE_NUM
};

/*!< 融合交通标识 */
typedef struct tagNJUST_FU_TRAFFIC
{
	int err;
	NJUST_FU_TRAFFIC_DIRECTION Trafficlight_Direction;      //交通灯指示的行驶方向
	NJUST_FU_TRAFFIC_DIRECTION Trafficsign_Direction;       //交通标志指示的行驶方向
	NJUST_FU_TRAFFIC_SIGN pSignType[NJUST_FU_MAX_TRAFFIC_SIGN_NUM];     //其他交通标志
	int pSignValue[NJUST_FU_MAX_TRAFFIC_SIGN_NUM];      //其他交通标志对应值
	int nSignNum;
}NJUST_FU_TRAFFIC;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  融合Debug信息
//
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_FU_PROGSTATE
{
	int                          fu_2d_freq;				//2D帧频，单位：Hz
	int                          fu_3d_freq;				//3D帧频，单位：Hz
	int                          fu_ls_freq;				//LS帧频，单位：Hz
	int                          fu_mc_freq;				//MC帧频，单位：Hz
	int                          fu_fu_freq;				//FU帧频，单位：Hz
}NJUST_FU_PROGSTATE;

typedef struct tagNJUST_FU_DBGINFO
{
	int															SN; 				//融合序列号
	NJUST_IP_TIME 												synTime;			//系统时间
	NJUST_FU_PROGSTATE											prog_state;			//程序运行状态
	char														pInfo[NJUST_FU_MAX_DBG_INFO_LEN + 1];			//调试信息
	int															nInfo; 				//调试信息含字符个数
	int															nSize;				//该结构体大小
}NJUST_FU_DBGINFO;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  融合结果
//
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagNJUST_FU_RESLUT
{
	int												SN;				/*!< 融合序列号(从0开始).*/
	NJUST_IP_TIME									synTime;		/*!< 融合输出时刻 .*/
	int												navID;			/*!< 最接近的导航数据编号(从0开始) */
	NJUST_FU_STATE									status;			/*状态循环中的状态量*/
	NJUST_FU_LANE									line;       	/*!< 行道线 */
	NJUST_FU_NATURALBOUNDARY						boundary;	 	/*!< 自然道边 */
	NJUST_FU_STOPLINE								stopline;  		/*!< 停止线 */
	NJUST_FU_ZEBRALINE								zebraline;		/*!< 斑马线 */
	NJUST_FU_TRAFFIC								traffic;		/*!< 交通标志（标志+信号灯）*/
	int                                             courseAngleRad; /*!< 航向角,单位:0.001,千分之一弧度 */
	NJUST_FU_POINT                                  earthCoord;     /*!< 车体的大地坐标 */
	NJUST_FU_OBS								    quadObs;	 	/*!< 运动的障碍物集合*/
	NJUST_FU_CROSSROAD								crossRoad;		/*!<岔路口引导点和引导线*/
	BYTE gridMsk[NJUST_LIDAR64_VERTICAL_GRID_NUM*NJUST_LIDAR64_HORIZONTAL_GRID_NUM]; //障碍物栅格(包含静止和运动障碍)

	//结构体大小
	int                                             nSize;          /*!< 结构体大小 */
	//检查和
	int                                             checksum;       /*!< Checksum of above data */
}NJUST_FU_TO_PL;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  恢复对齐方式
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)//恢复对齐状态

#endif
