#include "../NJUST_ALV_BYD_H/ALV_BYD.h"

#ifdef _NJUST_OS_USE_WINDOWS_


#define  _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <memory.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../XFL_Lidar64Def.h" 
#include "../XFL_Lidar64Extern.h"
#include "../XFL_Lidar64PreProc.h"
#include "RmwLidarProcLib.h"

#include "bmpFile.h"

/////////////////////////////////////////////////////////////////////////////////////
// 
//   ȫ�ֱ�������
//
/////////////////////////////////////////////////////////////////////////////////////
int gWidth,gHeight;
static RMW_LIDAR_3D_POINT_ORG  gData[MAX_LIDAR_MAX_POINT_NUM];
char g_packData[LIDAR64_IP_DATA_SIZE];//windows�µ�һ����������
/////////////////////////////////////////////////////////////////////////////////////
// 
//   ȫ�ֱ�������
//
/////////////////////////////////////////////////////////////////////////////////////
unsigned int gNavID=0;
class XFL_Lidar64PreProc XFL_Preproc;
/////////////////////////////////////////////////////////////////////////////////////
// 
//   ���ļ�
//
/////////////////////////////////////////////////////////////////////////////////////
int RmwReadLidar64Data(char *fileName,RMW_LIDAR_3D_POINT_ORG *pData,int N)
{   //������Ч���ݵĸ���
	FILE *fp;
	int i,k;
		
	// step.0-------------���ļ�-------------------------------------------//
	fp=fopen(fileName,"rb");
	if (!fp) return 0;
	// step.1-------------���ļ�---------------------------------------------//
	i=k=0;
	while (fread(&pData[i],sizeof(RMW_LIDAR_3D_POINT_ORG),1,fp)>0)
	{
		//�Ƿ���Ч
		if ( (pData[i].x!=0)&&(pData[i].y!=0)&&(pData[i].z!=0) )
		{
 			//��һ��
			i++;
		}
		else 
		{
	 		k++;
	 	}
		//�Ƿ������
		if (i>=N) break;
	}
	// step.2-------------����-----------------------------------------------//
	fclose(fp);
    return i;
}

