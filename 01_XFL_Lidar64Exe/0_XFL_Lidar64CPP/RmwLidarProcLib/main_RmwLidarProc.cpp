#define  _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <memory.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "bmpFile.h"

#include "../../3_XFL_Lidar64CPP/RmwRead64LidarFile.h"

#include "../../5_RmwLidarProcCPP/Rmw3DdataGrid.h"
#include "../../5_RmwLidarProcCPP/RmwLidarProcLib.h"

/////////////////////////////////////////////////////////////////////////////////////
// 
//   ȫ�ֱ�������
//
/////////////////////////////////////////////////////////////////////////////////////
int gWidth,gHeight;
static RMW_LIDAR_3D_POINT_ORG  gData[MAX_LIDAR_MAX_POINT_NUM];


void testCross()
{
	char filePathLidar[100][256];
 	char fileName[256];
	int i,k,N;
	clock_t t1,t2;
	int begin[100],end[100];

	//step.0---------------------���ݼ�--------------------------------------//
	//���ݼ�01
	begin[1]=9570; 
	end[1]=9756;
	strcpy(filePathLidar[1],"D:\\018_3DProc\\0_Data\\64_LIDAR\\01_���ϰ�\\64");
	//���ݼ�02
	begin[2]=8900; 
	end[2]=9115;
	strcpy(filePathLidar[2],"D:\\018_3DProc\\0_Data\\64_LIDAR\\02_������\\64");
	//���ݼ�03
	begin[3]=7050; 
	end[3]=7300;
	strcpy(filePathLidar[3],"D:\\018_3DProc\\0_Data\\64_LIDAR\\03_��·\\64");
	//���ݼ�04
	begin[4]=5500; 
	end[4]=5640;
	strcpy(filePathLidar[4],"D:\\018_3DProc\\0_Data\\64_LIDAR\\04_խ��\\64");
	//���ݼ�05
	begin[5]=9300; 
	end[5]=9500;
	strcpy(filePathLidar[5],"D:\\018_3DProc\\0_Data\\64_LIDAR\\05_����\\64");
	//���ݼ�06
	begin[6]=5350; 
	end[6]=5420;
	strcpy(filePathLidar[6],"D:\\018_3DProc\\0_Data\\64_LIDAR\\06_��·��1\\64");
	//���ݼ�07
	begin[7]=6200; 
	end[7]=6360;
	strcpy(filePathLidar[7],"D:\\018_3DProc\\0_Data\\64_LIDAR\\07_��·��2\\64");
	//���ݼ�08
	begin[8]=950; 
	end[8]=1050;
	strcpy(filePathLidar[8],"D:\\018_3DProc\\0_Data\\64_LIDAR\\08_���ϰ�_0815\\64");
	//���ݼ�09
	begin[9]=720; 
	end[9]=780;
	strcpy(filePathLidar[9],"C:\\ˮͰ_0820\\03_��Ͱ���ڵ�_ֱ��\\64");
	//���ݼ�10
	begin[10]=900; 
	end[10]=1500;
	strcpy(filePathLidar[10],"C:\\ˮͰ_0820\\01_��Ͱ���ڵ�_���Ƕ����\\64");
	//step.1---------------------��ʼ��--------------------------------------//
    RmwLidarRoadCross_Initialize( 5,//դ��ˮƽ����ľ���,��λcm,5cm�Ա����������Ӻͺ��ʵĴ����ٶ�
		                          2, //դ��ֱ����ľ���,��λcm,2cm�Ա������ֿܷ�
		                          40*100, //դ��ͼ��Ŀ��,��λcm,���Ҹ�20m
		                          -6*100, //դ��ͼ�����ʼ����,��λcm,�״��6m
  		                          10*100,  //դ��ͼ�����ֹ����,��λcm,�״�ǰ10m
							      30, //�ϰ�����С���,5cm*6��դ��=30cm
							      12 //�ϰ�����С����,2cm*6��դ��=12cm
				                );
	//step.2---------------------ִ��----------------------------------------//
	k=6;
	for(i=begin[k];i<=end[k];i++)
	{
		//i=6250;
		//��ȡ�״�����
		sprintf(fileName,"%s\\3D64_%010d.bin",filePathLidar[k],i);
		N=RmwReadLidar64Data(fileName,gData,MAX_LIDAR_MAX_POINT_NUM);
		if (N==0)
		{
			//printf("\n%s read err!",fileName);
			continue;
		}
		printf("\n%s read ok(%d)!",fileName,N);
		//ִ��
		t1=clock();
		RmwLidarRoadCross_DoNext( gData,
					              N,
					              i,
					              //true
								  false
					            );
		t2=clock();
		printf("\n%dms",t2-t1);
	}
	printf("\nPress any key to continue....");
	getchar();
	return;
}

