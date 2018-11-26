///////////////////////////////////////////////////////////////////////////////////
// �������� �� 
//  1. ������������������˿�����������Ȩ������������ҵĿ�ļ�Ӧ�������     
//  2. ���ڱ������Ǹ��˿����ģ����ɱ���ش��ڸ��ָ�����ȱ�ݻ�������Ա��������
//     ����Ӧ���ڿ����о��ͽ�ѧ�С�
//  3. ����������Ӧ����ʵ������ʱ�����䵼�µĸ��ֲ�����������˸Ų�����
//     ��������ִ��󣬿�����ϵrenmingwu@mail.njust.edu.cn, renmingwu@sina.com
//  4. �����ַ����ļ����뱣����Щ����
//
///////////////////////////////////////////////////////////////////////////////////
// Caution:
// 1) If you do not get authorization from Mingwu Ren, 
//    this lib CAN NOT be used for commerce.
// 2) It is only developed by individual, so the errors can not be avioded. 
//    I strongly suggest that it should only be used for research and education.
// 3) If you find errors, you can tell me. E-Mail: renmingwu@mail.njust.edu.cn
//    renmingwu@sina.com,
//    you can call me: 13952033136 
// 4) If you redistribute this lib, please keep these words.
///////////////////////////////////////////////////////////////////////////////////
#ifndef RMW_LIDAR_LINE_OBSTACLE_LIB_H
#define RMW_LIDAR_LINE_OBSTACLE_LIB_H

#include "RmwLidarStruct.h"

typedef unsigned char BYTE;

///////////////////////////////////////////////////////////////////////////////////
//
// ���ұ߽���
//
///////////////////////////////////////////////////////////////////////////////////
//��ʼ��
bool RmwLidarObstacleBoundary_Initialize( 
	                                  //դ��
	                                  int gridXcm, //դ��ˮƽ����ľ���,��λcm
                                      int gridYcm, //դ��ֱ����ľ���,��λcm
		                              int widthCM, //դ��ͼ��Ŀ��,��λcm
		                              int beginGridDistCM, //դ��ͼ�����ʼ����,��λcm
		                              int endGridDistCM, //դ��ͼ�����ֹ����,��λcm
									  //��·�߽�
									  int beginBoundaryDistCM, //�߽�դ��ͼ�����ʼ����,��λcm
		                              int endBoundaryDistCM, //�߽�դ��ͼ�����ֹ����,��λcm
								      int minRoadWidthCM, //��·����С���
					                  int maxRoadWidthCM, //��·�������
									  bool isStructuredRoad, //�Ƿ��ǽṹ����·,·�߱�·�����(�вݻ���·��)�����Ե����ṹ����·
									   //���ϰ�
                                      int minObstacleWCM, //���ϰ�����С���
		                              int minObstacleHCM,  //���ϰ�����С����
									  //���ϰ�
									  int minNegativeObstacleWCM, //���ϰ�����С���
								      int maxNegativeObstacleWCM  //���ϰ��������
				                   );
char *RmwLidarObstacleBoundary_GetErrInfo();
//ִ��
bool RmwLidarObstacleBoundary_DoNext( RMW_LIDAR_3D_POINT_ORG *p3DData,  //3D����
				                      int n3DData, //3D���ݵĸ���
				                      int frameID, //֡��
						              bool doDebug
			                        );
//դ����
BYTE *RmwLidarGrid_GetScanlineTypeImg(int *width,int *height);  //�ߺ�ͼ��
BYTE *RmwLidarGrid_GetObstacleScoreImg(int *width,int *height); //�ϵ÷�ͼ��
BYTE *RmwLidarGrid_GetObstacleTypeImg(int *width,int *height);  //�ϰ�����ͼ��
BYTE *RmwLidarGrid_GetZImg( int *width,int *height,
						    int *resolutionZCM,int *offset
						   ); //�߶�ͼ
//�õ��ϰ��Ķ�ֵͼ��,�û��Լ�������ֵ
BYTE *RmwLidarGrid_GetObstacleBinImg( int nearThre, //������ֵ��
									  int farThre, //Զ����ֵС
	                                  int *width,
									  int *height
								    );

//�߽���
void RmwLidarRoadBoundary_GetBoundaryLine( double *A1,double *B1,double *C1,bool *OK1, //���ֱ�ߵķ���AX+BY+C=0
					                       double *A2,double *B2,double *C2,bool *OK2  //�Ҳ�ֱ�ߵķ���AX+BY+C=0
					                     );
///////////////////////////////////////////////////////////////////////////////////
//
// ���ϰ�
//
///////////////////////////////////////////////////////////////////////////////////
//ִ��
int RmwLidarNegativeObstalce_DoNext( int scanlineID, //�ߺ�
	                                 int minDeltaHeightCM, //�ϰ���y�������С�仯,���ߺŲ�ͬ���仯
		                             int maxDeltaHeightCM, //�ϰ���y��������仯,���ߺŲ�ͬ���仯
	                                 int frameID, //֡��
						             bool doDebug
			                       );