int XFL_TransformOneFrame(char *fileName)
{
	static bool isFirst=true;
	bool isReady;
	FILE *fp;
	int nIP;

	// step.1-------------��ʼ��---------------------------------------------//
	nIP=0;
	isReady=false;
	if (isFirst)
	{
		XFL_Preproc.Initialize( &gNavID);
		isFirst=false;
	}
	// step.2-------------���ļ�---------------------------------------------//
	fp=fopen(fileName,"rb");
	if(!fp) return 0;
	nIP=0;
	while (fread(&g_packData,1,LIDAR64_IP_DATA_SIZE,fp)>0)
	{
		isReady=XFL_Preproc.DataTransformLoop();
		nIP++;
		if (isReady) break;
	}
	// step.3-------------����-----------------------------------------------//
	printf("\nisReady=%d__nIP=%d",isReady,nIP);
	//��λ
	XFL_Preproc.Reset();
	//����
	return nIP;
}
/////////////////////////////////////////////////////////////////////////////////////
// 
//   ���Ժ���
//
/////////////////////////////////////////////////////////////////////////////////////
void testBoundary()
{
	BYTE *pScanlineTypeImg;
	char filePathLidar[100][256];
 	char fileName[256];
	int width,height;
	int i,j,k,N;
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
  		                               18*100, //��·�߽����ֹ����,��λcm,��(�״�)ǰ18m,��ֹ���
								       3*100, //·����խ1������,3m
				                       20*100, //·�����5������,20m
									   //���ϰ�
                                       30, //���ϰ�����С���,5cm*6��դ��=30cm
							           12, //���ϰ�����С����,2cm*6��դ��=12cm
									   //���ϰ�
								       40, //���ϰ�����С���,40cm
								       40*5 //���ϰ��������,200cm
				                   );
	//step.2---------------------ִ��----------------------------------------//
	k=15;
	for(i=begin[k];i<=end[k];i++)
	{
		//k=10,i=1334;
		//k=14;i=3894;
		//k=11,i=3431;
		//i=755; //����
		//i=1007; //����
		//i=1027; //����
		//i=1135; //����

		//k=1,i=9716;
		//k=9,i=742;
	  
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
		bool isLeftBst; //�Ƿ���߽�������ѵ�
    	RmwLidarRoadBoundary_GetBoundaryLine( &A1,&B1,&C1,&OK1,
		                                      &A2,&B2,&C2,&OK2,
											  &isLeftBst
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
		for(int scanlineID=6;scanlineID<30;scanlineID++)
		{
			//scanlineID=29;
			RmwLidarNegativeObstalce_DoNext( scanlineID, //�ߺ�
				                             5, //�ϰ���y�������С�仯,���ߺŲ�ͬ���仯,10cm
		                                     120, //�ϰ���y��������仯,���ߺŲ�ͬ���仯,120cm
											 i, 
				                             false
				                             //true
				                           );
			//break;
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
		int pLeft[10];
		int pRight[10];
		int pTop[10];
		int pBottom[10];
		int nTarget;
	    t1=clock();
        //ִ��
		RmwLidarTarge_DoNext( i,
			                  //true
							  false
			                );
		//���
        nTarget=RmwLidarTarge_GetTargetPos(pLeft,pRight,pTop,pBottom,10);
     	t2=clock();
		printf("\n%dms",t2-t1);   
		//////////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//����
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
        pScanlineTypeImg=RmwLidarGrid_GetScanlineTypeImg(&width,&height);
		//����-��Ѻ�ɫ,������ɫ
		if (OK1)
		{
			if (isLeftBst)
			{
				RmwDrawABCLine(pScanlineTypeImg,width,height,A1,B1,C1,255);
			}
			else
			{
				RmwDrawABCLine(pScanlineTypeImg,width,height,A1,B1,C1,254);
			}
		}
		if (OK2)
		{
			if (!isLeftBst)
			{
				RmwDrawABCLine(pScanlineTypeImg,width,height,A2,B2,C2,255);
			}
			else
			{
				RmwDrawABCLine(pScanlineTypeImg,width,height,A2,B2,C2,254);
			}
		}
		//��� 
		for(j=0;j<nTarget;j++)
		{
			RmwRectFill( pScanlineTypeImg,
				         width,height,
					     pLeft[j]-2,
					     pRight[j]+2,
					     pTop[j]-2,
					     pBottom[j]+2,
					     255 //��ɫ
				      );
		}
		//����
		sprintf(fileName,"D:\\tmp\\res_RmwRoadBoundary_frameID=%010d_1.bmp",i);
		Rmw_Write8BitImg2BmpFileMark254Green255Red( pScanlineTypeImg,
							                        width,height,
													fileName
											      );
		//�õ��ϰ��Ķ�ֵͼ��,�û��Լ�������ֵ
        BYTE *pObstacleBinImg=RmwLidarGrid_GetObstacleBinImg( 5, //������ֵ��
									                          3, //Զ����ֵС
	                                                          &width,
									                          &height
								                           );
		sprintf(fileName,"D:\\tmp\\res_RmwRoadBoundary_frameID=%010d_1_Obs.bmp",i);
		Rmw_Write8BitImg2BmpFileMark254Green255Red( pObstacleBinImg,
							                        width,height,
													fileName
											      );
		//////////////////////////////////////////////////////////////////////////////////////
	    //
		//
		//��һ֡
	    //
		//
	    //////////////////////////////////////////////////////////////////////////////////////
		//break;
	}
	printf("\nPress any key to continue....");
	getchar();
	return;
}

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
	k=10;
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
					              true
								  //false
					            );
		t2=clock();
		printf("\n%dms",t2-t1);
 		//����
		//break;
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
#endif