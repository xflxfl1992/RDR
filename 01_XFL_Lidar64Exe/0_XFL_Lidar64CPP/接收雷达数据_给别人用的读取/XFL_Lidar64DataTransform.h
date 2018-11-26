#ifndef XFL_LIDAR64_DATA_TRANSFORM_H
#define XFL_LIDAR64_DATA_TRANSFORM_H

#include "NJUST_ALV_BYD_H/ALV_BYD.h"
#include "XFL_Lidar64Def.h"
#include "./RmwLidarProcLib/RmwLidarStruct.h"



//起始角度
#define XFL_TRANSFORM_START_ANGLE                  27000   //从270度（雷达左侧）作为起始,1/100度
#define XFL_ANGLE_NUM_ONE_ROUND                    (360*100) //一圈的角度个度

//内部结构体
typedef struct{
	int scannerType;
	int angle;
	int distance[32];
	int intense[32];
}UNIT;

typedef struct{
	double rotC;
	double vertC;
	double disC;
	double distCX;
	double distCY;
	double vertoffC;
	double horioffC;
}HDL_CALIB;

//类
class XFL_Lidar64DataTransform
{
public:
	//构造/析构
	XFL_Lidar64DataTransform();
	~XFL_Lidar64DataTransform();
    //初始化
    bool Initialize(char *pCalibrateFileName,//内参文件
					int *ExternalParaMatrix, //外参矩阵
					bool *isDeletedScanlineLut //抽线表
					);
	//执行,返回一帧是否准备好
	bool DoNext( void *pIPdata,//从雷达接收到的原始数据包
		         int nBytes //这包数据的长度（正常情况下为1206个字节）
			  );
	//结果-当前帧及其数据个数
	RMW_LIDAR_3D_POINT_ORG *GetFrame(int *nFrameData);
    //复位,在Windows下读文件时,每读完一个文件,需要Reset()一下
	void Reset();

private:
	// 内存释放
	void Dump();
    //读雷达内参数
	bool ReadCalibratePara(char *pCalibrateFileName);
    // step.2------------解析数据包,返回是否回绕------------------------------//
	bool DecodeIPData();
	inline void InnerParaCorrect(int scanlineID, UNIT unit);
	// step.3------------找到angle最小的点，并以此找到存储起始点--------------//
	int FindStartPointPos();
	// step.4------------从设置的开始位置,重新组织数据,并Pass无效点------------//
	void setPoint(int startPos);
    // step.5------------调试-------------------------------------------------//
	void Debug();

private:
	//初始化成功
	bool m_isInitOK;
  	int m_frameID;
  	int m_packageID;
	//内参
	HDL_CALIB m_calib[64];
	float m_cosRotCorrection[XFL_3D64_HEAD_NUM];
	float m_sinRotCorrection[XFL_3D64_HEAD_NUM];
	float m_cosVetCorrection[XFL_3D64_HEAD_NUM];
	float m_sinVetCorrection[XFL_3D64_HEAD_NUM];
	//外参矩阵
	int m_ExternalParaMatrix[3][4];
	//IP数据
	char m_IPdata[LIDAR64_IP_DATA_SIZE];
	int m_IPdataInt[LIDAR64_IP_DATA_SIZE];
	RMW_LIDAR_3D_POINT_ORG m_point3D; 
 	//解包
	RMW_LIDAR_3D_POINT_ORG m_pDecodeData[XFL_3D64_MAX_POINT_NUM];//解码后的数据
	int m_nDecodeData;
	int m_pAngle[XFL_SINGLE_LINE_MAX_POINT_NUM]; //数据的角度顺序表
	int m_nAngle;
	int m_nPackageAfterPreFrame; //自上一帧以来的解包个数
	//回绕
	int m_pUsedAngle[XFL_ANGLE_NUM_ONE_ROUND]; //本帧内用过的角度 
	int m_preAngle;
	bool m_isFrameFirst;
	bool m_isFrameReady; //是否一帧准备好
	//抽线表
	bool m_isDeletedScanlineLut[XFL_3D64_HEAD_NUM];
	//结果
	RMW_LIDAR_3D_POINT_ORG m_pFrameData[XFL_3D64_MAX_POINT_NUM]; //一帧满了时,才有数据
	int m_nFrameData; //当前帧内的数据个数
};

#endif
