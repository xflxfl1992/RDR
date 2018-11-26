////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//将雷达取得的原始数据，转化为可用的结构体，并存储到结构体数组中
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "NJUST_ALV_BYD_H/ALV_BYD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "XFL_Lidar64DataTransform.h"
#include "XFL_Lidar64Extern.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 调试
//
////////////////////////////////////////////////////////////////////////////////////////////////////
extern void XFL_DebugFile(int i,char *pStr);
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 构造/析构函数
//
////////////////////////////////////////////////////////////////////////////////////////////////////
XFL_Lidar64DataTransform::XFL_Lidar64DataTransform()
{  	

}
	
XFL_Lidar64DataTransform::~XFL_Lidar64DataTransform()
{	
	Dump();
}

void XFL_Lidar64DataTransform::Dump()
{   
	
}

/////////////////////////////////////////////////////////////////////////////////////
// 
//   线号转换表（从近到远）
//
/////////////////////////////////////////////////////////////////////////////////////
static int _gLidarScanLineIDLUT[64]={ 36,37,58,59,38,39,32,33,40,41,34,35,48,49,42,43,
                                      50,51,44,45,52,53,46,47,60,61,54,55,62,63,56,57,
                                      4,5,26,27,6,7,0,1,8,9,2,3,16,17,10,11,
                                      18,19,12,13,20,21,14,15,28,29,22,23,30,31,24,25
                                    };
/////////////////////////////////////////////////////////////////////////////////////
//
// 初始化
//
/////////////////////////////////////////////////////////////////////////////////////
bool XFL_Lidar64DataTransform::Initialize(char *pCalibrateFileName,int *ExternalParaMatrix,bool *isDeletedScanlineLut)
{    
	bool ret;

	// step.1------初始化结构体数组------------------------------------------------//
 	memset(m_pDecodeData,0,sizeof(RMW_LIDAR_3D_POINT_ORG)*XFL_3D64_MAX_POINT_NUM);
	memset(m_pUsedAngle,0,sizeof(int)*XFL_ANGLE_NUM_ONE_ROUND); //本帧内用过的角度
	m_isFrameFirst=true;
	// step.2------初始化变量--------------------------------------------------//
	m_packageID = 0; //包ID
	m_frameID = 0; //帧ID
	//解包
	m_nAngle=0;
	m_nDecodeData=0;
	m_nPackageAfterPreFrame=0; //自上一帧以来的解包个数
	m_isFrameReady=false; //是否得到一帧
	//结果
	m_nFrameData = 0; //当前帧的数据个数
	// step.3------初始化外参标定矩阵----------------------------------------------//
	memcpy(&m_ExternalParaMatrix[0][0],ExternalParaMatrix,sizeof(int)*3*4);
	// step.4------初始化雷达内参值------------------------------------------------//
	ret=ReadCalibratePara(pCalibrateFileName);
	// step.5------初始化抽线表----------------------------------------------------//
	memcpy(m_isDeletedScanlineLut,isDeletedScanlineLut,sizeof(bool)*XFL_3D64_HEAD_NUM);
	// step.6------返回------------------------------------------------------------//
	m_isInitOK = ret;
	return m_isInitOK;
}