/////////////////////////////////////////////////////////////////////////////////////
// 
//   ���Ժ���
//
/////////////////////////////////////////////////////////////////////////////////////
void testBoundary()
{
	BYTE *pDrawImg;
	char filePathLidar[100][256];
 	char fileName[256];
	int width,height;
	int i,j,k,m,N;
	clock_t t1,t2;
	int begin[100],end[100];
	int nLineNum;

	//step.0---------------------���ݼ�--------------------------------------//
	//���ݼ�01
	begin[1]=9570; 
	end[1]=9756;
	strcpy(filePathLidar[1],"D:\\018_3DProc\\0_Data\\64_LIDAR\\01_���ϰ�\\64");
	//���ݼ�02
	begin[2]=8900; 
	end[2]=9115;
	strcpy(filePathLidar[2],"D:\\018_3DProc\\0_Data\\64_LIDAR\\02_������\\64");
	//���ݼ�03
	begin[3]=7050; 
	end[3]=7300;
	strcpy(filePathLidar[3],"D:\\018_3DProc\\0_Data\\64_LIDAR\\03_��·\\64");
	//���ݼ�04
	begin[4]=5500; 
	end[4]=5640;
	strcpy(filePathLidar[4],"D:\\018_3DProc\\0_Data\\64_LIDAR\\04_խ��\\64");
	//���ݼ�05
	begin[5]=9300; 
	end[5]=9500;
	strcpy(filePathLidar[5],"D:\\018_3DProc\\0_Data\\64_LIDAR\\05_����\\64");
	//���ݼ�06
	begin[6]=5350; 
	end[6]=5420;
	strcpy(filePathLidar[6],"D:\\018_3DProc\\0_Data\\64_LIDAR\\06_��·��1\\64");
	//���ݼ�07
	begin[7]=6200; 
	end[7]=6360;
	strcpy(filePathLidar[7],"D:\\018_3DProc\\0_Data\\64_LIDAR\\07_��·��2\\64");
	//���ݼ�08
	begin[8]=950; 
	end[8]=1050;
	strcpy(filePathLidar[8],"D:\\018_3DProc\\0_Data\\64_LIDAR\\08_���ϰ�_0815\\64");
	//���ݼ�09
	begin[9]=720; 
	end[9]=780;
	strcpy(filePathLidar[9],"C:\\ˮͰ_0820\\03_��Ͱ���ڵ�_ֱ��\\64");
	//���ݼ�10
	begin[10]=900; 
	end[10]=1500;
	strcpy(filePathLidar[10],"C:\\ˮͰ_0820\\01_��Ͱ���ڵ�_���Ƕ����\\64");
	//���ݼ�11
	begin[11]=3430; 
	end[11]=3484;
	strcpy(filePathLidar[11],"C:\\ˮͰ_0820\\05_��Ͱ�����ڵ�_ֱ��\\64");
	//���ݼ�12
	begin[12]=4150; 
	end[12]=4450;
	strcpy(filePathLidar[12],"C:\\ˮͰ_0820\\06_��Ͱ�����ڵ�1.5m_�������\\64");
	//���ݼ�13
	begin[13]=4900; 
	end[13]=5000;
	strcpy(filePathLidar[13],"C:\\ˮͰ_0820\\07_��Ͱ�����ڵ�1.5m_ֱ��\\64");
	//���ݼ�14
	begin[14]=3715; 
	end[14]=3984;
	strcpy(filePathLidar[14],"C:\\ˮͰ_0820\\04_��Ͱ�����ڵ�_�������\\64");
	//���ݼ�14
	begin[15]=888; 
	end[15]=1208;
	strcpy(filePathLidar[15],"D:\\018_3DProc\\0_Data\\64_LIDAR\\08_���");
	//���ݼ�14
	begin[16]=0; 
	end[16]=62;
	strcpy(filePathLidar[16],"D:\\018_3DProc\\0_Data\\64_LIDAR\\10_�����");

	//���ݼ�14
	begin[17]=151; 
	end[17]=280;
	strcpy(filePathLidar[17],"D:\\018_3DProc\\0_Data\\64_LIDAR\\11_·׶");

	//���ݼ�14
	begin[18]=2400; 
	end[18]=2600;
	strcpy(filePathLidar[18],"D:\\018_3DProc\\0_Data\\64_LIDAR\\12_����");
	
	//���ݼ�15
	begin[19]=601; 
	end[19]=1054;
	strcpy(filePathLidar[19],"D:\\018_3DProc\\0_Data\\64_LIDAR\\13_ƽ��·�渺�ϰ�");


	//���ݼ�16
	//begin[20]=222; 
	begin[20]=810; 
	//end[20]=810;//1157;
	end[20]=1157;
	strcpy(filePathLidar[20],"F:\\����_��·_0905");
	//���ݼ�16
	begin[21]=2900; 
	end[21]=3200;
	strcpy(filePathLidar[21],"H:\\bin_0909");
	strcpy(filePathLidar[21],"D:\\018_3DProc\\0_Data\\64_LIDAR\\14_��·"); //2940���������ߵĹ�
	//���ݼ�22
	begin[22]=0; 
	end[22]=1000;
	strcpy(filePathLidar[22],"H:\\Hole_DM_0923");
    strcpy(filePathLidar[22],"H:\\Hole_BYD_0923"); //0,310[366]
	//strcpy(filePathLidar[22],"H:\\JieMu_BYD_0924"); 
	//strcpy(filePathLidar[22],"H:\\DM_0924\\DM_����Ͱ_0924");  
	//strcpy(filePathLidar[22],"H:\\DM_0924\\DM_ˮ�Ӽ���_0924");  //250,360
	strcpy(filePathLidar[22],"H:\\0909_��·����");  //250,360
	strcpy(filePathLidar[22],"D:\\018_3DProc\\0_Data\\64_LIDAR\\15_����");
	strcpy(filePathLidar[22],"H:\\DM_����_1013");
	strcpy(filePathLidar[22],"H:\\BYD_���ϰ�_1015");
	strcpy(filePathLidar[22],"H:\\DM_����_1016");

	//���ݼ�23
	begin[23]=0; 
	end[23]=1600;
	strcpy(filePathLidar[23],"H:\\BYD_��ҩ��_1013");
	strcpy(filePathLidar[23],"H:\\BYD_���_1014");
	strcpy(filePathLidar[23],"H:\\BYD_��̳_1014");

	//step.1---------------------��ʼ��--------------------------------------//
	RmwLidarObstacleBoundary_Initialize( 
	                                   //դ��
	                                   5,//դ��ˮƽ����ľ���,��λcm,5cm�Ա����������Ӻͺ��ʵĴ����ٶ�
		                               2, //դ��ֱ����ľ���,��λcm,2cm�Ա������ֿܷ�
		                               40*100, //դ��ͼ��Ŀ��,��λcm,���Ҹ�20m
		                               -6*100, //դ��ͼ�����ʼ����,��λcm,�״��6m
									   30*100,  //դ��ͼ�����ֹ����,��λcm,��(�״�)ǰ30m
									   //��·�߽�
									   6*100, //��·�߽����ʼ����,��λcm,�״�ǰ6m
  		                               //18*100, //��·�߽����ֹ����,��λcm,��(�״�)ǰ18m,��ֹ���
  		                               25*100, //��·�߽����ֹ����,��λcm,��(�״�)ǰ18m,��ֹ���
								       3*100, //·����խ1������,3m
				                       15*100, //·�����5������,20m
									   //true, //�Ƿ��ǽṹ����·,·�߱�·�����(�вݻ���·��)�����Ե����ṹ����·
				 					   false,
									   //���ϰ�
                                       30, //���ϰ�����С���,5cm*6��դ��=30cm
							           12, //���ϰ�����С����,2cm*6��դ��=12cm
									   //���ϰ�
								       65, //���ϰ�����С���,75cm
								       150 //���ϰ��������,150cm
				                   );
	//step.2---------------------ִ��----------------------------------------//
	k=22;
	//for(i=begin[k];i<=end[k];i+=10)
	//for(i=begin[k];i<=end[k];i++)
	for(i=166;i<180;i++)
	{
		//if (i!=165) continue;
		//if (i!=215) continue;
		//if (i!=226) continue;
		//if (i!=264) continue;
		//////////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//��ȡ�״�����
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
		sprintf(fileName,"%s\\3D64_%010d.bin",filePathLidar[k],i);
		N=RmwReadLidar64Data(fileName,gData,MAX_LIDAR_MAX_POINT_NUM);
		if (N==0)
		{
			//printf("\n%s read err!",fileName);
			continue;
		}
		printf("\n%s read ok(%d)!",fileName,N);

	    //////////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//ִ��: ���߼��
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
		t1=clock();
		RmwLidarObstacleBoundary_DoNext( gData,
					                     N,
					                     i,
					                     //true
									     false
					                   );
		//���߽��
	    double A1,B1,C1;
	    double A2,B2,C2;
		bool OK1,OK2;
    	RmwLidarRoadBoundary_GetBoundaryLine( &A1,&B1,&C1,&OK1,
		                                      &A2,&B2,&C2,&OK2
		                                    );
		t2=clock();
		printf("\n%dms",t2-t1);
		///////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//ִ��: ���ϰ����
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
		t1=clock();
		int nNegativeObstalce=0;
		RMW_LIDAR_3D_OBSTACLE pNegativeObstalce[200];
		for(int scanlineID=1;scanlineID<40;scanlineID++)
		{
			//if (scanlineID!=19) continue;

			//if (scanlineID!=31) continue;
			//if (scanlineID!=33) continue;
			RmwLidarNegativeObstalce_DoNext( scanlineID, //�ߺ�
				                             50, //�ϰ���y�������С�仯,���ߺŲ�ͬ���仯,50cm
		                                     150, //�ϰ���y��������仯,���ߺŲ�ͬ���仯,150cm
											 i, 
				                             false
				                             //true
				                           );



			//���
			nNegativeObstalce += RmwLidarNegativeObstalce_GetObstacleRegions(pNegativeObstalce+nNegativeObstalce,200-nNegativeObstalce);
		}
		t2=clock();
		printf("\n%dms",t2-t1);
		//////////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//ִ��: Ŀ�����
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
		int pTargetLeft[50];
		int pTargetRight[50];
		int pTargetTop[50];
		int pTargetBottom[50];
		int nTarget;
	    t1=clock();
        //ִ��
		RmwLidarTarget_DoNext( i,
			                  //true
							  false
			                );
		//���
        nTarget=RmwLidarTarget_GetTargetPos(pTargetLeft,pTargetRight,pTargetTop,pTargetBottom,50);
     	t2=clock();
		printf("\n%dms",t2-t1);   
		//////////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//����
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
        //pDrawImg=RmwLidarGrid_GetScanlineTypeImg(&width,&height);
		pDrawImg=RmwLidarGrid_GetObstacleTypeImg(&width,&height);
		//����-��ѻ�ɫ,������ɫ
		nLineNum=0;
		if (OK1)
		{
			nLineNum++;
			RmwDrawABCLine(pDrawImg,width,height,A1,B1,C1,252);
		}
		if (OK2)
		{
			nLineNum++;
			RmwDrawABCLine(pDrawImg,width,height,A2,B2,C2,252);
		}
		//���Ŀ��
		for(j=0;j<nTarget;j++)
		{
			RmwRectFill( pDrawImg,
				         width,height,
					     pTargetLeft[j]-2,
					     pTargetRight[j]+2,
					     pTargetTop[j]-2,
					     pTargetBottom[j]+2,
					     253 //��ɫ
				      );
		}
		//���ϰ�
		for(j=0;j<nNegativeObstalce;j++)
		{
			for(m=0;m<3;m++)
			RmwRectDraw( pDrawImg,
				         width,height,
						 pNegativeObstalce[j].gridX1-m,
						 pNegativeObstalce[j].gridX2+m,
						 pNegativeObstalce[j].gridY1-m,
						 pNegativeObstalce[j].gridY2+m,
					     252 //��ɫ
				      );
		}
		//����
		sprintf( fileName,"D:\\tmp\\res_RmwRoadBoundary_frameID=%010d_1_LineNum=%d_NegativeObstalceNum=%d_targetNum=%d.bmp",
			     i,
				 nLineNum,
				 nNegativeObstalce,
				 nTarget
			   );
		//if (nLineNum==2)
		//Rmw_Write8BitImgMark(pDrawImg,width,height,fileName);
		//�õ��û��Լ�������ϰ�ͼ��
		BYTE *pObstacleBinImg=RmwLidarGrid_GetObstacleBinImg( 3, //������ֵ��
									                          2, //Զ����ֵС
	                                                          &width,
									                          &height
								                           );
		sprintf(fileName,"D:\\tmp\\res_RmwRoadBoundary_frameID=%010d_1_ObstalceMap.bmp",i);
		Rmw_Write8BitImgMark(pObstacleBinImg,width,height,fileName);
		//�߶�ͼ
		BYTE *pZImg;
		int resolutionZCM,offset;
		pZImg=RmwLidarGrid_GetZImg(&width,&height,&resolutionZCM,&offset); 
		sprintf(fileName,"D:\\tmp\\res_RmwRoadBoundary_frameID=%010d_3_Z_%dcm.bmp",i,resolutionZCM);
		//Rmw_Write8BitImgMark(pZImg,width,height,fileName);
		//////////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//��һ֡
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
	}
	printf("\nPress any key to continue....");
	getchar();
	return;
}

void main()
{
	testBoundary();
	//testCross();
	return;
}

  