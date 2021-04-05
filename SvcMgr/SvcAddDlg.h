#pragma once
#include "CmnHeader.h"
#include "afxwin.h"

// CSvcAddDlg 对话框

class CSvcAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSvcAddDlg)

public:
	CSvcAddDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSvcAddDlg();

// 对话框数据
	enum { IDD = IDD_SVC_INSTALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	SERVICEINSTALLINFO sii;
	virtual BOOL OnInitDialog();
	int nSvcType;
	int nStartType;
	afx_msg void OnBnClickedButtonInstall();
	CString m_strSvcName;
	CString m_StrSvcPath;
	afx_msg void OnBnClickedButtonCancle();
	afx_msg void OnBnClickedButtonBrowse();
	CString m_strDisplay;
};