bool XFL_Lidar64DataTransform::ReadCalibratePara(char *pCalibrateFileName)
{   //读雷达内参数
	FILE *fp;
	int i;

	// step.0------初始化---------------------------------------------------------//
	memset(m_calib,0,sizeof(HDL_CALIB)*64);
	// step.1------打开文件-------------------------------------------------------//
	fp = fopen(pCalibrateFileName,"rt");
	if (fp == NULL)
	{
		return false;
	}
	// step.2------读文件-------------------------------------------------------//
	i=0;
	while (fscanf(fp,"%lf %lf %lf %lf %lf %lf %lf",
					 &m_calib[i].rotC,
					 &m_calib[i].vertC,
					 &m_calib[i].disC,
					 &m_calib[i].distCX,
					 &m_calib[i].distCY,
					 &m_calib[i].vertoffC,
					 &m_calib[i].horioffC) != EOF)
	{
		i++;
	}
	fclose(fp);
	// step.3------预先求值---------------------------------------------------//
	for(int i=0;i<64;i++)
	{
		m_cosVetCorrection[i]= (float) (cos(m_calib[i].vertC*PIDIVIDED));
		m_sinVetCorrection[i]= (float) (sin(m_calib[i].vertC*PIDIVIDED));	
		m_cosRotCorrection[i]= (float) (cos(m_calib[i].rotC*PIDIVIDED));
		m_sinRotCorrection[i]= (float) (sin(m_calib[i].rotC*PIDIVIDED));
	}
	// step.4------返回-------------------------------------------------------//
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 执行
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool XFL_Lidar64DataTransform::DoNext(  void *pIPdata, //从雷达接收到的原始数据包
							            int nBytes//这包数据的长度,应是1206
						             )
{ 	
	int startPos;

	// step.0------------安全性检测-------------------------------------------//
 	if (nBytes != LIDAR64_IP_DATA_SIZE)
	{
		#ifdef _NJUST_OS_USE_LINUX_
		if (_gDoDebug)
		{
			printf("XFL_Lidar64DataTransform() nBytes!=%d!\n",LIDAR64_IP_DATA_SIZE);
		}
        #endif
		return false;
	}
	// step.1------------保存数据----------------------------------------------//
	memcpy(m_IPdata,pIPdata,sizeof(char)*LIDAR64_IP_DATA_SIZE);
    // step.2------------解析数据包,返回是否回绕-------------------------------//
	m_isFrameReady=DecodeIPData();
	//自上一帧以来的解包个数
	m_nPackageAfterPreFrame++; 
	// step.3------------判断一帧是否结束--------------------------------------//
	if (m_isFrameReady)
	{
		// step.3.1------------找到距离2700角度最小的位置--------------------------//
		startPos=FindStartPointPos();
		// step.3.2------------以此点作为存储起始点--------------------------------//
		setPoint(startPos);
		// step.3.3------------准备下一帧-----------------------------------------//
		m_nAngle=0;
		m_nDecodeData=0;
		m_nPackageAfterPreFrame=0;
		//赋为无效点
		memset(m_pDecodeData,0,sizeof(RMW_LIDAR_3D_POINT_ORG)*XFL_3D64_MAX_POINT_NUM);
		//本帧内用过的角度
	    memset(m_pUsedAngle,0,sizeof(int)*XFL_ANGLE_NUM_ONE_ROUND);
		//是第一次
		m_isFrameFirst=true;
 		// step.3.4------------帧ID----------------------------------------------//
		m_frameID++;
	}
	// step.4.-----------返回一帧是否准备好------------------------------------//
	//总的解包个数
	m_packageID++;
	//返回
	return m_isFrameReady;
}

void XFL_Lidar64DataTransform::Reset()
{   //复位,在Windows下读文件时,每读完一个文件,需要Reset()一下
	m_nAngle=0;
	m_nDecodeData=0;
	m_nPackageAfterPreFrame=0;
	//赋为无效点
	memset(m_pDecodeData,0,sizeof(RMW_LIDAR_3D_POINT_ORG)*XFL_3D64_MAX_POINT_NUM);
	//本帧内用过的角度
	memset(m_pUsedAngle,0,sizeof(int)*XFL_ANGLE_NUM_ONE_ROUND);
	//是第一次
	m_isFrameFirst=true;
	//返回
	return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  结果-当前帧及其数据个数
//
////////////////////////////////////////////////////////////////////////////////////////////////////
RMW_LIDAR_3D_POINT_ORG *XFL_Lidar64DataTransform::GetFrame(int *nFrameData)
{   //当前帧及其数据个数
	*nFrameData = m_nFrameData;
	return m_pFrameData;
}
/////////////////////////////////////////////////////////////////////////////////
//
// step.2------------解析数据包,返回是否回绕
//
////////////////////////////////////////////////////////////////////////////////
bool XFL_Lidar64DataTransform::DecodeIPData()
{
	int radar_Matix[4][1];
	int ver_Matrix[3][1];
	UNIT cur;
	int i,j;
	int k,m,pos;
	int tmp;

	// step.1------------原始数据包格式强转------------------------------------//
	for (i = 0; i < LIDAR64_IP_DATA_SIZE; i++)
	{
		tmp = (int)m_IPdata[i];          
		tmp = (tmp >= 0) ? tmp : (256 + tmp);
		m_IPdataInt[i] = tmp;
	}
	// step.2------------将数据进行内参转化，变换成需要的结构体----------------//
	for( i=0; 
		 i<LIDAR64_IP_DATA_SIZE-6; //去掉尾部6位雷达固件信息
		 i+=LIDAR64_DATA_SECTOR_SIZE //每次处理一个扇区(100个字节)
	   )
	{
		//step.2.1-------[0,1]字节为扫描头信息-------------------------------//
		cur.scannerType = m_IPdataInt[i+1] * 256 + m_IPdataInt[i];
		//step.2.2-------[2,3]字节为扫描角度---------------------------------//
		cur.angle = m_IPdataInt[i+3] * 256 + m_IPdataInt[i+2];
		//step.2.3-------判断是否回绕(使用上扫描头)---------------------------//
		if (m_isFrameFirst)
		{
			//赋值
			m_pUsedAngle[cur.angle]=1;
			//上一次
			m_preAngle=cur.angle;
			//置位
			m_isFrameFirst=false;
		}
		else //1,4==> m=4-1, 3, ==> 1+1+0,1+1+1,1+1+2
		{
			m=(cur.angle+XFL_ANGLE_NUM_ONE_ROUND-m_preAngle)%XFL_ANGLE_NUM_ONE_ROUND;
			for(k=0;k<m;k++)
			{
				pos=(m_preAngle+1+k)%XFL_ANGLE_NUM_ONE_ROUND;
				if (m_pUsedAngle[pos])
				{
					return true;
				}
				m_pUsedAngle[pos]=1;
			}
			//上一次
			m_preAngle=cur.angle;
		}
		//step.2.4-------后跟32*3个字节的距离信息和辐射强度-------------------//
		for (j=0;j<LIDAR64_SCANLINE_NUM_IN_SECTOR;j++)
		{
			//1.距离
			cur.distance[j]=m_IPdataInt[j*3+i+5]* 256+m_IPdataInt[j*3+i+4];
			//2.强度
			cur.intense[j]=m_IPdataInt[j*3+i+6];
			//3.过滤:45cm近和300m远去掉
			if ( (cur.distance[j] < 450) || 
				 (cur.distance[j] > 30000)
			   )
			{
				continue;
			}			
			//4.内参数校正
		    InnerParaCorrect(j, cur);
	        //5.外参校正
			if (!(m_point3D.x == 0 && m_point3D.y == 0 && m_point3D.z == 0))
			{	
				radar_Matix[0][0] = m_point3D.x;
				radar_Matix[1][0] = m_point3D.y;
				radar_Matix[2][0] = m_point3D.z;
				radar_Matix[3][0] = 1;

				ver_Matrix[0][0] = m_ExternalParaMatrix[0][0] * radar_Matix[0][0] +
					m_ExternalParaMatrix[0][1] * radar_Matix[1][0] +
					m_ExternalParaMatrix[0][2] * radar_Matix[2][0] +
					m_ExternalParaMatrix[0][3] * radar_Matix[3][0];

				ver_Matrix[1][0] = m_ExternalParaMatrix[1][0] * radar_Matix[0][0] +
					m_ExternalParaMatrix[1][1] * radar_Matix[1][0] +
					m_ExternalParaMatrix[1][2] * radar_Matix[2][0] +
					m_ExternalParaMatrix[1][3] * radar_Matix[3][0];

				ver_Matrix[2][0] = m_ExternalParaMatrix[2][0] * radar_Matix[0][0] +
					m_ExternalParaMatrix[2][1] * radar_Matix[1][0] +
					m_ExternalParaMatrix[2][2] * radar_Matix[2][0] +
					m_ExternalParaMatrix[2][3] * radar_Matix[3][0];

				m_point3D.x = ver_Matrix[0][0];
				m_point3D.y = ver_Matrix[1][0];
				m_point3D.z = ver_Matrix[2][0];
				m_point3D.d = int(sqrt(1.0*m_point3D.x*m_point3D.x+m_point3D.y*m_point3D.y));
			}
			else continue;
			//6.赋值-强度
			m_point3D.intense = cur.intense[j];
			//7.赋值-线号
			m_point3D.scanLineID = _gLidarScanLineIDLUT[m_point3D.scanLineID]; 
			//8.保存
			m_pDecodeData[m_point3D.scanLineID*XFL_SINGLE_LINE_MAX_POINT_NUM+m_nAngle]=m_point3D;
			m_nDecodeData++;
		}//end of j(100)
		//step.2.5-------记录角度---------------------------------------------//
		if (cur.scannerType==LIDAR64_UPPER_HEADER_TYPE) //由于upper与lower包相间发送过来,所以仅使用其中一个即可
		{   
			//先赋
			m_pAngle[m_nAngle]=cur.angle;
			m_nAngle++;
		}
	}//end of i(1206),12个小包,每条线12个数据.
	// step.3------------返回--------------------------------------------------//
	return false;
}

inline void XFL_Lidar64DataTransform::InnerParaCorrect(int scanlineID, UNIT unit)
{
	int calibPos = 0;
	// step.1------------初始化------------------------------------------------//
	memset(&m_point3D,0,sizeof(RMW_LIDAR_3D_POINT_ORG));
	// step.2------------扫描线ID----------------------------------------------//
	if(unit.scannerType == LIDAR64_UPPER_HEADER_TYPE)
	{
		calibPos = scanlineID;
	}
	else if(unit.scannerType == LIDAR64_LOWER_HEADER_TYPE)
	{
		calibPos = scanlineID + 32;
	}
	// step.3------------坐标线转换--------------------------------------------//
	float rotAngle = (float) (unit.angle * 0.01);
	float distance_1 = (float) ((unit.distance[scanlineID]) * 0.2);	
	float distance = (float) (((distance_1 + m_calib[calibPos].disC)));
	float cosVertAngle = m_cosVetCorrection[calibPos];  //0.997
	float sinVertAngle = m_sinVetCorrection[calibPos];  //-0.0733
	float cosRotCorr = m_cosRotCorrection[calibPos]; //0.99997
	float sinRotCorr = m_sinRotCorrection[calibPos]; //0.0076
	//cos(a-b)= cos(a)cos(b)+sin(a)sin(b)
	//sin(a-b)= sin(a)cos(b)-cos(a)sin(b)
	float cosRotA = (float) cos(rotAngle*PIDIVIDED);
	float sinRotA = (float) sin(rotAngle*PIDIVIDED);
	float cosRotAngle = (cosRotA * cosRotCorr + sinRotA * sinRotCorr);  //0.81724*0.99997-0.5763*0.0076= 0.8138
	float sinRotAngle = (sinRotA * cosRotCorr - cosRotA * sinRotCorr);  //-0.5763*0.99997-0.81724*0.0076= -0.5823		
	float hOffsetCorr = (float) m_calib[calibPos].horioffC; //cm   -2.6
	float vOffsetCorr = (float) m_calib[calibPos].vertoffC; //cm    20.1288	
	//	float xyDistance = distance * cosVertAngle;
	//	float x = xyDistance * sinRotAngle;
	//	float y = xyDistance * cosRotAngle;
	//	float z = distance * sinVertAngle;
	float xyDistance = (distance * cosVertAngle - vOffsetCorr * sinVertAngle);  //273.8*0.997+20.1288*0.0733=274.45
	int x = (int)(xyDistance * sinRotAngle - hOffsetCorr * cosRotAngle); // here exists a +pos.getX() here.   274.45*(-0.5823)-(-2.6)*0.8138=-157.696
	int y = (int)(xyDistance * cosRotAngle + hOffsetCorr * sinRotAngle); //....   274.45*0.8138 +(-2.6)*(-0.5823)=224.8613
	int z = (int)(distance * sinVertAngle + vOffsetCorr * cosVertAngle);    // 273.8*(-0.0733)+20.1288*0.997=-0.0011
	m_point3D.x = -x;
	m_point3D.y = y;
	m_point3D.z = z;
	m_point3D.scanLineID = calibPos;
	// step.4------------返回-------------------------------------------------//
	return;
}
/////////////////////////////////////////////////////////////////////////////////
//
// step.3 找到angle最小的点，并以此找到存储起始点
//
/////////////////////////////////////////////////////////////////////////////////
int XFL_Lidar64DataTransform::FindStartPointPos()
{	
	int i;
	int dif,pos;
	int mindif;

	// step.0------------初始化------------------------------------------------//
	pos = 0;
	dif = mindif = abs(m_pAngle[0] - XFL_TRANSFORM_START_ANGLE);
	// step.1------------搜索angle数组中的XFL_TRANSFORM_START_ANGLE点---------//
	for (i = 1; i < m_nAngle; i++)
	{
		dif = abs(m_pAngle[i]-XFL_TRANSFORM_START_ANGLE);
		if (dif<mindif)
		{
			pos = i;
			mindif = dif;
		}
	}
	// step.2------------返回-------------------------------------------------//
 	return pos;
}
/////////////////////////////////////////////////////////////////////////////////
//
// step.4 从设置的开始位置,重新组织数据结构,并Pass无效点
//
/////////////////////////////////////////////////////////////////////////////////
void XFL_Lidar64DataTransform::setPoint(int startPos)
{
	int scanLineID,DeletedLineNum,j;
	int offset,pos;

    // step.0------------初始化------------------------------------------------//
	m_nFrameData=0;
	DeletedLineNum=0;
	// step.1------------重新组织数据------------------------------------------//
	for (scanLineID=0;scanLineID<XFL_3D64_HEAD_NUM;scanLineID++)
	{
		//抽线
		if (m_isDeletedScanlineLut[scanLineID])
		{
			DeletedLineNum++;
			continue;
		}

		offset=scanLineID*XFL_SINGLE_LINE_MAX_POINT_NUM;
		for (j=0;j<m_nAngle;j++)
		{
			//位置
			pos = (startPos+j) % m_nAngle;
			pos += offset;
			//去掉无效数据,因为在一帧准备好时,将m_pDecodeData清零的.
			if  (  (m_pDecodeData[pos].x==0)&&
				   (m_pDecodeData[pos].y==0)&&
				   (m_pDecodeData[pos].z==0)
				)
			{
				continue;
			}
			//复制
			else
			{
				memcpy(&m_pFrameData[m_nFrameData],&m_pDecodeData[pos],sizeof(RMW_LIDAR_3D_POINT_ORG));
				m_pFrameData[m_nFrameData].scanLineID = scanLineID-DeletedLineNum;
				m_pFrameData[m_nFrameData].pointID = j;
				m_nFrameData++;
			}
		}
	}
 	// step.2------------返回--------------------------------------------------//
 	return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 调试
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void XFL_Lidar64DataTransform::Debug()
{   	
	return;
}

