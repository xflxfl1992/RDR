////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//���״�ȡ�õ�ԭʼ���ݣ�ת��Ϊ���õĽṹ�壬���洢���ṹ��������
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
// ����
//
////////////////////////////////////////////////////////////////////////////////////////////////////
extern void XFL_DebugFile(int i,char *pStr);
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����/��������
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
//   �ߺ�ת�����ӽ���Զ��
//
/////////////////////////////////////////////////////////////////////////////////////
static int _gLidarScanLineIDLUT[64]={ 36,37,58,59,38,39,32,33,40,41,34,35,48,49,42,43,
                                      50,51,44,45,52,53,46,47,60,61,54,55,62,63,56,57,
                                      4,5,26,27,6,7,0,1,8,9,2,3,16,17,10,11,
                                      18,19,12,13,20,21,14,15,28,29,22,23,30,31,24,25
                                    };
/////////////////////////////////////////////////////////////////////////////////////
//
// ��ʼ��
//
/////////////////////////////////////////////////////////////////////////////////////
bool XFL_Lidar64DataTransform::Initialize(char *pCalibrateFileName,int *ExternalParaMatrix,bool *isDeletedScanlineLut)
{    
	bool ret;

	// step.1------��ʼ���ṹ������------------------------------------------------//
 	memset(m_pDecodeData,0,sizeof(RMW_LIDAR_3D_POINT_ORG)*XFL_3D64_MAX_POINT_NUM);
	memset(m_pUsedAngle,0,sizeof(int)*XFL_ANGLE_NUM_ONE_ROUND); //��֡���ù��ĽǶ�
	m_isFrameFirst=true;
	// step.2------��ʼ������--------------------------------------------------//
	m_packageID = 0; //��ID
	m_frameID = 0; //֡ID
	//���
	m_nAngle=0;
	m_nDecodeData=0;
	m_nPackageAfterPreFrame=0; //����һ֡�����Ľ������
	m_isFrameReady=false; //�Ƿ�õ�һ֡
	//���
	m_nFrameData = 0; //��ǰ֡�����ݸ���
	// step.3------��ʼ����α궨����----------------------------------------------//
	memcpy(&m_ExternalParaMatrix[0][0],ExternalParaMatrix,sizeof(int)*3*4);
	// step.4------��ʼ���״��ڲ�ֵ------------------------------------------------//
	ret=ReadCalibratePara(pCalibrateFileName);
	// step.5------��ʼ�����߱�----------------------------------------------------//
	memcpy(m_isDeletedScanlineLut,isDeletedScanlineLut,sizeof(bool)*XFL_3D64_HEAD_NUM);
	// step.6------����------------------------------------------------------------//
	m_isInitOK = ret;
	return m_isInitOK;
}