//���
int RmwLidarNegativeObstalce_GetObstacleRegions(RMW_LIDAR_3D_OBSTACLE *pRegions,int N);
//ȡ����
int RmwLidarNegativeObstalce_Get3DPointRes( int scanlineID,
										    int beginPointID,int endPointID,
											RMW_LIDAR_3D_POINT_RES *p3DData,
											int N
									      );
///////////////////////////////////////////////////////////////////////////////////
//
// Ŀ�����
//
///////////////////////////////////////////////////////////////////////////////////
//ִ��
bool RmwLidarTarget_DoNext( int frameID, //֡�� 
				            bool doDebug
			              );
//���
int RmwLidarTarget_GetTargetPos( int *pLeft,
								 int *pRight,
								 int *pTop,
								 int *pBottom,
								 int N
							   );
///////////////////////////////////////////////////////////////////////////////////
//
// ��·���
//
///////////////////////////////////////////////////////////////////////////////////
//��ʼ��
bool RmwLidarRoadCross_Initialize( int gridXcm, //դ��ˮƽ����ľ���,��λcm
                                   int gridYcm, //դ��ֱ����ľ���,��λcm
		                           int widthCM, //դ��ͼ��Ŀ��,��λcm
		                           int beginObstacleDistCM, //դ��ͼ�����ʼ����,��λcm
		                           int endObstacleDistCM, //դ��ͼ�����ֹ����,��λcm
								   int minObstacleWCM, //�ϰ�����С���
		                           int minObstacleHCM  //�ϰ�����С����
				                 );
//ִ��
bool RmwLidarRoadCross_DoNext( RMW_LIDAR_3D_POINT_ORG *p3DData,  //3D����
				               int n3DData, //3D���ݵĸ���
				               int frameID, //֡��
						       bool doDebug
			                 );
//���-դ��ͼ��
BYTE *RmwLidarRoadCross_GetScanlineTypeImg(int *width,int *height);
BYTE *RmwLidarRoadCross_GetObstacleTypeImg(int *width,int *height);
//���:��ѵ�����
bool RmwLidarCross_GetBestPoint(int *x,int *y);
//���:���ֱ�ߵķ���,������ѵ�����ʱ����false
bool RmwLidarCross_GetBestLine( double *A,double *B,double *C);
//���:��·����ģ,������ѵ�����ʱ����NULL
BYTE *RmwLidarCross_GetCrossMskImg(int *width,int *height);
////////////////////////////////////////////////////////////////////////////////
//
// �������
//
////////////////////////////////////////////////////////////////////////////////
//ִ��
bool RmwLidarRoadDitch_DoNext( BYTE *pObstalceBinImg, //�ϰ�ͼ��
				               int frameID, //֡�� 
				               bool doDebug
			                );
//���
bool RmwLidarRoadDitch_GetDitchPos( int *nearX1,int *nearX2,int *nearY1,int *nearY2,
		                            int *farX1,int *farX2,int *farY1,int *farY2
		                          );
bool RmwLidarRoadDitch_GetMendPos( int *mendX1,int *mendX2,int *mendY1,int *mendY2);
////////////////////////////////////////////////////////////////////////////////
//
// ˮ����
//
///////////////////////////////////////////////////////////////////////////////////
//��ʼ��
bool RmwLidarWater_Initialize( int width, //դ��ͼ��Ŀ��
		                       int height, //դ��ͼ��ĸ߶�
		                       int minObstacleW, //�ϰ�����С���
		                       int minObstacleH, //�ϰ�����С�߶�
					           int nShrinkY //y������С�ı���,����ٶ�
				             );
//ִ��
int RmwLidarWater_DoNext( BYTE *pGryImg,
					      int frameID,
						  bool doDebug
						);
//���
int RmwLidarWater_GetWaterRegions(RMW_LIDAR_2D_OBSTACLE *pRegions,int N);
///////////////////////////////////////////////////////////////////////////////////
//
//  ����
//
///////////////////////////////////////////////////////////////////////////////////
void RmwDrawABCLine( BYTE *pGryImg,int width,int height,
					 double A,double B,double C,
					 BYTE Color
				   );
void Rmw2PLine(BYTE *pGryImg,int width,int x0,int y0,int x1,int y1,int color);
void RmwRectDraw(BYTE *pGryImg,int width,int height,int x1,int x2,int y1,int y2,int Color);
void RmwRectFill(BYTE *pGryImg,int width,int height,int x1,int x2,int y1,int y2,int Color);

#endif
 