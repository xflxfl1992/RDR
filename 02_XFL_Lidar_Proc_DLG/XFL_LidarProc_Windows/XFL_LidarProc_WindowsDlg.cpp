
// readlidarDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "../../01_XFL_Lidar64Exe/0_XFL_Lidar64CPP/NJUST_ALV_BYD_H/ALV_BYD.h"
#include "XFL_LidarProc_Windows.h"
#include "XFL_LidarProc_WindowsDlg.h"
#include "afxdialogex.h"
#include <vfw.h>
#include "../../01_XFL_Lidar64Exe/0_XFL_Lidar64CPP/XFL_Lidar64extern.h"
#include "../../01_XFL_Lidar64Exe/0_XFL_Lidar64CPP/XFL_Lidar64PreProc.h"
#include "../../01_XFL_Lidar64Exe/0_XFL_Lidar64CPP/XFL_Lidar64ProcFrame.h"


#define WHOLE_SIZE_WIDTH  NJUST_LIDAR64_HORIZONTAL_GRID_NUM
#define WHOLE_SIZE_HEIGHT  NJUST_LIDAR64_VERTICAL_GRID_NUM
#define SINGLE_GRID_HERIZON NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM  //cm
#define SINGLE_GRID_VERTICAL NJUST_LIDAR64_VERTICAL_RESOLUTION_CM //cm
#define WHOLESIZE (WHOLE_SIZE_WIDTH*WHOLE_SIZE_HEIGHT) 

//����ݴ�ṹ��
 static NJUST_3D64_OBS_TO_FU             gNJUST_3D64_Obs;
 static NJUST_3D64_ROAD_BOUNDARY_TO_FU   gNJUST_3D64_Boundary;
 static NJUST_3D64_GRID_TO_FU      gNJUST_3D64_grid;
 static NJUST_3D64_CROSS_TO_FU     gNJUST_3D64_cross;

int FrameID=0;
CRect m_rect;
int DataSource = 0;
int gWholeSizeMap[WHOLE_SIZE_HEIGHT][WHOLE_SIZE_WIDTH];
BYTE gWholeSizeMap_BYTE[WHOLESIZE];

char g_packData[LIDAR64_IP_DATA_SIZE];//windows�µ�һ����������
BYTE *pMskImg;//�ݴ���

//MC����
static NJUST_MC_NAV_INFO MC_data;

//---------------------ȫ�ֱ������壬���ڽ�������----------------//����������¶������
unsigned int gMcID = 0;
unsigned int gNavIDStart = 0;
unsigned int gNavIDEnd = 1;
 unsigned int gNavID = 0;  
static bool isFirst = true;
NJUST_MC_NAV_INFO	gNavInfo;
int gPL_CMD = 0;//PL������ǰ����������
bool _gDoDebug = false;

//---------------------���캯��-----------------------------------//
XFL_Lidar64PreProc XFL_Preproc;
XFL_Lidar64ProcFrame _XFLLidar64ProcFrame;


extern void XFL_DebugFile(int i,char *pStr);

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CreadlidarDlg �Ի���




CreadlidarDlg::CreadlidarDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CreadlidarDlg::IDD, pParent)
	, m_FrameID(0)
	, m_PlaySpeed(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CreadlidarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_CTL_next);
	DDX_Control(pDX, IDC_PIC, m_Pic);
	DDX_Text(pDX, IDC_EDIT2, m_FrameID);
	DDX_Text(pDX, IDC_EDIT3, m_PlaySpeed);
	DDX_Control(pDX, IDC_COMBO2, m_DataSource);
}

BEGIN_MESSAGE_MAP(CreadlidarDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CreadlidarDlg::OnBnClicked_next)
	//	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2, &CreadlidarDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CreadlidarDlg::OnBnClickedButton3)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO2, &CreadlidarDlg::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CreadlidarDlg ��Ϣ�������

