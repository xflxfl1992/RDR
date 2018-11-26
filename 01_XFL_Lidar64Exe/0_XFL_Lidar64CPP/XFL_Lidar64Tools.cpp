#include "XFL_Lidar64Tools.h"

/////////////////////////////////////////////////////////////////////////////////////
// 
//   ��ʼ��pPosImg�Ĳ���(XFL_3D64_DataHold)
//
/////////////////////////////////////////////////////////////////////////////////////
void Initialize_ImgInput(bool* isForward,
						 //դ��
	                     int* gridXcm, //դ��ˮƽ����ľ���,��λcm
                         int* gridYcm, //դ��ֱ����ľ���,��λcm
		                 int* widthCM, //դ��ͼ��Ŀ��,��λcm
		                 int* beginGridDistCM, //դ��ͼ�����ʼ����,��λcm
		                 int* endGridDistCM, //դ��ͼ�����ֹ����,��λcm
						 //��·�߽�
						 int* beginBoundaryDistCM, //�߽�դ��ͼ�����ʼ����,��λcm
		                 int* endBoundaryDistCM, //�߽�դ��ͼ�����ֹ����,��λcm
						 int* minRoadWidthCM, //��·����С���
					     int* maxRoadWidthCM, //��·�������
						 //���ϰ�
                         int* minObstacleWCM, //���ϰ�����С���
		                 int* minObstacleHCM,  //���ϰ�����С����
						 //���ϰ�
						 int* minNegativeObstacleWCM, //���ϰ�����С���
		                 int* maxNegativeObstacleWCM,  //���ϰ��������
						 //��·�ڵĴ�������
						 int* beginObstacleDistCM, //դ��ͼ�����ʼ����,��λcm
						 int* endObstacleDistCM //դ��ͼ�����ֹ����,��λcm
						 )
{
	if (* isForward)
	{
		* gridXcm = 5; //դ��ˮƽ����ľ���,��λcm,5cm�Ա����������Ӻͺ��ʵĴ����ٶ�
		* gridYcm = 5; //դ��ֱ����ľ���,��λcm,2cm�Ա������ֿܷ�
		* widthCM = 40*100; //դ��ͼ��Ŀ��,��λcm,���Ҹ�20m
		* beginGridDistCM= 0*100; //դ��ͼ�����ʼ����,��λcm,��(�״�)ǰ1m,ä��
		* endGridDistCM = 60*100; //դ��ͼ�����ֹ����,��λcm,��λcm,��(�״�)ǰ18m
		
		* beginBoundaryDistCM = 3*100 ; //�߽�դ��ͼ�����ʼ����,��λcm
		* endBoundaryDistCM = 60*100 ; //�߽�դ��ͼ�����ֹ����,��λcm
		* minRoadWidthCM = 3*100; //��·����С���
		* maxRoadWidthCM = 15*100; //��·�������

		* minObstacleWCM = 30; //�ϰ�����С���,��λcm
		* minObstacleHCM = 12; //�ϰ�����С�߶�,��λcm
		
		* minNegativeObstacleWCM = 80 ; //���ϰ�����С���
		* maxNegativeObstacleWCM = 180; //���ϰ��������
	}
	if (!(* isForward))
	{
		* gridXcm = 5; //դ��ˮƽ����ľ���,��λcm
		* gridYcm = 2; //դ��ֱ����ľ���,��λcm
		* widthCM = 40*100; //դ��ͼ��Ŀ��,��λcm
		* beginObstacleDistCM= -6*100; //դ��ͼ�����ʼ����,��λcm
		* endObstacleDistCM = 10*100; //դ��ͼ�����ֹ����,��λcm
		* minObstacleWCM = 30; //�ϰ�����С���,��λcm
		* minObstacleHCM = 12; //�ϰ�����С����,��λcm
	}
	
	
	return ;
}


/////////////////////////////////////////////////////////////////////////////////////
// 
//   ��ֱ�߲����
//
/////////////////////////////////////////////////////////////////////////////////////
void InsertPoint(int step_y,//����
 				 int N,//��໭N����
 				 double A,double B,double C,//ֱ�߷��̲���
 				 NJUST_3D64_ROAD_BOUNDARY_TO_FU * p3D64Boundary,
 				 bool isLeft,
 				 int gridXcm,
 				 int gridYcm,
 				 int beginDistCM
 				 )
 {
 	//step.1----------------���ԭͼ�ϵĶ�Ӧ�����---------------------//
 	int x = 0;
 	int y = step_y*5-beginDistCM/gridYcm;//��ǰͼ������ϵ�µ�x,y
 
 	for(int i = 0; i< N ; i++)
 	{
 		y += step_y;
 		if ((A<0.001)&&(A>-0.001)) continue;
 		x = int((-C-B*y)/A);//����ͼ���е�x����
 		x = x*gridXcm -2000;//ת������ʵ�����꣬��λΪcm
 		if(x<2000&&x>-2000)
 		{
 			p3D64Boundary->Point[p3D64Boundary->nPoint].x_cm = x;
 			p3D64Boundary->Point[p3D64Boundary->nPoint].y_cm = y*gridYcm + beginDistCM;//ת�ɳ���ʵ������
 			if(isLeft) p3D64Boundary->Type[p3D64Boundary->nPoint] = NJUST_3D64_ROAD_POINT_TYPE_LEFT;
 			else p3D64Boundary->Type[p3D64Boundary->nPoint] = NJUST_3D64_ROAD_POINT_TYPE_RIGHT;
 			(p3D64Boundary->nPoint) ++;
 		}			
 	}
 	return;
 }