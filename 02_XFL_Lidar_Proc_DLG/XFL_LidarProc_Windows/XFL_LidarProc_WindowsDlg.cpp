
// readlidarDlg.cpp : 实现文件
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

//输出暂存结构体
 static NJUST_3D64_OBS_TO_FU             gNJUST_3D64_Obs;
 static NJUST_3D64_ROAD_BOUNDARY_TO_FU   gNJUST_3D64_Boundary;
 static NJUST_3D64_GRID_TO_FU      gNJUST_3D64_grid;
 static NJUST_3D64_CROSS_TO_FU     gNJUST_3D64_cross;

int FrameID=0;
CRect m_rect;
int DataSource = 0;
int gWholeSizeMap[WHOLE_SIZE_HEIGHT][WHOLE_SIZE_WIDTH];
BYTE gWholeSizeMap_BYTE[WHOLESIZE];

char g_packData[LIDAR64_IP_DATA_SIZE];//windows下的一包数据输入
BYTE *pMskImg;//暂存器

//MC数据
static NJUST_MC_NAV_INFO MC_data;

//---------------------全局变量定义，用于接收数据----------------//在离线情况下都是虚的
unsigned int gMcID = 0;
unsigned int gNavIDStart = 0;
unsigned int gNavIDEnd = 1;
 unsigned int gNavID = 0;  
static bool isFirst = true;
NJUST_MC_NAV_INFO	gNavInfo;
int gPL_CMD = 0;//PL发来的前进后退命令
bool _gDoDebug = false;

//---------------------构造函数-----------------------------------//
XFL_Lidar64PreProc XFL_Preproc;
XFL_Lidar64ProcFrame _XFLLidar64ProcFrame;


extern void XFL_DebugFile(int i,char *pStr);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
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


// CreadlidarDlg 对话框




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


// CreadlidarDlg 消息处理程序

