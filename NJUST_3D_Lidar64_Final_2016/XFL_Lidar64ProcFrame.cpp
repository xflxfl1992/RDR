#include "NJUST_ALV_BYD_H/ALV_BYD.h"
#include "XFL_Lidar64ProcFrame.h"
#include "XFL_Lidar64Extern.h"
#include "XFL_Lidar64Tools.h"
/////////////////////////////////////////////////////////////////////////////////////
//
//
//   雷达算法处理线程（ProcFrameThrd）
//
//
/////////////////////////////////////////////////////////////////////////////////////	

/////////////////////////////////////////////////////////////////////////////////////
// 
//   静态全局变量
//
/////////////////////////////////////////////////////////////////////////////////////
static RMW_LIDAR_3D_POINT_ORG gData[XFL_3D64_MAX_POINT_NUM];//一帧数据保存在这里
static RMW_LIDAR_3D_POINT_RES g3DData[XFL_SINGLE_LINE_MAX_POINT_NUM];//用于取出障碍物段的空间
extern void XFL_DebugFile(int i,char *pStr);//Debug

/////////////////////////////////////////////////////////////////////////////////////
// 
//   XFL_Lidar64ProcFrame构造析构函数
//
/////////////////////////////////////////////////////////////////////////////////////
XFL_Lidar64ProcFrame::XFL_Lidar64ProcFrame()
{
	lid_pack_id = 0;

	m_isForward = true;//一般初始为true，由于默认车往前开
	m_isForward_StatusChange = false;//车辆行驶行驶状态发生改变的flag
	m_isObjDetect = false;//是否进入侦查模式,默认为false
	m_UseNegativeObsDetect = true;//是否要使用负障碍，默认为true
	m_isStructuredRoad = false; //是否是结构化道路,路边比路面高者(有草或者路牙)尽可以当做结构化道路
	m_HasObstacle = false;//用于负障碍检测时，用正障碍验证的模式，初始化为false
	m_DataNum_InOneFrame = 0;

	memset(&m_3D64Obs,0,sizeof(NJUST_3D64_OBS_TO_FU));
	memset(&m_3D64Boundary,0,sizeof(NJUST_3D64_ROAD_BOUNDARY_TO_FU));
	memset(&m_3D64Grid,0,sizeof(NJUST_3D64_GRID_TO_FU));
	memset(&m_3D64Cross,0,sizeof(NJUST_3D64_CROSS_TO_FU));
	memset(&m_3D64TargetDetect,0,sizeof(NJUST_3D64_TARGET_TO_2D));
	memset(&gData,0,sizeof(RMW_LIDAR_3D_POINT_ORG)*XFL_3D64_MAX_POINT_NUM);

	//初始化配置算法参数
	Initialize_ImgInput( &m_isForward,
						 //栅格化
						 &m_gridXcm , //栅格水平方向的精度,单位cm
						 &m_gridYcm , //栅格垂直方向的精度,单位cm
						 &m_widthCM , //栅格图像的宽度,单位cm
						 &m_beginGridDistCM, //栅格图像的起始距离,单位cm
						 &m_endGridDistCM, //栅格图像的终止距离,单位cm
						//道边
						 &m_beginBoundaryDistCM, //边界栅格图像的起始距离,单位cm
		                 &m_endBoundaryDistCM, //边界栅格图像的终止距离,单位cm
						 &m_minRoadWidthCM, //道路的最小宽度
						 &m_maxRoadWidthCM, //道路的最大宽度
						 //正障碍
						 &m_minObstacleWCM,//正障碍的最小宽度
						 &m_minObstacleHCM, //正障碍的最小长度
						 //负障碍
						 &m_minNegativeObstacleWCM, //负障碍的最小宽度
						 &m_maxNegativeObstacleWCM,  //负障碍的最大宽度
						 //岔路口的处理区域
						 &m_beginObstacleDistCM, //栅格图像的起始距离,单位cm
						 &m_endObstacleDistCM //栅格图像的终止距离,单位cm
					  );
	if (m_isForward)
	{
		RmwLidarObstacleBoundary_Initialize( m_gridXcm , //栅格水平方向的精度,单位cm
											 m_gridYcm , //栅格垂直方向的精度,单位cm
											 m_widthCM , //栅格图像的宽度,单位cm
											 m_beginGridDistCM, //栅格图像的起始距离,单位cm
											 m_endGridDistCM, //栅格图像的终止距离,单位cm
											 //道边
											 m_beginBoundaryDistCM, //边界栅格图像的起始距离,单位cm
											 m_endBoundaryDistCM, //边界栅格图像的终止距离,单位cm
											 m_minRoadWidthCM, //道路的最小宽度
											 m_maxRoadWidthCM, //道路的最大宽度
											 m_isStructuredRoad, //是否是结构化道路,路边比路面高者(有草或者路牙)尽可以当做结构化道路
											 //正障碍
											 m_minObstacleWCM,//正障碍的最小宽度
											 m_minObstacleHCM, //正障碍的最小长度
											 //负障碍
											 m_minNegativeObstacleWCM, //负障碍的最小宽度
											 m_maxNegativeObstacleWCM  //负障碍在y方向的最小变化
									       );

		
	}
	if (!m_isForward)
	{
		RmwLidarRoadCross_Initialize( m_gridXcm, //栅格水平方向的精度,单位cm
									  m_gridYcm, //栅格垂直方向的精度,单位cm
									  m_widthCM, //栅格图像的宽度,单位cm
									  m_beginObstacleDistCM, //栅格图像的起始距离,单位cm
									  m_endObstacleDistCM, //栅格图像的终止距离,单位cm
									  m_minObstacleWCM, //障碍的最小宽度
									  m_minObstacleHCM  //障碍的最小长度
									);
	}

}

XFL_Lidar64ProcFrame::~XFL_Lidar64ProcFrame()
{

}

