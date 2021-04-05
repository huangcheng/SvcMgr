
// SvcMgrDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "Svc.h"
#include "atltypes.h"
#include "SvcAddDlg.h"
#include "CmnHeader.h"

#define  COL_SVC        0
#define  COL_STATUS     1
#define  COL_TYPE       2
#define  COL_NAME       3
#define  COL_PATH       4
#define  COL_COMPANY    5

#define  WM_REFRESH     WM_USER+101

//刷新ListCtrl 内容
#define  REFRESH        nSelectedItem = GetSelectedItem();                 \
                        m_SvcList.DeleteAllItems();                        \
                        RefreshView();                                     \
						m_SvcList.SetFocus();                              \
						m_SvcList.SetItemState(nSelectedItem,   LVIS_SELECTED,   LVIS_SELECTED);

//更新特定行,特定列的信息
#define  UPDATETEXT(nCol, lpText)     	int nIndex = GetSelectedItem();                \
                                        m_SvcList.SetItemText(nIndex, nCol, lpText);

// CSvcMgrDlg 对话框
class CSvcMgrDlg : public CDialogEx
{
// 构造
public:
	CSvcMgrDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CSvcMgrDlg();
// 对话框数据
	enum { IDD = IDD_SVCMGR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_SvcList;
    CSvc *pSvc;

	
	afx_msg void OnClose();

	//刷新ListCtrl
	BOOL RefreshView(void);

	//将服务的可执行文件路径添加到ListCtrl
	BOOL AppendSvcInfo(int, LPCTSTR);

	//更改ListCtrl的大小(用于窗口大小改变及最大化)
	void AdjustSize(CRect rcChanged);
private:
	CRect rcParent;
public:
	CRect rcList;
	int nSelectedItem;
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

	afx_msg void OnNMRClickListSvc(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnServiceStart();
	CString strSvcName;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//初始化ListCtrl 头
	void AddListHeaders(void);

	//获得选中的ListCtrl项目
	int GetSelectedItem(void);
	
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//将公司名添加到ListCtrl
	void AddCompany(int nItem, LPCTSTR lpSvcFullName);
	afx_msg void OnServiceStop();
	
	afx_msg void OnServiceSuspend();
	
	//刷新ListCtrl的消息,废弃
	//afx_msg LRESULT OnRefresh(WPARAM wParam, LPARAM lParam);

	//监视ListCtrl项目变化的线程,废弃
	//static UINT  __cdecl MonitorThread(LPVOID pParam);
	afx_msg void OnServiceResume();
	afx_msg void OnServiceRestart();
	afx_msg void OnBootAuto();
	afx_msg void OnBootManual();
	afx_msg void OnBootDisable();
	afx_msg void OnServiceUninstall();
	afx_msg void OnServiceInstall();
	afx_msg void OnSearchGoogle();
	afx_msg void OnSearchBing();
	afx_msg void OnSearchBaidu();
};