BOOL CreadlidarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_pBmpInfo = (BITMAPINFO*)new BYTE[sizeof(BITMAPINFOHEADER)+256 * sizeof(RGBQUAD)];
	memset(gWholeSizeMap_BYTE,0,WHOLESIZE);
	GetClientRect(&m_rect);//取原始对话框的大小

	m_DataSource.SetCurSel(0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CreadlidarDlg::OnPaint()
{

	int			i;
	CRect		rect;
	HDC			hDC;           //显示设备句柄
	HDRAWDIB	hDD;			//绘图句柄
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		//Step.1 ---------- 配置信息头信息 -----------------------------//
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
		//Step.2 ---------- 配置调色板 ---------------------------------//
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
		//Step.3 ---------- 在控件上显示原图 ---------------------------//
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CreadlidarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CreadlidarDlg::OnBnClicked_next()
{
	int i,j,k;
	// TODO: 在此添加控件通知处理程序代码

	// Step.1-----------------------------------------初始化-------------------------------------------//
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
		
	// Step.2-----------------------------------------数据读取-------------------------------------------//
		
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
		/////////////////////////////////////打开雷达数据目录//////////////////////////////////////////////AAAAAAAAAAAAAAAAAAA
		/*
		switch (DataSource)
		{//岔路口1;负障碍;小路（非结构化）;土路（T型路口）;长桥;窄桥2;窄桥1;悬挂物;///悬挂物;窄桥1;窄桥2;长桥;土路（T型路口）;小路（非结构化）;负障碍;岔路口1;
		case 0:
			sprintf_s(filename1, "E:\\Lidar_Data\\悬挂物\\binFileNameList.txt");
			break;
		case 1:
			sprintf_s(filename1, "E:\\Lidar_Data\\窄桥1\\binFileNameList.txt");
			break;
		case 2:
			sprintf_s(filename1, "E:\\Lidar_Data\\窄桥2\\binFileNameList.txt");
			break;
		case 3:
			sprintf_s(filename1, "E:\\Lidar_Data\\长桥\\binFileNameList.txt");
			break;
		case 4:
			sprintf_s(filename1, "E:\\Lidar_Data\\土路（T型路口）\\binFileNameList.txt");
			break;
		case 5:
			sprintf_s(filename1, "E:\\Lidar_Data\\小路（非结构化）\\binFileNameList.txt");
			break;
		case 6:
			sprintf_s(filename1, "E:\\Lidar_Data\\负障碍\\binFileNameList.txt");
			break;
		case 7:
			sprintf_s(filename1, "E:\\Lidar_Data\\岔路口1\\binFileNameList.txt");
			break;
		default:
			break;
		}
		*/
		//sprintf_s(filename1, "E:\\0.雷达数据\\BYD_坨里全程_1013_原始\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\bin\\binFileNameList.txt");
		sprintf_s(filename1, "C:\\Users\\LeiLei\\Desktop\\无人车_雷达模块\\XFL_Lidar64_Final\\RDR\\original_data\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\0.雷达数据\\水桶_0820_org\\binFileNameList.txt");
		//sprintf_s(filename1, "C:\\Users\\RMW\\Desktop\\XFL_Lidar64\\NJUST_3D_Lidar64_Final_2016\\bin_err\\binFileNameList.txt");
		//sprintf_s(filename1, "C:\\Users\\RMW\\Desktop\\bin_damai\\binFileNameList.txt");
		//sprintf_s(filename1, "C:\\Users\\RMW\\Desktop\\bin_ye\\error\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\xfl\\BYD_Hole\\binFileNameList.txt");		
		//sprintf_s(filename1, "E:\\xfl\\DM_Hole\\binFileNameList.txt");		
		//sprintf_s(filename1, "E:\\xfl\\BYD_JieMu_0924\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\xfl\\DM_Gua_0924\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\xfl\\DM_水坑加土_0924\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\xfl\\BYD_大坡_0925\\binFileNameList.txt");
		//sprintf_s(filename1, "C:\\Users\\RMW\\Desktop\\123\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\xfl\\DM_坨里_1013\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\0.雷达数据\\BYD_fuzhangai_1015\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\xfl\\BYD_弹药箱_1013\\binFileNameList.txt");
		//sprintf_s(filename1, "C:\\Users\\RMW\\Desktop\\test\\binFileNameList.txt");
		//sprintf_s(filename1, "E:\\0.雷达数据\\20161016_3\\binFileNameList.txt");
		fopen_s(&fp1,filename1,"rb");
		if(fp1 == NULL)
		{
			KillTimer(1);
			MessageBox(TEXT("无法打开雷达数据目录"),TEXT("通知"),MB_OK);
			return;
		}
		else 
		{
			fseek(fp1,0,SEEK_SET);
			fgets(filename2,255,fp1);
			flen=ftell(fp1);
			fseek(fp1,1L*flen*m_FrameID,SEEK_SET);
			//fseek(fp1,1L*flen*m_FrameID*6,SEEK_SET);//隔6帧处理一帧
			fgets(filename2,flen-2,fp1);
		}

		////////////////////////////////////根据目录打开雷达数据////////////////////////////////////////////
		fopen_s(&fp2,filename2,"rb");
		clock_t t1,t2;
		if(fp2 == NULL)
		{
			KillTimer(1);
			MessageBox(TEXT("雷达文件播放结束"),TEXT("通知"),MB_OK);    
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
		////////////////////////////////////取处理后的数据，并清空变量//////////////////////////////////
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
		////////////////////////////////////在界面上显示结果////////////////////////////////////////////
		int tmp_i,tmp_j;
		for (i=0;i<height_gXFL;i++)
		{
			for ( j = 0; j < width_gXFL; j++)
			{
				//原始图（原分辨率图中有一个点有值，就把gWholeSizeMap_BYTE对应点置为255
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
				//岔路口图
				if (gNJUST_3D64_cross.crossMsk[i*WHOLE_SIZE_WIDTH+j])
				{
					gWholeSizeMap_BYTE[i*WHOLE_SIZE_WIDTH+j]=254;
				}
				//障碍图
				if (gNJUST_3D64_grid.gridMsk[i*WHOLE_SIZE_WIDTH+j])
				{
					gWholeSizeMap_BYTE[i*WHOLE_SIZE_WIDTH+j]=253;
				}
				//侦查汽油桶
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

		////////////////////////////////////在界面上画车体示意图////////////////////////////////////////////
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
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	if (nType != 1 && GetDlgItem(IDC_PIC))
	{
		//GetDlgItem(IDC_PIC)->GetClientRect(&rect);//取窗口位置
		//GetDlgItem(IDC_PIC)->MoveWindow(CRect(rect.left+5,rect.top + 50,rect.right-5,rect.bottom-5));
		GetDlgItem(IDC_PIC)->GetWindowRect(&rect);  
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标  
		//    cx/m_rect.Width()为对话框在横向的变化比例  
		rect.left=rect.left*cx/m_rect.Width();//调整控件大小  
		rect.right=rect.right*cx/m_rect.Width();  
		rect.top=rect.top*cy/m_rect.Height();  
		rect.bottom=rect.bottom*cy/m_rect.Height()-10;  
		
		/*//控件位于对话框中央
		LONG cWidth,cHeight;
		cWidth=m_rect.Width()-rect.right;
		cHeight=m_rect.Height()-rect.bottom;
		rect.left=cx-rect.Width()-cWidth;
		rect.right=cx-cWidth;   
		rect.top=cy-rect.Height()-cHeight;
		rect.bottom=cy-cHeight;
		*/
		GetDlgItem(IDC_PIC)->MoveWindow(rect);//设置控件大小
	}

	//平移对话框中的控件
	if (nType != 1 && GetDlgItem(IDC_EDIT2))
	{
		GetDlgItem(IDC_EDIT2)->GetWindowRect(&rect);//取窗口位置
		GetDlgItem(IDC_EDIT2)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_EDIT3))
	{
		GetDlgItem(IDC_EDIT3)->GetWindowRect(&rect);//取窗口位置
		GetDlgItem(IDC_EDIT3)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_STATIC1))
	{
		GetDlgItem(IDC_STATIC1)->GetWindowRect(&rect);//取窗口位置
		GetDlgItem(IDC_STATIC1)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_STATIC2))
	{
		GetDlgItem(IDC_STATIC2)->GetWindowRect(&rect);//取窗口位置
		GetDlgItem(IDC_STATIC2)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_STATIC3))
	{
		GetDlgItem(IDC_STATIC3)->GetWindowRect(&rect);//取窗口位置
		GetDlgItem(IDC_STATIC3)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_STATIC4))
	{
		GetDlgItem(IDC_STATIC4)->GetWindowRect(&rect);//取窗口位置
		GetDlgItem(IDC_STATIC4)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDOK))
	{
		GetDlgItem(IDOK)->GetWindowRect(&rect);//取窗口位置
		GetDlgItem(IDOK)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDCANCEL))
	{
		GetDlgItem(IDCANCEL)->GetWindowRect(&rect);//取窗口位置
		GetDlgItem(IDCANCEL)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_BUTTON1))
	{
		GetDlgItem(IDC_BUTTON1)->GetWindowRect(&rect);//取窗口位置
		GetDlgItem(IDC_BUTTON1)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_BUTTON2))
	{
		GetDlgItem(IDC_BUTTON2)->GetWindowRect(&rect);//取窗口位置
		GetDlgItem(IDC_BUTTON2)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_BUTTON3))
	{
		GetDlgItem(IDC_BUTTON3)->GetWindowRect(&rect);//取窗口位置
		GetDlgItem(IDC_BUTTON3)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	if (nType != 1 && GetDlgItem(IDC_COMBO2))
	{
		GetDlgItem(IDC_COMBO2)->GetWindowRect(&rect);//取窗口位置
		GetDlgItem(IDC_COMBO2)->MoveWindow(CRect(rect.left+cx-m_rect.right+m_rect.left,rect.top,rect.right+cx-m_rect.right+m_rect.left,rect.bottom));
	}
	GetClientRect(&m_rect);
	Invalidate(FALSE);

}


void CreadlidarDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	SetTimer(1,m_PlaySpeed,NULL);
}


void CreadlidarDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
}


void CreadlidarDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CreadlidarDlg::OnBnClicked_next();
	CDialogEx::OnTimer(nIDEvent);
}


void CreadlidarDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	DataSource= m_DataSource.GetCurSel();
	CString cs ;
	m_DataSource.GetLBText(DataSource,cs);
	MessageBox(cs);
	m_FrameID = 0;
	UpdateData(FALSE);
}
