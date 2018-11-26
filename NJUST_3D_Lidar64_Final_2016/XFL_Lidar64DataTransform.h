#ifndef XFL_LIDAR64_DATA_TRANSFORM_H
#define XFL_LIDAR64_DATA_TRANSFORM_H

#include "NJUST_ALV_BYD_H/ALV_BYD.h"
#include "XFL_Lidar64Def.h"
#include "./RmwLidarProcLib/RmwLidarStruct.h"



//��ʼ�Ƕ�
#define XFL_TRANSFORM_START_ANGLE                  27000   //��270�ȣ��״���ࣩ��Ϊ��ʼ,1/100��
#define XFL_ANGLE_NUM_ONE_ROUND                    (360*100) //һȦ�ĽǶȸ���

//�ڲ��ṹ��
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

//��
class XFL_Lidar64DataTransform
{
public:
	//����/����
	XFL_Lidar64DataTransform();
	~XFL_Lidar64DataTransform();
    //��ʼ��
    bool Initialize(char *pCalibrateFileName,//�ڲ��ļ�
					int *ExternalParaMatrix, //��ξ���
					bool *isDeletedScanlineLut //���߱�
					);
	//ִ��,����һ֡�Ƿ�׼����
	bool DoNext( void *pIPdata,//���״���յ���ԭʼ���ݰ�
		         int nBytes //������ݵĳ��ȣ����������Ϊ1206���ֽڣ�
			  );
	//���-��ǰ֡�������ݸ���
	RMW_LIDAR_3D_POINT_ORG *GetFrame(int *nFrameData);
    //��λ,��Windows�¶��ļ�ʱ,ÿ����һ���ļ�,��ҪReset()һ��
	void Reset();

private:
	// �ڴ��ͷ�
	void Dump();
    //���״��ڲ���
	bool ReadCalibratePara(char *pCalibrateFileName);
    // step.2------------�������ݰ�,�����Ƿ����------------------------------//
	bool DecodeIPData();
	inline void InnerParaCorrect(int scanlineID, UNIT unit);
	// step.3------------�ҵ�angle��С�ĵ㣬���Դ��ҵ��洢��ʼ��--------------//
	int FindStartPointPos();
	// step.4------------�����õĿ�ʼλ��,������֯����,��Pass��Ч��------------//
	void setPoint(int startPos);
    // step.5------------����-------------------------------------------------//
	void Debug();

private:
	//��ʼ���ɹ�
	bool m_isInitOK;
  	int m_frameID;
  	int m_packageID;
	//�ڲ�
	HDL_CALIB m_calib[64];
	float m_cosRotCorrection[XFL_3D64_HEAD_NUM];
	float m_sinRotCorrection[XFL_3D64_HEAD_NUM];
	float m_cosVetCorrection[XFL_3D64_HEAD_NUM];
	float m_sinVetCorrection[XFL_3D64_HEAD_NUM];
	//��ξ���
	int m_ExternalParaMatrix[3][4];
	//IP����
	char m_IPdata[LIDAR64_IP_DATA_SIZE];
	int m_IPdataInt[LIDAR64_IP_DATA_SIZE];
	RMW_LIDAR_3D_POINT_ORG m_point3D; 
 	//���
	RMW_LIDAR_3D_POINT_ORG m_pDecodeData[XFL_3D64_MAX_POINT_NUM];//����������
	int m_nDecodeData;
	int m_pAngle[XFL_SINGLE_LINE_MAX_POINT_NUM]; //���ݵĽǶ�˳���
	int m_nAngle;
	int m_nPackageAfterPreFrame; //����һ֡�����Ľ������
	//����
	int m_pUsedAngle[XFL_ANGLE_NUM_ONE_ROUND]; //��֡���ù��ĽǶ� 
	int m_preAngle;
	bool m_isFrameFirst;
	bool m_isFrameReady; //�Ƿ�һ֡׼����
	//���߱�
	bool m_isDeletedScanlineLut[XFL_3D64_HEAD_NUM];
	//���
	RMW_LIDAR_3D_POINT_ORG m_pFrameData[XFL_3D64_MAX_POINT_NUM]; //һ֡����ʱ,��������
	int m_nFrameData; //��ǰ֡�ڵ����ݸ���
};

#endif
