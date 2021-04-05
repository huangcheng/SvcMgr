
// SvcMgrDlg.h : ͷ�ļ�
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

//ˢ��ListCtrl ����
#define  REFRESH        nSelectedItem = GetSelectedItem();                 \
                        m_SvcList.DeleteAllItems();                        \
                        RefreshView();                                     \
						m_SvcList.SetFocus();                              \
						m_SvcList.SetItemState(nSelectedItem,   LVIS_SELECTED,   LVIS_SELECTED);

//�����ض���,�ض��е���Ϣ
#define  UPDATETEXT(nCol, lpText)     	int nIndex = GetSelectedItem();                \
                                        m_SvcList.SetItemText(nIndex, nCol, lpText);

// CSvcMgrDlg �Ի���
class CSvcMgrDlg : public CDialogEx
{
// ����
public:
	CSvcMgrDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CSvcMgrDlg();
// �Ի�������
	enum { IDD = IDD_SVCMGR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_SvcList;
    CSvc *pSvc;

	
	afx_msg void OnClose();

	//ˢ��ListCtrl
	BOOL RefreshView(void);

	//������Ŀ�ִ���ļ�·����ӵ�ListCtrl
	BOOL AppendSvcInfo(int, LPCTSTR);

	//����ListCtrl�Ĵ�С(���ڴ��ڴ�С�ı估���)
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

	//��ʼ��ListCtrl ͷ
	void AddListHeaders(void);

	//���ѡ�е�ListCtrl��Ŀ
	int GetSelectedItem(void);
	
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//����˾����ӵ�ListCtrl
	void AddCompany(int nItem, LPCTSTR lpSvcFullName);
	afx_msg void OnServiceStop();
	
	afx_msg void OnServiceSuspend();
	
	//ˢ��ListCtrl����Ϣ,����
	//afx_msg LRESULT OnRefresh(WPARAM wParam, LPARAM lParam);

	//����ListCtrl��Ŀ�仯���߳�,����
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