bool XFL_Lidar64DataTransform::ReadCalibratePara(char *pCalibrateFileName)
{   //���״��ڲ���
	FILE *fp;
	int i;

	// step.0------��ʼ��---------------------------------------------------------//
	memset(m_calib,0,sizeof(HDL_CALIB)*64);
	// step.1------���ļ�-------------------------------------------------------//
	fp = fopen(pCalibrateFileName,"rt");
	if (fp == NULL)
	{
		return false;
	}
	// step.2------���ļ�-------------------------------------------------------//
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
	// step.3------Ԥ����ֵ---------------------------------------------------//
	for(int i=0;i<64;i++)
	{
		m_cosVetCorrection[i]= (float) (cos(m_calib[i].vertC*PIDIVIDED));
		m_sinVetCorrection[i]= (float) (sin(m_calib[i].vertC*PIDIVIDED));	
		m_cosRotCorrection[i]= (float) (cos(m_calib[i].rotC*PIDIVIDED));
		m_sinRotCorrection[i]= (float) (sin(m_calib[i].rotC*PIDIVIDED));
	}
	// step.4------����-------------------------------------------------------//
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ִ��
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool XFL_Lidar64DataTransform::DoNext(  void *pIPdata, //���״���յ���ԭʼ���ݰ�
							            int nBytes//������ݵĳ���,Ӧ��1206
						             )
{ 	
	int startPos;

	// step.0------------��ȫ�Լ��-------------------------------------------//
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
	// step.1------------��������----------------------------------------------//
	memcpy(m_IPdata,pIPdata,sizeof(char)*LIDAR64_IP_DATA_SIZE);
    // step.2------------�������ݰ�,�����Ƿ����-------------------------------//
	m_isFrameReady=DecodeIPData();
	//����һ֡�����Ľ������
	m_nPackageAfterPreFrame++; 
	// step.3------------�ж�һ֡�Ƿ����--------------------------------------//
	if (m_isFrameReady)
	{
		// step.3.1------------�ҵ�����2700�Ƕ���С��λ��--------------------------//
		startPos=FindStartPointPos();
		// step.3.2------------�Դ˵���Ϊ�洢��ʼ��--------------------------------//
		setPoint(startPos);
		// step.3.3------------׼����һ֡-----------------------------------------//
		m_nAngle=0;
		m_nDecodeData=0;
		m_nPackageAfterPreFrame=0;
		//��Ϊ��Ч��
		memset(m_pDecodeData,0,sizeof(RMW_LIDAR_3D_POINT_ORG)*XFL_3D64_MAX_POINT_NUM);
		//��֡���ù��ĽǶ�
	    memset(m_pUsedAngle,0,sizeof(int)*XFL_ANGLE_NUM_ONE_ROUND);
		//�ǵ�һ��
		m_isFrameFirst=true;
 		// step.3.4------------֡ID----------------------------------------------//
		m_frameID++;
	}
	// step.4.-----------����һ֡�Ƿ�׼����------------------------------------//
	//�ܵĽ������
	m_packageID++;
	//����
	return m_isFrameReady;
}

void XFL_Lidar64DataTransform::Reset()
{   //��λ,��Windows�¶��ļ�ʱ,ÿ����һ���ļ�,��ҪReset()һ��
	m_nAngle=0;
	m_nDecodeData=0;
	m_nPackageAfterPreFrame=0;
	//��Ϊ��Ч��
	memset(m_pDecodeData,0,sizeof(RMW_LIDAR_3D_POINT_ORG)*XFL_3D64_MAX_POINT_NUM);
	//��֡���ù��ĽǶ�
	memset(m_pUsedAngle,0,sizeof(int)*XFL_ANGLE_NUM_ONE_ROUND);
	//�ǵ�һ��
	m_isFrameFirst=true;
	//����
	return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ���-��ǰ֡�������ݸ���
//
////////////////////////////////////////////////////////////////////////////////////////////////////
RMW_LIDAR_3D_POINT_ORG *XFL_Lidar64DataTransform::GetFrame(int *nFrameData)
{   //��ǰ֡�������ݸ���
	*nFrameData = m_nFrameData;
	return m_pFrameData;
}
/////////////////////////////////////////////////////////////////////////////////
//
// step.2------------�������ݰ�,�����Ƿ����
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

	// step.1------------ԭʼ���ݰ���ʽǿת------------------------------------//
	for (i = 0; i < LIDAR64_IP_DATA_SIZE; i++)
	{
		tmp = (int)m_IPdata[i];          
		tmp = (tmp >= 0) ? tmp : (256 + tmp);
		m_IPdataInt[i] = tmp;
	}
	// step.2------------�����ݽ����ڲ�ת�����任����Ҫ�Ľṹ��----------------//
	for( i=0; 
		 i<LIDAR64_IP_DATA_SIZE-6; //ȥ��β��6λ�״�̼���Ϣ
		 i+=LIDAR64_DATA_SECTOR_SIZE //ÿ�δ���һ������(100���ֽ�)
	   )
	{
		//step.2.1-------[0,1]�ֽ�Ϊɨ��ͷ��Ϣ-------------------------------//
		cur.scannerType = m_IPdataInt[i+1] * 256 + m_IPdataInt[i];
		//step.2.2-------[2,3]�ֽ�Ϊɨ��Ƕ�---------------------------------//
		cur.angle = m_IPdataInt[i+3] * 256 + m_IPdataInt[i+2];
		//step.2.3-------�ж��Ƿ����(ʹ����ɨ��ͷ)---------------------------//
		if (m_isFrameFirst)
		{
			//��ֵ
			m_pUsedAngle[cur.angle]=1;
			//��һ��
			m_preAngle=cur.angle;
			//��λ
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
			//��һ��
			m_preAngle=cur.angle;
		}
		//step.2.4-------���32*3���ֽڵľ�����Ϣ�ͷ���ǿ��-------------------//
		for (j=0;j<LIDAR64_SCANLINE_NUM_IN_SECTOR;j++)
		{
			//1.����
			cur.distance[j]=m_IPdataInt[j*3+i+5]* 256+m_IPdataInt[j*3+i+4];
			//2.ǿ��
			cur.intense[j]=m_IPdataInt[j*3+i+6];
			//3.����:45cm����300mԶȥ��
			if ( (cur.distance[j] < 450) || 
				 (cur.distance[j] > 30000)
			   )
			{
				continue;
			}			
			//4.�ڲ���У��
		    InnerParaCorrect(j, cur);
	        //5.���У��
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
			//6.��ֵ-ǿ��
			m_point3D.intense = cur.intense[j];
			//7.��ֵ-�ߺ�
			m_point3D.scanLineID = _gLidarScanLineIDLUT[m_point3D.scanLineID]; 
			//8.����
			m_pDecodeData[m_point3D.scanLineID*XFL_SINGLE_LINE_MAX_POINT_NUM+m_nAngle]=m_point3D;
			m_nDecodeData++;
		}//end of j(100)
		//step.2.5-------��¼�Ƕ�---------------------------------------------//
		if (cur.scannerType==LIDAR64_UPPER_HEADER_TYPE) //����upper��lower����䷢�͹���,���Խ�ʹ������һ������
		{   
			//�ȸ�
			m_pAngle[m_nAngle]=cur.angle;
			m_nAngle++;
		}
	}//end of i(1206),12��С��,ÿ����12������.
	// step.3------------����--------------------------------------------------//
	return false;
}

inline void XFL_Lidar64DataTransform::InnerParaCorrect(int scanlineID, UNIT unit)
{
	int calibPos = 0;
	// step.1------------��ʼ��------------------------------------------------//
	memset(&m_point3D,0,sizeof(RMW_LIDAR_3D_POINT_ORG));
	// step.2------------ɨ����ID----------------------------------------------//
	if(unit.scannerType == LIDAR64_UPPER_HEADER_TYPE)
	{
		calibPos = scanlineID;
	}
	else if(unit.scannerType == LIDAR64_LOWER_HEADER_TYPE)
	{
		calibPos = scanlineID + 32;
	}
	// step.3------------������ת��--------------------------------------------//
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
	// step.4------------����-------------------------------------------------//
	return;
}
/////////////////////////////////////////////////////////////////////////////////
//
// step.3 �ҵ�angle��С�ĵ㣬���Դ��ҵ��洢��ʼ��
//
/////////////////////////////////////////////////////////////////////////////////
int XFL_Lidar64DataTransform::FindStartPointPos()
{	
	int i;
	int dif,pos;
	int mindif;

	// step.0------------��ʼ��------------------------------------------------//
	pos = 0;
	dif = mindif = abs(m_pAngle[0] - XFL_TRANSFORM_START_ANGLE);
	// step.1------------����angle�����е�XFL_TRANSFORM_START_ANGLE��---------//
	for (i = 1; i < m_nAngle; i++)
	{
		dif = abs(m_pAngle[i]-XFL_TRANSFORM_START_ANGLE);
		if (dif<mindif)
		{
			pos = i;
			mindif = dif;
		}
	}
	// step.2------------����-------------------------------------------------//
 	return pos;
}
/////////////////////////////////////////////////////////////////////////////////
//
// step.4 �����õĿ�ʼλ��,������֯���ݽṹ,��Pass��Ч��
//
/////////////////////////////////////////////////////////////////////////////////
void XFL_Lidar64DataTransform::setPoint(int startPos)
{
	int scanLineID,DeletedLineNum,j;
	int offset,pos;

    // step.0------------��ʼ��------------------------------------------------//
	m_nFrameData=0;
	DeletedLineNum=0;
	// step.1------------������֯����------------------------------------------//
	for (scanLineID=0;scanLineID<XFL_3D64_HEAD_NUM;scanLineID++)
	{
		//����
		if (m_isDeletedScanlineLut[scanLineID])
		{
			DeletedLineNum++;
			continue;
		}

		offset=scanLineID*XFL_SINGLE_LINE_MAX_POINT_NUM;
		for (j=0;j<m_nAngle;j++)
		{
			//λ��
			pos = (startPos+j) % m_nAngle;
			pos += offset;
			//ȥ����Ч����,��Ϊ��һ֡׼����ʱ,��m_pDecodeData�����.
			if  (  (m_pDecodeData[pos].x==0)&&
				   (m_pDecodeData[pos].y==0)&&
				   (m_pDecodeData[pos].z==0)
				)
			{
				continue;
			}
			//����
			else
			{
				memcpy(&m_pFrameData[m_nFrameData],&m_pDecodeData[pos],sizeof(RMW_LIDAR_3D_POINT_ORG));
				m_pFrameData[m_nFrameData].scanLineID = scanLineID-DeletedLineNum;
				m_pFrameData[m_nFrameData].pointID = j;
				m_nFrameData++;
			}
		}
	}
 	// step.2------------����--------------------------------------------------//
 	return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void XFL_Lidar64DataTransform::Debug()
{   	
	return;
}