BOOL CreadlidarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_pBmpInfo = (BITMAPINFO*)new BYTE[sizeof(BITMAPINFOHEADER)+256 * sizeof(RGBQUAD)];
	memset(gWholeSizeMap_BYTE,0,WHOLESIZE);
	GetClientRect(&m_rect);//ȡԭʼ�Ի���Ĵ�С

	m_DataSource.SetCurSel(0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CreadlidarDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CreadlidarDlg::OnPaint()
{

	int			i;
	CRect		rect;
	HDC			hDC;           //��ʾ�豸���
	HDRAWDIB	hDD;			//��ͼ���
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		//Step.1 ---------- ������Ϣͷ��Ϣ -----------------------------//
		m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		m_pBmpInfo->bmiHeader.biWidth = WHOLE_SIZE_WIDTH;
		m_pBmpInfo->bmiHeader.biHeight = WHOLE_SIZE_HEIGHT;
		m_pBmpInfo->bmiHeader.biPlanes = 1;
		m_pBmpInfo->bmiHeader.biBitCount = 8;
		m_pBmpInfo->bmiHeader.biCompression = 0;
		m_pBmpInfo->bmiHeader.biSizeImage = WHOLESIZE;
		m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
		m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
		m_pBmpInfo->bmiHeader.biClrUsed = 0;
		m_pBmpInfo->bmiHeader.biClrImportant = 0;
		//Step.2 ---------- ���õ�ɫ�� ---------------------------------//
		for ( i = 0; i < 256; i++)
		{
			m_pBmpInfo->bmiColors[i].rgbBlue = i;
			m_pBmpInfo->bmiColors[i].rgbGreen = i;
			m_pBmpInfo->bmiColors[i].rgbRed = i;
			m_pBmpInfo->bmiColors[i].rgbReserved = 0;
		}	
		m_pBmpInfo->bmiColors[254].rgbBlue = 255;
		m_pBmpInfo->bmiColors[254].rgbGreen = 0;
		m_pBmpInfo->bmiColors[254].rgbRed = 0;
		m_pBmpInfo->bmiColors[253].rgbBlue = 0;
		m_pBmpInfo->bmiColors[253].rgbGreen = 0;
		m_pBmpInfo->bmiColors[253].rgbRed = 255;
		m_pBmpInfo->bmiColors[252].rgbBlue = 0;
		m_pBmpInfo->bmiColors[252].rgbGreen = 255;
		m_pBmpInfo->bmiColors[252].rgbRed = 0;
		m_pBmpInfo->bmiColors[251].rgbBlue = 155;
		m_pBmpInfo->bmiColors[251].rgbGreen = 100;
		m_pBmpInfo->bmiColors[251].rgbRed = 0;
		CPaintDC dc( &m_Pic );
		//Step.3 ---------- �ڿؼ�����ʾԭͼ ---------------------------//
		m_Pic.GetClientRect(&rect);
		hDC = dc.GetSafeHdc();
		hDD = DrawDibOpen();
		//int iMapMode = dc.SetMapMode(MM_ANISOTROPIC);
		//dc.SetViewportExt(1,-1);
		//	dc.SetViewportOrg(rect.left,rect.bottom);
		DrawDibDraw(hDD, hDC, rect.left, rect.top, rect.right - rect.left,rect.bottom - rect.top,
			(LPBITMAPINFOHEADER)m_pBmpInfo, gWholeSizeMap_BYTE, 
			-1,-1,WHOLE_SIZE_WIDTH,WHOLE_SIZE_HEIGHT, DDF_BACKGROUNDPAL);
		//dc.SetMapMode(iMapMode);
		DrawDibClose(hDD);
		//hDC = dc.GetSafeHdc();//realize dc
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CreadlidarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CreadlidarDlg::OnBnClicked_next()
{
	int i,j,k;
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// Step.1-----------------------------------------��ʼ��-------------------------------------------//
		UpdateData(TRUE);
		for ( i = 0; i <WHOLE_SIZE_HEIGHT; i++)
		{
			for ( j = 0; j < WHOLE_SIZE_WIDTH ; j++)
			{
				gWholeSizeMap[i][j] = 0;
				gWholeSizeMap_BYTE[i*WHOLE_SIZE_WIDTH+j] = 0;
			}
		}
		memset(&g_packData,0,LIDAR64_IP_DATA_SIZE);
		memset(&MC_data,0,sizeof(NJUST_MC_NAV_INFO));
		gNavID = m_FrameID;
		
	// Step.2-----------------------------------------���ݶ�ȡ-------------------------------------------//
		
		//char filename[256];
		char filename1[256];
		FILE *fp1;
		char filename2[256];
		FILE *fp2;
		int flen;
		bool isReady;
		char Para_txt[100];
		static bool isFirst=true;
		#ifdef _USE_BYD_PARAMETERS_
		sprintf_s(Para_txt,"BYD_InternParameters.txt");
		#endif
		#ifdef _USE_DM_PARAMETERS_
		sprintf(Para_txt,"DM_InternParameters.txt");
		#endif
		/////////////////////////////////////���״�����Ŀ¼//////////////////////////////////////////////AAAAAAAAAAAAAAAAAAA
		/*
		switch (DataSource)
		{//��·��1;���ϰ�;С·���ǽṹ����;��·��T��·�ڣ�;����;խ��2;խ��1;������;///������;խ��1;խ��2;����;��·��T��·�ڣ�;С·���ǽṹ����;���ϰ�;��·��1;
		case 0:
			sprintf_s(filename1, "E:\\Lidar_Data\\������\\binFileNameList.txt");
			break;
		case 1:
			sprintf_s(filename1, "E:\\Lidar_Data\\խ��1\\binFileNameList.txt");
			break;
		case 2:
			sprintf_s(filename1, "E:\\Lidar_Data\\խ��2\\binFileNameList.txt");
			break;
		case 3:
			sprintf_s(filename1, "E:\\Lidar_Data\\����\\binFileNameList.txt");
			break;
		case 4:
			sprintf_s(filename1, "E:\\Lidar_Data\\��·��T��·�ڣ�\\binFileNameList.txt");
			break;
		case 5:
			sprintf_s(filename1, "E:\\Lidar_Data\\С·���ǽṹ����\\binFileNameList.txt");
			break;
		case 6:
			sprintf_s(filename1, "E:\\Lidar_Data\\���ϰ�\\binFileNameList.txt");
			break;
		case 7:
			sprintf_s(filename1, "E:\\Lidar_Data\\��·��1\\binFileNameList.txt");
			break;
		default:
			break;
		}
		*/
		//sprintf_s(filename1, "E:\\0.�״�����\\BYD_����ȫ��_1013_ԭʼ\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\bin\\binFileNameList.txt");
		sprintf_s(filename1, "C:\\Users\\LeiLei\\Desktop\\���˳�_�״�ģ��\\XFL_Lidar64_Final\\RDR\\original_data\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\0.�״�����\\ˮͰ_0820_org\\binFileNameList.txt");
		//sprintf_s(filename1, "C:\\Users\\RMW\\Desktop\\XFL_Lidar64\\NJUST_3D_Lidar64_Final_2016\\bin_err\\binFileNameList.txt");
		//sprintf_s(filename1, "C:\\Users\\RMW\\Desktop\\bin_damai\\binFileNameList.txt");
		//sprintf_s(filename1, "C:\\Users\\RMW\\Desktop\\bin_ye\\error\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\xfl\\BYD_Hole\\binFileNameList.txt");		
		//sprintf_s(filename1, "E:\\xfl\\DM_Hole\\binFileNameList.txt");		
		//sprintf_s(filename1, "E:\\xfl\\BYD_JieMu_0924\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\xfl\\DM_Gua_0924\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\xfl\\DM_ˮ�Ӽ���_0924\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\xfl\\BYD_����_0925\\binFileNameList.txt");
		//sprintf_s(filename1, "C:\\Users\\RMW\\Desktop\\123\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\xfl\\DM_����_1013\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\0.�״�����\\BYD_fuzhangai_1015\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\xfl\\BYD_��ҩ��_1013\\binFileNameList.txt");
		//sprintf_s(filename1, "C:\\Users\\RMW\\Desktop\\test\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\0.�״�����\\20161016_3\\binFileNameList.txt");
		fopen_s(&fp1,filename1,"rb");
		if(fp1 == NULL)
		{
			KillTimer(1);
			MessageBox(TEXT("�޷����״�����Ŀ¼"),TEXT("֪ͨ"),MB_OK);
			return;
		}
		else 
		{
			fseek(fp1,0,SEEK_SET);
			fgets(filename2,255,fp1);
			flen=ftell(fp1);
			fseek(fp1,1L*flen*m_FrameID,SEEK_SET);
			//fseek(fp1,1L*flen*m_FrameID*6,SEEK_SET);//��6֡����һ֡
			fgets(filename2,flen-2,fp1);
		}

		////////////////////////////////////����Ŀ¼���״�����////////////////////////////////////////////
		fopen_s(&fp2,filename2,"rb");
		clock_t t1,t2;
		if(fp2 == NULL)
		{
			KillTimer(1);
			MessageBox(TEXT("�״��ļ����Ž���"),TEXT("֪ͨ"),MB_OK);    
			return;
		}
		else
		{
			if (isFirst)
			{
				XFL_Preproc.Initialize( &gNavID);
				_XFLLidar64ProcFrame.Initialize(&gNavID);
				isFirst=false;
			}
			t1 = clock();
			while (fread(&g_packData,1,LIDAR64_IP_DATA_SIZE,fp2)>0)
			{
				isReady = XFL_Preproc.DataTransformLoop();
				if (isReady) break;
			}
			t2 = clock();

			//XFL_DebugFile(t2-t1,"time");
		}
		_XFLLidar64ProcFrame.ProcFrameLoop();

		fclose(fp1);
		fclose(fp2);
		////////////////////////////////////ȡ���������ݣ�����ձ���//////////////////////////////////
		memcpy(&gNJUST_3D64_Obs,&_XFLLidar64ProcFrame.m_3D64Obs,sizeof(NJUST_3D64_OBS_TO_FU));
		memcpy(&gNJUST_3D64_Boundary, &_XFLLidar64ProcFrame.m_3D64Boundary,sizeof(NJUST_3D64_ROAD_BOUNDARY_TO_FU));
		memcpy(&gNJUST_3D64_grid,&_XFLLidar64ProcFrame.m_3D64Grid,sizeof(NJUST_3D64_GRID_TO_FU));
		memcpy(&gNJUST_3D64_cross,&_XFLLidar64ProcFrame.m_3D64Cross,sizeof(NJUST_3D64_CROSS_TO_FU));
		
		pMskImg = NULL;
		int gridXcm = _XFLLidar64ProcFrame.m_gridXcm;
		int gridYcm = _XFLLidar64ProcFrame.m_gridYcm;
		int width_gXFL = _XFLLidar64ProcFrame.m_widthCM/gridXcm;
		int begin_gXFL = _XFLLidar64ProcFrame.m_beginGridDistCM/gridYcm;
		int end_gXFL = _XFLLidar64ProcFrame.m_endGridDistCM/gridYcm;
		int height_gXFL = end_gXFL - begin_gXFL;
		pMskImg = _XFLLidar64ProcFrame.pObstacleTypeImg;
		int shrink_x = NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM/gridXcm;
		int shrink_y = NJUST_LIDAR64_VERTICAL_RESOLUTION_CM/gridYcm;

		memset(&_XFLLidar64ProcFrame.m_3D64Obs,0,sizeof(NJUST_3D64_OBS_TO_FU));
		memset(&_XFLLidar64ProcFrame.m_3D64Boundary,0,sizeof(NJUST_3D64_ROAD_BOUNDARY_TO_FU));
		memset(&_XFLLidar64ProcFrame.m_3D64Grid,0,sizeof(NJUST_3D64_GRID_TO_FU));
		memset(&_XFLLidar64ProcFrame.m_3D64Cross,0,sizeof(NJUST_3D64_CROSS_TO_FU));
		////////////////////////////////////�ڽ�������ʾ���////////////////////////////////////////////
		int tmp_i,tmp_j;
		for (i=0;i<height_gXFL;i++)
		{
			for ( j = 0; j < width_gXFL; j++)
			{
				//ԭʼͼ��ԭ�ֱ���ͼ����һ������ֵ���Ͱ�gWholeSizeMap_BYTE��Ӧ����Ϊ255
				if (pMskImg[i*width_gXFL+j])
				{
					tmp_i = (int)(i/shrink_y+(_XFLLidar64ProcFrame.m_beginGridDistCM+2000)/NJUST_LIDAR64_VERTICAL_RESOLUTION_CM);
					tmp_j = (int)(j/shrink_x);
					gWholeSizeMap_BYTE[tmp_i*WHOLE_SIZE_WIDTH+tmp_j]=255;
				}
			}
			
		}
#ifndef AAAAAA
		for (i=0;i<WHOLE_SIZE_HEIGHT;i++)
		{
			for ( j = 0; j < WHOLE_SIZE_WIDTH; j++)
			{
				//��·��ͼ
				if (gNJUST_3D64_cross.crossMsk[i*WHOLE_SIZE_WIDTH+j])
				{
					gWholeSizeMap_BYTE[i*WHOLE_SIZE_WIDTH+j]=254;
				}
				//�ϰ�ͼ
				if (gNJUST_3D64_grid.gridMsk[i*WHOLE_SIZE_WIDTH+j])
				{
					gWholeSizeMap_BYTE[i*WHOLE_SIZE_WIDTH+j]=253;
				}
				//�������Ͱ
				if (gNJUST_3D64_grid.gridMsk[i*WHOLE_SIZE_WIDTH+j]==NJUST_3D64_GRID_TYPE_OBJ_DETECTED)
				{
					gWholeSizeMap_BYTE[i*WHOLE_SIZE_WIDTH+j]=251;
				}
			}
			
		}

		int beginX,beginY,endX,endY;
		for (i = 0; i < gNJUST_3D64_Obs.nObj; i++)
		{
			beginX = (gNJUST_3D64_Obs.pObj[i].pPoint[0].x_cm+2000)/NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM;
			endX = (gNJUST_3D64_Obs.pObj[i].pPoint[1].x_cm+2000)/NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM;
			beginY = (gNJUST_3D64_Obs.pObj[i].pPoint[1].y_cm+2000)/NJUST_LIDAR64_VERTICAL_RESOLUTION_CM;
			endY = (gNJUST_3D64_Obs.pObj[i].pPoint[2].y_cm+2000)/NJUST_LIDAR64_VERTICAL_RESOLUTION_CM;
			if ((beginX<0)||(beginX>NJUST_LIDAR64_HORIZONTAL_GRID_NUM)||(beginY<0)||(beginY>NJUST_LIDAR64_VERTICAL_GRID_NUM))
				continue;
			for ( j = beginX; j < endX; j++)
			{
				gWholeSizeMap_BYTE[beginY*WHOLE_SIZE_WIDTH+j] = 252;
				gWholeSizeMap_BYTE[endY*WHOLE_SIZE_WIDTH+j] = 252;
			}
			for ( j = beginY; j < endY; j++)
			{
				gWholeSizeMap_BYTE[j*WHOLE_SIZE_WIDTH+beginX] = 252;
				gWholeSizeMap_BYTE[j*WHOLE_SIZE_WIDTH+endX] = 252;
			}
		}

		for (k = 0; k < gNJUST_3D64_Boundary.nPoint; k++)
		{
			i = (gNJUST_3D64_Boundary.Point[k].y_cm+2000)/NJUST_LIDAR64_VERTICAL_RESOLUTION_CM;
			j = (gNJUST_3D64_Boundary.Point[k].x_cm+2000)/NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM; 
			if (i<0||i>400||j<0||j>200)
			{
				continue;
			}
			gWholeSizeMap_BYTE[i*WHOLE_SIZE_WIDTH+j]=252;
		}
#endif

		////////////////////////////////////�ڽ����ϻ�����ʾ��ͼ////////////////////////////////////////////
		for (i=WHOLE_SIZE_HEIGHT/4-150/NJUST_LIDAR64_VERTICAL_RESOLUTION_CM;i<WHOLE_SIZE_HEIGHT/4+150/NJUST_LIDAR64_VERTICAL_RESOLUTION_CM;i++)
		{
			for ( j=WHOLE_SIZE_WIDTH/2-100/NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM;j<WHOLE_SIZE_WIDTH/2+100/NJUST_LIDAR64_HORIZONTAL_RESOLUTION_CM;j++)
			{
				gWholeSizeMap_BYTE[i*WHOLE_SIZE_WIDTH+j]=254;
			}
		}

		m_FrameID++;
		Invalidate(FALSE);
		UpdateData(FALSE);

}

void CreadlidarDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	if (nType != 1 && GetDlgItem(IDC_PIC))
	{
		//GetDlgItem(IDC_PIC)->GetClientRect(&rect);//ȡ����λ��
		//GetDlgItem(IDC_PIC)->MoveWindow(CRect(rect.left+5,rect.top + 50,rect.right-5,rect.bottom-5));
		GetDlgItem(IDC_PIC)->GetWindowRect(&rect);  
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������  
		//    cx/m_rect.Width()Ϊ�Ի����ں���ı仯����  
		rect.left=rect.left*cx/m_rect.Width();//�����ؼ���С  
		rect.right=rect.right*cx/m_rect.Width();  
		rect.top=rect.top*cy/m_rect.Height();  
		rect.bottom=rect.bottom*cy/m_rect.Height()-10;  
		
		/*//�ؼ�λ�ڶԻ�������
		LONG cWidth,cHeight;
		cWidth=m_rect.Width()-rect.right;
		cHeight=m_rect.Height()-rect.bottom;
		rect.left=cx-rect.Width()-cWidth;
		rect.right=cx-cWidth;   
		rect.top=cy-rect.Height()-cHeight;
		rect.bottom=cy-cHeight;
		*/
		GetDlgItem(IDC_PIC)->MoveWindow(rect);//���ÿؼ���С
	}

	//ƽ�ƶԻ����еĿؼ�
	if (nType != 1 && GetDlgItem(IDC_EDIT2))
	{
		GetDlgItem(IDC_EDIT2)->GetWindowRect(&rect);//ȡ����λ��
		GetDlgItem(IDC_EDIT2)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_EDIT3))
	{
		GetDlgItem(IDC_EDIT3)->GetWindowRect(&rect);//ȡ����λ��
		GetDlgItem(IDC_EDIT3)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_STATIC1))
	{
		GetDlgItem(IDC_STATIC1)->GetWindowRect(&rect);//ȡ����λ��
		GetDlgItem(IDC_STATIC1)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_STATIC2))
	{
		GetDlgItem(IDC_STATIC2)->GetWindowRect(&rect);//ȡ����λ��
		GetDlgItem(IDC_STATIC2)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_STATIC3))
	{
		GetDlgItem(IDC_STATIC3)->GetWindowRect(&rect);//ȡ����λ��
		GetDlgItem(IDC_STATIC3)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_STATIC4))
	{
		GetDlgItem(IDC_STATIC4)->GetWindowRect(&rect);//ȡ����λ��
		GetDlgItem(IDC_STATIC4)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDOK))
	{
		GetDlgItem(IDOK)->GetWindowRect(&rect);//ȡ����λ��
		GetDlgItem(IDOK)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDCANCEL))
	{
		GetDlgItem(IDCANCEL)->GetWindowRect(&rect);//ȡ����λ��
		GetDlgItem(IDCANCEL)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_BUTTON1))
	{
		GetDlgItem(IDC_BUTTON1)->GetWindowRect(&rect);//ȡ����λ��
		GetDlgItem(IDC_BUTTON1)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_BUTTON2))
	{
		GetDlgItem(IDC_BUTTON2)->GetWindowRect(&rect);//ȡ����λ��
		GetDlgItem(IDC_BUTTON2)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_BUTTON3))
	{
		GetDlgItem(IDC_BUTTON3)->GetWindowRect(&rect);//ȡ����λ��
		GetDlgItem(IDC_BUTTON3)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_COMBO2))
	{
		GetDlgItem(IDC_COMBO2)->GetWindowRect(&rect);//ȡ����λ��
		GetDlgItem(IDC_COMBO2)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	GetClientRect(&m_rect);
	Invalidate(FALSE);

}


void CreadlidarDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	SetTimer(1,m_PlaySpeed,NULL);
}


void CreadlidarDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(1);
}


void CreadlidarDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CreadlidarDlg::OnBnClicked_next();
	CDialogEx::OnTimer(nIDEvent);
}


void CreadlidarDlg::OnCbnSelchangeCombo2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DataSource= m_DataSource.GetCurSel();
	CString cs ;
	m_DataSource.GetLBText(DataSource,cs);
	MessageBox(cs);
	m_FrameID = 0;
	UpdateData(FALSE);
}