/////////////////////////////////////////////////////////////////////////////////////
// 
//   XFL_Lidar64ProcFrame初始化函数
//
/////////////////////////////////////////////////////////////////////////////////////
bool XFL_Lidar64ProcFrame::Initialize(unsigned int *pNavID)
{
	//step.0---------------保存惯导数据地址-----------------------//
	m_pNavID=pNavID;
	//重新初始化配置算法参数
	Initialize_ImgInput( &m_isForward,
						 //栅格化
						 &m_gridXcm , //栅格水平方向的精度,单位cm
						 &m_gridYcm , //栅格垂直方向的精度,单位cm
						 &m_widthCM , //栅格图像的宽度,单位cm
						 &m_beginGridDistCM, //栅格图像的起始距离,单位cm
						 &m_endGridDistCM, //栅格图像的终止距离,单位cm
						//道边
						 &m_beginBoundaryDistCM, //边界栅格图像的起始距离,单位cm
		                 &m_endBoundaryDistCM, //边界栅格图像的终止距离,单位cm
						 &m_minRoadWidthCM, //道路的最小宽度
						 &m_maxRoadWidthCM, //道路的最大宽度
						 //正障碍
						 &m_minObstacleWCM,//正障碍的最小宽度
						 &m_minObstacleHCM, //正障碍的最小长度
						 //负障碍
						 &m_minNegativeObstacleWCM, //负障碍的最小宽度
						 &m_maxNegativeObstacleWCM,  //负障碍的最大宽度
						 //岔路口的处理区域
						 &m_beginObstacleDistCM, //栅格图像的起始距离,单位cm
						 &m_endObstacleDistCM //栅格图像的终止距离,单位cm
					  );
	if (m_isForward)
	{
		RmwLidarObstacleBoundary_Initialize( m_gridXcm , //栅格水平方向的精度,单位cm
											 m_gridYcm , //栅格垂直方向的精度,单位cm
											 m_widthCM , //栅格图像的宽度,单位cm
											 m_beginGridDistCM, //栅格图像的起始距离,单位cm
											 m_endGridDistCM, //栅格图像的终止距离,单位cm
											 //道边
											 m_beginBoundaryDistCM, //边界栅格图像的起始距离,单位cm
											 m_endBoundaryDistCM, //边界栅格图像的终止距离,单位cm
											 m_minRoadWidthCM, //道路的最小宽度
											 m_maxRoadWidthCM, //道路的最大宽度
											 m_isStructuredRoad, //是否是结构化道路,路边比路面高者(有草或者路牙)尽可以当做结构化道路
											 //正障碍
											 m_minObstacleWCM,//正障碍的最小宽度
											 m_minObstacleHCM, //正障碍的最小长度
											 //负障碍
											 m_minNegativeObstacleWCM, //负障碍的最小宽度
											 m_maxNegativeObstacleWCM  //负障碍的最小宽度

									       );
	}
	if (!m_isForward)
	{
		RmwLidarRoadCross_Initialize( m_gridXcm, //栅格水平方向的精度,单位cm
									  m_gridYcm, //栅格垂直方向的精度,单位cm
									  m_widthCM, //栅格图像的宽度,单位cm
									  m_beginObstacleDistCM, //栅格图像的起始距离,单位cm
									  m_endObstacleDistCM, //栅格图像的终止距离,单位cm
									  m_minObstacleWCM, //障碍的最小宽度
									  m_minObstacleHCM  //障碍的最小长度
									);
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
// 
//   XFL_Lidar64ProcFrame主处理函数
//
/////////////////////////////////////////////////////////////////////////////////////
void XFL_Lidar64ProcFrame::ProcFrameLidar64Data()
{
	//step.0-----------查询当前惯导帧号----------------------------------------//
	#ifdef _NJUST_OS_USE_LINUX_
	pthread_mutex_lock(&gMutex_MC_CALLBACK);
	m_navID=*m_pNavID;
	pthread_mutex_unlock(&gMutex_MC_CALLBACK);
	#endif
	#ifdef _NJUST_OS_USE_WINDOWS_
	m_navID=*m_pNavID;
	#endif
	//step0.1----------解析指令，判断是否PL需要改变雷达的工作状态--------------//
	#ifdef _NJUST_OS_USE_LINUX_
	pthread_mutex_lock(&gMutex_PL_CALLBACK);
	#ifdef _USE_XFL_DEBUG_FILE_
	if (gPL_CMD)
	{
		char debug[100] = "gPL_CMD";
		XFL_DebugFile(gPL_CMD,debug);
	}
	#endif
	if(gPL_CMD)//收到切换状态命令
	{	
		if(gPL_CMD == NJUST_PL_3D64_COMMAND_TYPE_REPLAN_ASK) { m_isForward_StatusChange = true; m_isForward = false;}
		if(gPL_CMD == NJUST_PL_3D64_COMMAND_TYPE_CAR_REVERSE_STOP) { m_isForward_StatusChange = true;m_isForward = true;}
		if(gPL_CMD == NJUST_PL_3D64_COMMAND_TYPE_SCOUT_START) gIsObjDetect = true;
		if(gPL_CMD == NJUST_PL_3D64_COMMAND_TYPE_SCOUT_END) gIsObjDetect = false;
		gPL_CMD = 0;
	//	printf("Receive PL Command!\n");
	}
	pthread_mutex_unlock(&gMutex_PL_CALLBACK);

	pthread_mutex_lock(&gMutex_MO_CALLBACK);
	m_isObjDetect = gIsObjDetect;
	m_UseNegativeObsDetect = gUseNegativeObsDetect;
	pthread_mutex_unlock(&gMutex_MO_CALLBACK);
	#endif	
	//step0.2----------判断是否有前进状态切换，如果有则重新初始化--------------//
	if(m_isForward_StatusChange)
	{
		Initialize(m_pNavID);
		m_isForward_StatusChange = false;
	}

	//step1------------前进模式下的检测（isForward）---------------------------//
	if(m_isForward)//当往前开时取Img是车前100cm~1800cm的数据（此时没有抽样问题）
	{
		XFLLidar_Forward( );
	}

	//step2------------后退模式下的检测（isBackward）--------------------------//
	if(!m_isForward)//还需要添加是否启用岔路口信息
	{
		XFLLidar_Backward();
	}

	//step3------------编码发送------------------------------------------------//
	CodeAndSend();
	lid_pack_id++;
	
	//step4------------清空变量------------------------------------------------//
	#ifdef _NJUST_OS_USE_LINUX_ 
	memset(&m_3D64Obs,0,sizeof(NJUST_3D64_OBS_TO_FU));
	memset(&m_3D64Boundary,0,sizeof(NJUST_3D64_ROAD_BOUNDARY_TO_FU));
	memset(&m_3D64Grid,0,sizeof(NJUST_3D64_GRID_TO_FU));
	memset(&m_3D64Cross,0,sizeof(NJUST_3D64_CROSS_TO_FU));
	memset(&m_3D64TargetDetect,0,sizeof(NJUST_3D64_TARGET_TO_2D));
	#endif //用Windows模式时，需要取出这些数据，不能清空，故在Dlg中清空这些变量
	memset(gData,0,sizeof(RMW_LIDAR_3D_POINT_ORG)*XFL_3D64_MAX_POINT_NUM);
	m_DataNum_InOneFrame = 0;
	#ifdef _NJUST_OS_USE_LINUX_
	if (_gDoDebug)
	{
		printf("Already Received a frame Lidar data!\n");
	}
    #endif
	return;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//   前进(Forward)状态下的算法处理
//
/////////////////////////////////////////////////////////////////////////////////////
void XFL_Lidar64ProcFrame::XFLLidar_Forward()
{
	int width,height;
	int i,j,k;
	//step1------------执行: 道边检测------------------------------------------//
	RmwLidarObstacleBoundary_DoNext( gData,
									 m_DataNum_InOneFrame,
									 m_navID,
									 //true
									 false
									);
	double A1,B1,C1;
	double A2,B2,C2;
	bool OK1,OK2;
	//Grid结果
	pObstacleTypeImg = RmwLidarGrid_GetObstacleTypeImg(&width,&height);
	//道边结果
	RmwLidarRoadBoundary_GetBoundaryLine( &A1,&B1,&C1,&OK1,
										  &A2,&B2,&C2,&OK2
										);
#ifdef _NJUST_OS_USE_WINDOWS_
#ifdef _DEBUG_
	BYTE *pScanlineTypeImg;	
	pScanlineTypeImg=RmwLidarRoadBoundary_GetScanlineTypeImg(&width,&height);
	char fileName[256];
	if (OK1)
	{
		RmwDrawABCLine(pScanlineTypeImg,width,height,A1,B1,C1,255);
	}
	if (OK2)
	{
		RmwDrawABCLine(pScanlineTypeImg,width,height,A2,B2,C2,254);
	}
	sprintf(fileName,"D:\\tmp\\res_RmwRoadBoundary_frameID=%010d_1.bmp",i);
	//if ( (!OK1)||(!OK2) )
	Rmw_Write8BitImg2BmpFileMark254Green255Red( pScanlineTypeImg,
		width,height,
		fileName
		);
#endif
#endif
	//step2------------执行: 新正障碍------------------------------------------//
	BYTE *pObstacleBinImg;
	pObstacleBinImg=RmwLidarGrid_GetObstacleBinImg( 3, //近处阈值大
									                2, //远处阈值小
	                                                &width,
									                &height
								                   );
	//step3.1----------执行: 负障碍检测----------------------------------------//
	if((!m_isObjDetect)&&(m_UseNegativeObsDetect))
	{
		int scanlineID;
		int minDeltaHeightCM,maxDeltaHeightCM;
		for ( scanlineID = 0; scanlineID < 40; scanlineID++)
		{
			if ( scanlineID >30)
			{
				minDeltaHeightCM = 70;
				maxDeltaHeightCM = 150;
			}
			else if ( scanlineID >25)
			{
				minDeltaHeightCM = 60;
				maxDeltaHeightCM = 150;
			}
			else if ( scanlineID >20)
			{
				minDeltaHeightCM = 50;
				maxDeltaHeightCM = 150;
			}
			else if ( scanlineID >10)
			{
				minDeltaHeightCM = 40;
				maxDeltaHeightCM = 150;
			}		
			else
			{
				minDeltaHeightCM = 30;
				maxDeltaHeightCM = 120;
			}
			RmwLidarNegativeObstalce_DoNext( scanlineID, //线号
											 80,//minDeltaHeightCM, //障碍在y方向的最小变化,依线号不同而变化,10cm
											 180,//maxDeltaHeightCM, //障碍在y方向的最大变化,依线号不同而变化,120cm
											 m_navID, 
											 false
											 //true
											);
			//---------------负障碍结构体
			int N_Negative,N_pointNum,pos_begin,pos_end;
			N_Negative=N_pointNum=pos_begin=pos_end=0;
			bool IsDeleted = false;
			//当前线上的负障碍
			N_Negative = RmwLidarNegativeObstalce_GetObstacleRegions(m_NegativeRegions,MAX_NEGATIVE_OBASTACLE_NUM/20);
			if((m_3D64Obs.nObj + N_Negative + 10)>MAX_NEGATIVE_OBASTACLE_NUM)  break;//预留余量给壕沟
			if (N_Negative==0) continue;
			for(i =0; i< N_Negative;i++)
			{
				IsDeleted = false;m_HasObstacle=false;
				//从res中取出障碍物的区域
				N_pointNum = m_NegativeRegions[i].pointIDend-m_NegativeRegions[i].pointIDbegin;
				RmwLidarNegativeObstalce_Get3DPointRes( m_NegativeRegions[i].scanLineID,
														m_NegativeRegions[i].pointIDbegin,
														m_NegativeRegions[i].pointIDend,
														g3DData,
														N_pointNum+1
													   );
				
				for ( j = 1; j < N_pointNum; j++)
				{
					//判断如果跨线了就不要
					if (g3DData[j].orgData.scanLineID!=g3DData[j-1].orgData.scanLineID)
					{
						IsDeleted = true;
					}
					//正障碍前面的负障碍才算
					if (g3DData[j].obstacleType==255)
					{
						m_HasObstacle = true;
					}
					//寻找其中的结束点
					if (g3DData[j].orgData.pointID == m_NegativeRegions[i].pointIDend)
					{
						break;
					}
				}
				//使用正障碍验证负障碍
				if (!m_HasObstacle)
				{
					//IsDeleted = true;
				}
				//高度滤波
				if (m_NegativeRegions[i].deltay<30)
				{
					IsDeleted = true;
				}
				//面积滤波
				if ((m_NegativeRegions[i].S<1000)||
					(m_NegativeRegions[i].S>50000)
					)
				{
					IsDeleted = true;
				}
				//宽高比滤波
				if ((m_NegativeRegions[i].deltay/(abs(g3DData[j].orgData.x-g3DData[0].orgData.x)+1)>9)||
					(abs(g3DData[j].orgData.x-g3DData[0].orgData.x)/(m_NegativeRegions[i].deltay+1)>9)
					)
				{
					IsDeleted = true;
				}
				//车两侧的滤掉
				if (abs(g3DData[j].orgData.y)<500)
				{
					IsDeleted = true;
				}
				if(IsDeleted) continue;
				//填写负障碍外接四边形//从距离车体较近的左端点逆时针放端点
				m_3D64Obs.pObj[m_3D64Obs.nObj].nPoint = 4;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[0].x_cm = g3DData[0].orgData.x;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[0].y_cm = g3DData[0].orgData.y;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[1].x_cm = g3DData[j].orgData.x;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[1].y_cm = g3DData[j].orgData.y;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[2].x_cm = g3DData[j].orgData.x;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[2].y_cm = g3DData[j].orgData.y+m_NegativeRegions[i].deltay;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[3].x_cm = g3DData[0].orgData.x;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[3].y_cm = g3DData[0].orgData.y+m_NegativeRegions[i].deltay;
				m_3D64Obs.pObj[m_3D64Obs.nObj].z_cm = 0; //表示一般负障碍类型
				m_3D64Obs.nObj ++;

			}		
			memset(m_NegativeRegions,0,sizeof(RMW_LIDAR_3D_OBSTACLE)*MAX_NEGATIVE_OBASTACLE_NUM);
		}
	}

	//step3.2----------执行: 壕沟的检测----------------------------------------//
	if((!m_isObjDetect)&&(m_UseNegativeObsDetect))
	{
		//是否含有壕沟的标志位
		bool hasDitch,hasMend;
		//壕沟的图像坐标，单位像素
		int nearX1,nearX2,nearY1,nearY2,farX1,farX2,farY1,farY2;
		//壕沟的车体坐标，单位厘米
		int nearX1_cm,nearX2_cm,nearY1_cm,nearY2_cm,farX1_cm,farX2_cm,farY1_cm,farY2_cm;
		//壕沟补边的障碍图像坐标，单位像素
		int mendX1,mendX2,mendY1,mendY2;
		//壕沟补边的障碍车体坐标，单位cm
		int mendX1_cm,mendX2_cm,mendY1_cm,mendY2_cm;
#ifdef _TEST_MEND_
		//发送一帧假壕沟数据
		if (lid_pack_id<1)
		{
			m_3D64Obs.pObj[m_3D64Obs.nObj].nPoint = 4;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[0].x_cm = -500;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[0].y_cm = 2000;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[1].x_cm = 100;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[1].y_cm = 2000;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[2].x_cm = 100;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[2].y_cm = 2200;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[3].x_cm = -500;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[3].y_cm = 2200;
			m_3D64Obs.pObj[m_3D64Obs.nObj].z_cm = 1; //表示壕沟类型
			m_3D64Obs.nObj ++;
		}
		

#else
		//调用壕沟检测
		RmwLidarRoadDitch_DoNext( pObstacleBinImg, //壕沟检测图像
								  m_navID, //帧号 
				                  false
					       	     );
		//获取壕沟的图像坐标
		hasDitch = RmwLidarRoadDitch_GetDitchPos( &nearX1,&nearX2,&nearY1,&nearY2,
												  &farX1,&farX2,&farY1,&farY2
												 );
		//获取补边的图像坐标
		hasMend = RmwLidarRoadDitch_GetMendPos( &mendX1,&mendX2,&mendY1,&mendY2);
		if(hasDitch)
		{
			//将图像坐标转换为车体坐标
			nearX1_cm = nearX1*m_gridXcm -2000;//转到车体实际坐标，单位为cm
			nearX2_cm = nearX2*m_gridXcm -2000;//转到车体实际坐标，单位为cm
			nearY1_cm = nearY1*m_gridYcm + m_beginGridDistCM ;//转到车体实际坐标，单位为cm
			nearY2_cm = nearY2*m_gridYcm + m_beginGridDistCM ;//转到车体实际坐标，单位为cm
			farX1_cm = farX1*m_gridXcm -2000;//转到车体实际坐标，单位为cm
			farX2_cm = farX2*m_gridXcm -2000;//转到车体实际坐标，单位为cm
			farY1_cm = farY1*m_gridYcm + m_beginGridDistCM;//转到车体实际坐标，单位为cm
			farY2_cm = farY2*m_gridYcm + m_beginGridDistCM;//转到车体实际坐标，单位为cm
			//第一个壕沟
			m_3D64Obs.pObj[m_3D64Obs.nObj].nPoint = 4;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[0].x_cm = nearX1_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[0].y_cm = nearY1_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[1].x_cm = nearX2_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[1].y_cm = nearY1_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[2].x_cm = nearX2_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[2].y_cm = nearY2_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[3].x_cm = nearX1_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[3].y_cm = nearY2_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].z_cm = 1; //表示壕沟类型
			m_3D64Obs.nObj ++;
			//第二个壕沟
			m_3D64Obs.pObj[m_3D64Obs.nObj].nPoint = 4;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[0].x_cm = farX1_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[0].y_cm = farY1_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[1].x_cm = farX2_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[1].y_cm = farY1_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[2].x_cm = farX2_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[2].y_cm = farY2_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[3].x_cm = farX1_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[3].y_cm = farY2_cm;
			m_3D64Obs.pObj[m_3D64Obs.nObj].z_cm = 1; //表示壕沟类型
			m_3D64Obs.nObj ++;
			//补边
			if (hasMend)
			{
				//坐标转换
				mendX1_cm = mendX1*m_gridXcm -2000;//转到车体实际坐标，单位为cm
				mendX2_cm = mendX2*m_gridXcm -2000;//转到车体实际坐标，单位为cm
				mendY1_cm = mendY1*m_gridYcm +m_beginGridDistCM;//转到车体实际坐标，单位为cm
				mendY2_cm = mendY2*m_gridYcm +m_beginGridDistCM;//转到车体实际坐标，单位为cm
				//填入结构体
				m_3D64Obs.pObj[m_3D64Obs.nObj].nPoint = 4;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[0].x_cm = mendX1_cm;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[0].y_cm = mendY1_cm;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[1].x_cm = mendX2_cm;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[1].y_cm = mendY1_cm;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[2].x_cm = mendX2_cm;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[2].y_cm = mendY2_cm;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[3].x_cm = mendX1_cm;
				m_3D64Obs.pObj[m_3D64Obs.nObj].pPoint[3].y_cm = mendY2_cm;
				m_3D64Obs.pObj[m_3D64Obs.nObj].z_cm = 1; //表示壕沟类型
				m_3D64Obs.nObj ++;
			}
		}
#endif
	}
	//step4------------执行: 目标侦查------------------------------------------//
	if(m_isObjDetect)
	{
		RmwLidarTarget_DoNext( m_navID,
							  //true
							  false
							);
	}
	
	//step5------------填写发包结构体------------------------------------------//
	
	//step5.1--------------正障碍结构体
	//变换到输出栅格坐标
 	int shrink_x = NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM/m_gridXcm;
 	int shrink_y = NJUST_LIDAR64_VERTICAL_RESOLUTION_CM/m_gridYcm;
 	//判断障碍
	int tmp_i,tmp_j,thre,num;
	thre = 15;//取值范围0~40
	memset(m_num,0,sizeof(m_num));
	for (i=0;i<height;i++)
	{
		for ( j = 0; j < width; j++)
		{
			//从原始图中进行映射计数
			if (pObstacleBinImg[i*width+j])
			{
				//栅格图像中2000表示车后有20m
				tmp_i = (int)(i/shrink_y+(m_beginGridDistCM+2000)/NJUST_LIDAR64_VERTICAL_RESOLUTION_CM);
				tmp_j = (int)(j/shrink_x);
				m_num[tmp_i*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+tmp_j]++;
			}
		}

	}
	/*//无滤波
 	for (i = 0; i < NJUST_LIDAR64_VERTICAL_GRID_NUM; i++)
 	{
 		for (j = 0; j < NJUST_LIDAR64_HORIZONTAL_GRID_NUM; j++)
 		{
 			if (m_num[i*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j]>thre)
 			{
				//正障碍
 				m_3D64Grid.gridMsk[i*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j] = NJUST_3D64_GRID_TYPE_OBS;
 			}
 		}
 	}*/
	for (i = 1; i < NJUST_LIDAR64_VERTICAL_GRID_NUM-1; i++)
 	{
 		for (j = 1; j < NJUST_LIDAR64_HORIZONTAL_GRID_NUM-1; j++)
 		{
			/*//统计学3*3滤波，会造成膨胀
			num = m_num[i*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j]+m_num[i*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j-1]+m_num[i*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j+1]+
				m_num[(i-1)*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j]+m_num[(i-1)*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j-1]+m_num[(i-1)*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j+1]+
				m_num[(i+1)*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j]+m_num[(i+1)*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j-1]+m_num[(i+1)*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j+1];
 			*/
			//垂直方向滤波
			/*
			num = m_num[i*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j]+
				  m_num[(i-1)*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j]+
				  m_num[(i+1)*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j];
			if (num>thre*3)
			*/
			num = m_num[i*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j];
			if (num>thre)
 			{
				//正障碍
 				m_3D64Grid.gridMsk[i*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j] = NJUST_3D64_GRID_TYPE_OBS;
 			}
 		}
 	}
	
	//step5.2--------------高障碍
	//目标侦查时使用更严格的栅格
	pObstacleBinImg=RmwLidarGrid_GetObstacleBinImg( 20, //近处阈值大
													12, //远处阈值小
													&width,
													&height
												   );
	for (i=0;i<height;i++)
	{
		for ( j = 0; j < width; j++)
		{
			//从原始图中进行映射计数
			if (pObstacleBinImg[i*width+j])
			{
				//栅格图像中2000表示车后有20m
				tmp_i = (int)(i/shrink_y+(m_beginGridDistCM+2000)/NJUST_LIDAR64_VERTICAL_RESOLUTION_CM);
				tmp_j = (int)(j/shrink_x);
				m_3D64Grid.gridMsk[tmp_i*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+tmp_j] = NJUST_3D64_GRID_TYPE_HIGH_OBS;
			}
		}

	}
	
	//step5.3----------------道边结构体
	//int step_y = 100/m_gridYcm;//图像坐标系下y的步长,真实距离为1m
 	//int N = 25;//最多画出100组点
	int step_y = 25/m_gridYcm;//图像坐标系下y的步长,真实距离为1m
 	int N = 100;//最多画出100组点
 	if (OK1)
 	{
 		InsertPoint(step_y,//步长
 				    N,//最多画N个点
 				    A1,B1,C1,//直线方程参数
 				    &m_3D64Boundary,
 				    true,
 					m_gridXcm,
 					m_gridYcm,
 					m_beginGridDistCM
 				   );
 	}
 	if (OK2)
 	{
 		InsertPoint(step_y,//步长
 				    N,//最多画N个点
 				    A2,B2,C2,//直线方程参数
 				    &m_3D64Boundary,
 				    false,
 					m_gridXcm,
 					m_gridYcm,
 					m_beginGridDistCM
 				   );
 		
 	}
	
	//step5.4---------------侦查结构体
	if(m_isObjDetect)
	{
		int pLeft[10];
		int pRight[10];
		int pTop[10];
		int pBottom[10];
		int nTarget;
		int Target_x,Target_y;
		//int Offset_x = 0;
		int Offset_y = (2000+m_beginGridDistCM)/NJUST_LIDAR64_VERTICAL_RESOLUTION_CM;

		nTarget=RmwLidarTarget_GetTargetPos(pLeft,pRight,pTop,pBottom,10);
		nTarget = min(nTarget,MAX_OBJ_NUM);
		m_3D64TargetDetect.objNum = nTarget;
		for ( i = 0; i < nTarget; i++)
		{
			//填写侦查目标栅格坐标,该坐标系是图像坐标系，左下角为0点，车体中心在（100，100）处
			m_3D64TargetDetect.obj_grid_coord[i].left_j = pLeft[i]*m_gridXcm/NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM;
			m_3D64TargetDetect.obj_grid_coord[i].right_j = pRight[i]*m_gridXcm/NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM;
			m_3D64TargetDetect.obj_grid_coord[i].up_i = pBottom[i]*m_gridYcm/NJUST_LIDAR64_VERTICAL_RESOLUTION_CM+Offset_y;//把y方向0点下移20+beginDist的距离
			m_3D64TargetDetect.obj_grid_coord[i].low_i = pTop[i]*m_gridYcm/NJUST_LIDAR64_VERTICAL_RESOLUTION_CM+Offset_y;
			
			//计算侦查目标车体坐标系下实际坐标
			//step1 图像坐标系下
			Target_x = (m_3D64TargetDetect.obj_grid_coord[i].left_j+m_3D64TargetDetect.obj_grid_coord[i].right_j)*NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM/2;
			Target_y = (m_3D64TargetDetect.obj_grid_coord[i].up_i+m_3D64TargetDetect.obj_grid_coord[i].low_i)*NJUST_LIDAR64_VERTICAL_RESOLUTION_CM/2;
			//step2 车体坐标系下
			Target_x = Target_x - 2000;
			Target_y = Target_y - 2000;
			
			//计算侦查目标大地坐标系下实际坐标
			#ifdef _NJUST_OS_USE_LINUX_
			pthread_mutex_lock(&gMutex_MC_CALLBACK);
			//北向
			m_3D64TargetDetect.obj_earth_coord[i].y_cm =(int)( -Target_x * sin(gNavInfo.Yaw_rad)+ Target_y*cos(gNavInfo.Yaw_rad) + gNavInfo.EarthRefCoord[0]*100);
			//东向
			m_3D64TargetDetect.obj_earth_coord[i].x_cm = (int)(Target_x*cos(gNavInfo.Yaw_rad) + Target_y*sin(gNavInfo.Yaw_rad)+ gNavInfo.EarthRefCoord[1]*100);
			pthread_mutex_unlock(&gMutex_MC_CALLBACK);
			#endif
			//侦查目标加入障碍栅格中
			for ( k = m_3D64TargetDetect.obj_grid_coord[i].low_i; k < m_3D64TargetDetect.obj_grid_coord[i].up_i; k++)
			{
				for ( j = m_3D64TargetDetect.obj_grid_coord[i].left_j; j < m_3D64TargetDetect.obj_grid_coord[i].right_j; j++)
				{
					m_3D64Grid.gridMsk[k*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j] = NJUST_3D64_GRID_TYPE_OBJ_DETECTED;
				}
			}
		}
	}
	return;
}
/////////////////////////////////////////////////////////////////////////////////////
//
//   后退(Backward)状态下的算法处理
//
/////////////////////////////////////////////////////////////////////////////////////
void XFL_Lidar64ProcFrame::XFLLidar_Backward()
{
	int width,height;
	int i,j;
	int begin = (m_beginObstacleDistCM+2000)/NJUST_LIDAR64_VERTICAL_RESOLUTION_CM;//栅格图像中2000表示车后有20m
 	int end = (m_endObstacleDistCM+2000)/NJUST_LIDAR64_VERTICAL_RESOLUTION_CM;
 	int shrink_x = NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM/m_gridXcm;
 	int shrink_y = NJUST_LIDAR64_VERTICAL_RESOLUTION_CM/m_gridYcm;
	//step1---------------------执行: 岔路口检测--------------------------------//
	RmwLidarRoadCross_DoNext( gData,
					          m_DataNum_InOneFrame,
					          m_navID,
					          false
					         );
	//step2---------------------执行: 正障碍检测--------------------------------//
	pObstacleTypeImg = RmwLidarRoadCross_GetObstacleTypeImg(&width,&height);

	//step3---------------------填写发包结构体----------------------------------//
	//step3.1---------------------Grid结构体
	//判断障碍
	int tmp_i,tmp_j,thre;
	thre = 20;//取值范围0~40
	memset(m_num,0,sizeof(m_num));
 	for (i=0;i<height;i++)
 	{
 		for ( j = 0; j < width; j++)
 		{
 			//从原始图中进行映射计数
 			if (pObstacleTypeImg[i*width+j]==255)
 			{
				//栅格图像中2000表示车后有20m
				tmp_i = (int)(i/shrink_y+begin);
 				tmp_j = (int)(j/shrink_x);
 				m_num[tmp_i*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+tmp_j]++;
 			}
 		}
 
 	}
 	for (i = 0; i < NJUST_LIDAR64_VERTICAL_GRID_NUM; i++)
 	{
 		for (j = 0; j < NJUST_LIDAR64_HORIZONTAL_GRID_NUM; j++)
 		{
 			if (m_num[i*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j]>thre)
 			{
				//正障碍
 				m_3D64Grid.gridMsk[i*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j] = NJUST_3D64_GRID_TYPE_OBS;
 			}
 		}
 	}
	//step3.2---------------------岔路口结构体	
 	//最佳直线的方程
 	double A,B,C;
 	BYTE *pMskImg;
 	pMskImg = NULL;
 	bool hasCross=RmwLidarCross_GetBestLine(&A,&B,&C);
	#ifdef  _DEBUG_
 	char fileName[256];
 	if (hasCross)
 	{
 		pMskImg=RmwLidarCrossLines_GetCrossMskImg();
 		RmwDrawABCLineByColor( pMskImg,width,height,
 			A,B,C,
 			255
 			);
 		sprintf( fileName,"D:\\tmp\\RmwLidarCrossLines_frameID=%04d_res.bmp",1);
 		Rmw_Write8BitImg2BmpFileMark254Green255Red( pMskImg,width,height,fileName);
 	}
	#else
 	//岔路的掩模	
 	if (hasCross)
 	{
 		pMskImg=RmwLidarCross_GetCrossMskImg(&width,&height);
 	}
	#endif
 
 	//shrink: Ax/shrink_x+B(y-begin)/shrink_y+C = 0
 	A = A*shrink_x;
 	B = B*shrink_y;
 	C = C-B*begin;
 
 	if(pMskImg == NULL)
 	{
 		return ;//表示没有检测到岔路口
 	}
 	for( i = begin;i< end; i++)
 	{
 		for( j =0; j<NJUST_LIDAR64_HORIZONTAL_GRID_NUM;j++)//水平方向没有截断抽样，所以为原大小
 		{
 			if(pMskImg[(i-begin)*width*shrink_y+j*shrink_x]) 
 			{
 				m_3D64Cross.crossMsk[i*NJUST_LIDAR64_HORIZONTAL_GRID_NUM+j] = 254;
 			}
 		}
 	}
 
	#ifdef  _DEBUG_
 	if (hasCross)
 	{
 		RmwDrawABCLineByColor( p3D64Cross->crossMsk,NJUST_LIDAR64_HORIZONTAL_GRID_NUM,NJUST_LIDAR64_VERTICAL_GRID_NUM,
 			A,B,C,
 			255
 			);
 		sprintf( fileName,"D:\\tmp\\RmwLidarCrossLines_frameID=%04d_res.bmp",2);
 		Rmw_Write8BitImg2BmpFileMark254Green255Red( p3D64Cross->crossMsk,NJUST_LIDAR64_HORIZONTAL_GRID_NUM,NJUST_LIDAR64_VERTICAL_GRID_NUM,fileName);
 	}
	#endif
	return;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//   发包
//
/////////////////////////////////////////////////////////////////////////////////////
void XFL_Lidar64ProcFrame::CodeAndSend()
{
	//step1.1-----------填写obs结构体-----------------------------------------//
	//DataType  0代表OBS，1代表BOUNDARY，2代表GRID，3代表CROSS
	m_3D64Obs.frameID = lid_pack_id;
	m_3D64Obs.navID = m_navID;
	#ifdef _NJUST_OS_USE_LINUX_
	NJUST_IP_TIME time = NJUST_IP_get_time();
	m_3D64Obs.synTime = time;
	#endif
	//step1.2-----------填写boundary结构体-------------------------------------//
	//DataType  0代表OBS，1代表BOUNDARY，2代表GRID，3代表CROSS
	m_3D64Boundary.frameID = lid_pack_id;
	m_3D64Boundary.navID = m_navID;
	#ifdef _NJUST_OS_USE_LINUX_
	time = NJUST_IP_get_time();
	m_3D64Boundary.synTime = time;
	#endif
	//step1.3-----------填写grid结构体------------------------------------------//
	//DataType  0代表OBS，1代表BOUNDARY，2代表GRID，3代表CROSS
	m_3D64Grid.frameID = lid_pack_id;
	m_3D64Grid.navID = m_navID;
	#ifdef _NJUST_OS_USE_LINUX_
	time = NJUST_IP_get_time();
	m_3D64Grid.synTime = time;
	#endif
	//step1.4-----------填写cross结构体-----------------------------------------//
	//DataType  0代表OBS，1代表BOUNDARY，2代表GRID，3代表CROSS
	m_3D64Cross.frameID = lid_pack_id;
	m_3D64Cross.navID =  m_navID;
	#ifdef _NJUST_OS_USE_LINUX_
	time = NJUST_IP_get_time();
	m_3D64Cross.synTime = time;
	#endif
	//step1.5-----------填写Detect结构体-----------------------------------------//
	m_3D64TargetDetect.frameID = lid_pack_id;
	m_3D64TargetDetect.navID = m_navID;
	#ifdef _NJUST_OS_USE_LINUX_
	time = NJUST_IP_get_time();
	m_3D64TargetDetect.synTime = time;
	#endif

	//step2.1----------------对要发送的结构体进行编码并发送-----------------------//
	int nlength;
	unsigned char pResBuf[sizeof(NJUST_3D64_OBS_TO_FU)
						+sizeof(NJUST_3D64_ROAD_BOUNDARY_TO_FU)
						+sizeof(NJUST_3D64_GRID_TO_FU)
						+sizeof(NJUST_3D64_CROSS_TO_FU)
						+1000];//一个足够大的buffer
	memset(pResBuf,0,sizeof(pResBuf));
	if(m_isForward)
	{
		//负障碍
		nlength = NJUST_3D64_Encode_Compress(
													0,//对Obs进行编码
													&m_3D64Obs,
													&m_3D64Boundary,
													&m_3D64Grid,
													&m_3D64Cross,
													pResBuf,
													sizeof(pResBuf)
												);
		#ifdef _NJUST_OS_USE_LINUX_
		#ifdef NJUST_IP_COMM_USE_OLD_BEFORE_20160826
		if (_gDoDebug)
		{
			printf("nlength = %d\n",nlength);
		}
		//向Fu发送数据，以tcp发送
		int Obs_to_FUsize = NJUST_IP_udp_send_to(FU_MODEL,pResBuf,nlength);		
		if (_gDoDebug)
		{
			printf("Obs_to_FUsize is %d\n",Obs_to_FUsize);		
		}	
		//向Mo发送数据，以udp发送
		int Obs_to_MOsize = NJUST_IP_udp_send_to(MO_MODEL,pResBuf,nlength);			
		if (_gDoDebug)
		{
			printf("Obs_to_MOsize is %d\n",Obs_to_MOsize);
		}
		#else
		if (_gDoDebug)
		{
			printf("nlength = %d\n",nlength);
		}
		//向Fu发送数据，以tcp发送
		int Obs_to_FUsize = NJUST_IP_send_to(FU_MODEL,pResBuf,nlength);		
		if (_gDoDebug)
		{
			printf("Obs_to_FUsize is %d\n",Obs_to_FUsize);		
		}	
		//向Mo发送数据，以udp发送
		int Obs_to_MOsize = NJUST_IP_send_to(MO_MODEL,pResBuf,nlength);			
		if (_gDoDebug)
		{
			printf("Obs_to_MOsize is %d\n",Obs_to_MOsize);
		}
		#endif
		#endif
	}

	//step2.2----------------对要发送的结构体进行编码并发送------------------------//
	if(m_isForward)
	{
		memset(pResBuf,0,sizeof(pResBuf));
		nlength = NJUST_3D64_Encode_Compress(
												1,//对Boundary进行编码
												&m_3D64Obs,
												&m_3D64Boundary,
												&m_3D64Grid,
												&m_3D64Cross,
												pResBuf,
												sizeof(pResBuf)
											);
		#ifdef _NJUST_OS_USE_LINUX_
		#ifdef NJUST_IP_COMM_USE_OLD_BEFORE_20160826
		if (_gDoDebug)
		{
			printf("nlength = %d\n",nlength);
		}
		//向Fu发送数据，以udp发送
		int Boundary_to_FUsize = NJUST_IP_udp_send_to(FU_MODEL,pResBuf,nlength);		
		if (_gDoDebug)
		{
			printf("Boundary_to_FUsize is %d\n",Boundary_to_FUsize);		
		}	
		//向Mo发送数据，以udp发送
		int Boundary_to_MOsize = NJUST_IP_udp_send_to(MO_MODEL,pResBuf,nlength);			
		if (_gDoDebug)
		{
			printf("Boundary_to_MOsize is %d\n",Boundary_to_MOsize);
		}

		#else
		if (_gDoDebug)
		{
			printf("nlength = %d\n",nlength);
		}
		//向Fu发送数据，以udp发送
		int Boundary_to_FUsize = NJUST_IP_send_to(FU_MODEL,pResBuf,nlength);		
		if (_gDoDebug)
		{
			printf("Boundary_to_FUsize is %d\n",Boundary_to_FUsize);		
		}	
		//向Mo发送数据，以udp发送
		int Boundary_to_MOsize = NJUST_IP_send_to(MO_MODEL,pResBuf,nlength);			
		if (_gDoDebug)
		{
			printf("Boundary_to_MOsize is %d\n",Boundary_to_MOsize);
		}
		#endif
		#endif
	}

	//step2.3----------------对要发送的结构体进行编码并发送------------------------//
	memset(pResBuf,0,sizeof(pResBuf));
	nlength = NJUST_3D64_Encode_Compress(
												2,//对grid进行压缩//一直发送
												&m_3D64Obs,
												&m_3D64Boundary,
												&m_3D64Grid,
												&m_3D64Cross,
												pResBuf,
												sizeof(pResBuf)
											);
	#ifdef _NJUST_OS_USE_LINUX_
	#ifdef NJUST_IP_COMM_USE_OLD_BEFORE_20160826
	if (_gDoDebug)
	{
		printf("nlength = %d\n",nlength);
	}
	//向Fu发送数据
	int grid_to_FUsize = NJUST_IP_udp_send_to(FU_MODEL,pResBuf,nlength);		
	if (_gDoDebug)
	{
		printf("grid_to_FUsize is %d\n",grid_to_FUsize);		
	}	
	//向Mo发送数据
	int grid_to_MOsize = NJUST_IP_udp_send_to(MO_MODEL,pResBuf,nlength);			
	if (_gDoDebug)
	{
		printf("grid_to_MOsize is %d\n",grid_to_MOsize);
	}
	#else 
	if (_gDoDebug)
	{
		printf("nlength = %d\n",nlength);
	}
	//向Fu发送数据
	int grid_to_FUsize = NJUST_IP_send_to(FU_MODEL,pResBuf,nlength);		
	if (_gDoDebug)
	{
		printf("grid_to_FUsize is %d\n",grid_to_FUsize);		
	}	
	//向Mo发送数据
	int grid_to_MOsize = NJUST_IP_send_to(MO_MODEL,pResBuf,nlength);			
	if (_gDoDebug)
	{
		printf("grid_to_MOsize is %d\n",grid_to_MOsize);
	}
	#endif
	#endif
	//step2.4----------------对要发送的结构体进行编码并发送------------------------//
	if(!m_isForward)
	{
		memset(pResBuf,0,sizeof(pResBuf));
		nlength = NJUST_3D64_Encode_Compress(
												3,//对cross进行压缩
												&m_3D64Obs,
												&m_3D64Boundary,
												&m_3D64Grid,
												&m_3D64Cross,
												pResBuf,
												sizeof(pResBuf)
											);
		#ifdef _NJUST_OS_USE_LINUX_
		#ifdef NJUST_IP_COMM_USE_OLD_BEFORE_20160826
		if (_gDoDebug)
		{
			printf("nlength = %d\n",nlength);
		}
		//向Fu发送数据
		int cross_to_FUsize = NJUST_IP_udp_send_to(FU_MODEL,pResBuf,nlength);		
		if (_gDoDebug)
		{
			printf("cross_to_FUsize is %d\n",cross_to_FUsize);		
		}	
		//向Mo发送数据
		int cross_to_MOsize = NJUST_IP_udp_send_to(MO_MODEL,pResBuf,nlength);			
		if (_gDoDebug)
		{
			printf("cross_to_MOsize is %d\n",cross_to_MOsize);
		}

		#else 
		if (_gDoDebug)
		{
			printf("nlength = %d\n",nlength);
		}
		//向Fu发送数据
		int cross_to_FUsize = NJUST_IP_send_to(FU_MODEL,pResBuf,nlength);		
		if (_gDoDebug)
		{
			printf("cross_to_FUsize is %d\n",cross_to_FUsize);		
		}	
		//向Mo发送数据
		int cross_to_MOsize = NJUST_IP_send_to(MO_MODEL,pResBuf,nlength);			
		if (_gDoDebug)
		{
			printf("cross_to_MOsize is %d\n",cross_to_MOsize);
		}
		#endif
		#endif
	}

	//step2.5----------------对要发送的侦查结构体进行编码并发送---------------------//
	if(m_isObjDetect)
	{
		#ifdef _NJUST_OS_USE_LINUX_
		#ifdef NJUST_IP_COMM_USE_OLD_BEFORE_20160826
		//向2D发送数据，以tcp发送
		int ObsDetect_to_IR_size = NJUST_IP_tcp_send_to(IR_MODEL,&m_3D64TargetDetect,sizeof(NJUST_3D64_TARGET_TO_2D));		
		if (_gDoDebug)
		{
			printf("Obs_Detect_to_IR is %d\n",ObsDetect_to_IR_size);		
		}	
		#else
		//向2D发送数据
		int ObsDetect_to_IR_size = NJUST_IP_send_to(IR_MODEL,&m_3D64TargetDetect,sizeof(NJUST_3D64_TARGET_TO_2D));		
		if (_gDoDebug)
		{
			printf("Obs_Detect_to_IR is %d\n",ObsDetect_to_IR_size);		
		}	
		#endif
		#endif
	}

	//step3----------------对工作状态进行编码并发送---------------------------------//
	#ifdef _NJUST_OS_USE_LINUX_
	t1 = NJUST_IP_get_time();
	workstat.synTime = NJUST_IP_get_time();
	workstat.moduleID = 2;
	workstat.myselfTimeOutMS = 2000;
	workstat.stat = NJUST_MO_WORK_STAT_VAILD;
	workstat.PELR = (gNavIDEnd - gNavIDStart + 1 - gMcID) * 1000 / (gNavIDEnd - gNavIDStart + 1);
	workstat.timeConsumingMS = (int)NJUST_IP_get_time_GAP_ms(t1,t2);
	workstat.errCode = NJUST_MO_ERRCODE_NOERR;
	sprintf(workstat.pErrMsg,"%s","\0");
	int nByte = 0;
	void *pStat = NULL;
	pStat = NJUST_MO_Encode_STA_IP_Data(&workstat,&nByte);
	#ifdef NJUST_IP_COMM_USE_OLD_BEFORE_20160826
	//向Mo发送数据，以udp发送
	int workstat_to_MOsize = NJUST_IP_udp_send_to(MO_MODEL,pStat,nByte);
	#else
	//向Mo发送数据
	int workstat_to_MOsize = NJUST_IP_send_to(MO_MODEL,pStat,nByte);
	#endif
	if (_gDoDebug)
	{
		printf("workstat_to_MOsize is %d\n",workstat_to_MOsize);
	}
	t2 = NJUST_IP_get_time();
	#endif
}

/////////////////////////////////////////////////////////////////////////////////////
//
//   处理死循环
//
/////////////////////////////////////////////////////////////////////////////////////
#ifdef _NJUST_OS_USE_LINUX_ 
void XFL_Lidar64ProcFrame::ProcFrameLoop()
{
	//已处理的帧数
	int nProcessedFrameNum,nReceivedFrameNum;
	nProcessedFrameNum = 0;
	//死循环
	while(true)
	{
		pthread_mutex_lock(&gMutex_NewFrame);
		nReceivedFrameNum = _gTotal3DDataFrame;
		pthread_mutex_unlock(&gMutex_NewFrame);
		//如果没有一帧需要处理的数据过来，那就等待
		if (nProcessedFrameNum == nReceivedFrameNum)//已经是最新了
		{//按雷达10HZ,100ms/348包=0.3ms,300us/包,所以设计延迟100us.
			usleep(100); 
			continue;
		}
		else
		{
			pthread_mutex_lock(&gMutex_NewFrame);			
			memcpy(gData,_g3DDataNewFrame,sizeof(RMW_LIDAR_3D_POINT_ORG)*XFL_3D64_MAX_POINT_NUM);
			m_DataNum_InOneFrame = _g3DDataNumInNewFrame;
			pthread_mutex_unlock(&gMutex_NewFrame);
			if (_gDoDebug)
			{
				printf("----------------ProcFrameLidar64Data----------------\n");
			}
			ProcFrameLidar64Data();
			nProcessedFrameNum = nReceivedFrameNum;
		}
	}
	return;
}
#endif 

#ifdef _NJUST_OS_USE_WINDOWS_ 
void XFL_Lidar64ProcFrame::ProcFrameLoop()
{	
	m_DataNum_InOneFrame = _g3DDataNumInNewFrame;
	memcpy(gData,_g3DDataNewFrame,sizeof(RMW_LIDAR_3D_POINT_ORG)*m_DataNum_InOneFrame);
	ProcFrameLidar64Data();
	return;
}
#endif 





