
// readlidarDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CreadlidarDlg �Ի���
class CreadlidarDlg : public CDialogEx
{
// ����
public:
	CreadlidarDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_READLIDAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	BITMAPINFO	*m_pBmpInfo;//������ʾͼ��


	// ���ɵ���Ϣӳ�亯��
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
