
// readlidarDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CreadlidarDlg 对话框
class CreadlidarDlg : public CDialogEx
{
// 构造
public:
	CreadlidarDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_READLIDAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	BITMAPINFO	*m_pBmpInfo;//用于显示图像


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_CTL_next;
	afx_msg void OnBnClicked_next();
	CStatic m_Pic;
//	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int m_FrameID;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_PlaySpeed;
	afx_msg void OnCbnSelchangeCombo2();
	CComboBox m_DataSource;
	afx_msg void OnStnClickedStatic4();
};
