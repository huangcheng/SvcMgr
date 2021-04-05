
// SvcMgrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SvcMgr.h"
#include "SvcMgrDlg.h"
#include "afxdialogex.h"

//#include "CmnHeader.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CSvcMgrDlg 对话框




CSvcMgrDlg::CSvcMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSvcMgrDlg::IDD, pParent)
	, strSvcName(_T(""))
	, nSelectedItem(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pSvc = new CSvc;
}

CSvcMgrDlg::~CSvcMgrDlg()
{
	delete pSvc;
}

void CSvcMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SVC, m_SvcList);
}

BEGIN_MESSAGE_MAP(CSvcMgrDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SIZING()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SVC, &CSvcMgrDlg::OnNMRClickListSvc)
	ON_COMMAND(IDM_SERVICE_START, &CSvcMgrDlg::OnServiceStart)
	ON_WM_SIZE()
	ON_COMMAND(IDM_SERVICE_STOP, &CSvcMgrDlg::OnServiceStop)
	ON_COMMAND(IDM_SERVICE_SUSPEND, &CSvcMgrDlg::OnServiceSuspend)
//	ON_MESSAGE(WM_REFRESH, OnRefresh)
	ON_COMMAND(IDM_SERVICE_RESUME, &CSvcMgrDlg::OnServiceResume)
	ON_COMMAND(IDM_RERVICE_RESTART, &CSvcMgrDlg::OnServiceRestart)
	ON_COMMAND(IDM_BOOT_AUTO, &CSvcMgrDlg::OnBootAuto)
	ON_COMMAND(IDM_BOOT_MANUAL, &CSvcMgrDlg::OnBootManual)
	ON_COMMAND(IDM_BOOT_DISABLE, &CSvcMgrDlg::OnBootDisable)
	ON_COMMAND(IDM_SERVICE_UNINSTALL, &CSvcMgrDlg::OnServiceUninstall)
	ON_COMMAND(IDM_SERVICE_INSTALL, &CSvcMgrDlg::OnServiceInstall)
	ON_COMMAND(IDM_SEARCH_GOOGLE, &CSvcMgrDlg::OnSearchGoogle)
	ON_COMMAND(IDM_SEARCH_BING, &CSvcMgrDlg::OnSearchBing)
	ON_COMMAND(IDM_SEARCH_BAIDU, &CSvcMgrDlg::OnSearchBaidu)
END_MESSAGE_MAP()


// CSvcMgrDlg 消息处理程序

BOOL CSvcMgrDlg::OnInitDialog()
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


	//Modify styles of ListBox
	m_SvcList.SetView(LVS_REPORT);
	m_SvcList.ModifyStyle(0, LVS_SORTASCENDING);
	m_SvcList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	AddListHeaders();
	RefreshView();

	this->GetWindowRect(&rcParent);
	ScreenToClient(&rcParent);
	m_SvcList.GetWindowRect(&rcList);
	ScreenToClient(&rcList);

	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSvcMgrDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSvcMgrDlg::OnPaint()
{
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSvcMgrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSvcMgrDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialogEx::OnClose();
}


BOOL CSvcMgrDlg::RefreshView(void)
{
	
	CString strResult, strStatus;
	int nItem = 0;
	DWORD dwRequiedSize =0;
	HLOCAL hMem = NULL;
	LPENUM_SERVICE_STATUS_PROCESS pResult = NULL;
	
	dwRequiedSize = pSvc->EnumServices(NULL,0);

	ASSERT(dwRequiedSize != 0);
    
	hMem = LocalAlloc(LPTR, dwRequiedSize);
	if (hMem == NULL)
	{
		::ErrMsg(TEXT("分配内存失败!"));
		return FALSE;
	}

	pResult= (LPENUM_SERVICE_STATUS_PROCESS)hMem;
	pSvc->EnumServices(pResult,dwRequiedSize);

	strResult.Format(TEXT("Services Manager  共%d个服务"),pSvc->dwServicesReturned);
	SetWindowText(strResult);

	for (UINT i = 0; i<pSvc->dwServicesReturned; ++i)
	{

		nItem = m_SvcList.InsertItem(COL_SVC,pResult[i].lpServiceName);

		//m_SvcList.SetItemText(nItem,1,pResult[i].lpDisplayName);

		switch(pResult[i].ServiceStatusProcess.dwCurrentState)
		{
		case SERVICE_STOPPED:
			strStatus = TEXT("已停止");
			break;

		case SERVICE_RUNNING:
			strStatus = TEXT("已启动");
			break;

		case SERVICE_PAUSED:
			strStatus = TEXT("已暂停");
			break;
		}

		m_SvcList.SetItemText(nItem, COL_STATUS, strStatus);
		m_SvcList.SetItemText(nItem, COL_NAME,pResult[i].lpDisplayName);
		
		//switch(pResult[i].ServiceStatusProcess.dwServiceType)
		AppendSvcInfo(nItem, pResult[i].lpServiceName);
	}
	LocalFree(hMem);

	return TRUE;
}


BOOL CSvcMgrDlg::AppendSvcInfo(int nItem, LPCTSTR lpSvcName)
{
	CString strType, strFileFullName, strTemp;
	int nIndex = 0;
	pSvc->dwSizeNeeded = 0;
	HLOCAL hSvcInfo = NULL; 

	pSvc->GetSvcInfo(lpSvcName,NULL, 0);
	if (pSvc->dwSizeNeeded != 0)
	{
		hSvcInfo = LocalAlloc(LPTR, pSvc->dwSizeNeeded);
		if (hSvcInfo != NULL)
		{
			LPQUERY_SERVICE_CONFIG lpServiceConfig = (LPQUERY_SERVICE_CONFIG)hSvcInfo;
			pSvc->GetSvcInfo(lpSvcName, lpServiceConfig, pSvc->dwSizeNeeded);

			
			//此段用来清除个别字符串中的引号
			strTemp = lpServiceConfig->lpBinaryPathName;
			nIndex  = strTemp.ReverseFind('.');
			if (nIndex != 0)
			{
				strFileFullName = strTemp.Left(nIndex+4);
			}
			strFileFullName.Remove('\"');
			strFileFullName.TrimLeft();
			strFileFullName.TrimRight();



			m_SvcList.SetItemText(nItem, COL_PATH, strFileFullName);

			AddCompany(nItem, strFileFullName);

			switch(lpServiceConfig ->dwStartType)
			{
			case SERVICE_AUTO_START:
				strType = TEXT("自动");
				break;
			case SERVICE_DISABLED:
				strType = TEXT("已禁用");
				break;
			case SERVICE_DEMAND_START:
				strType = TEXT("手动");
				break;
			}
			m_SvcList.SetItemText(nItem, COL_TYPE,strType);

		}
		ASSERT(LocalFree(hSvcInfo) == NULL);
		hSvcInfo = NULL;
	}
	return TRUE;
}


void CSvcMgrDlg::AdjustSize(CRect rcChanged)
{
	CRect rcListNew;
	rcListNew.top   = rcChanged.top- (rcParent.top - rcList.top);
	rcListNew.left   = rcChanged.left- (rcParent.left - rcList.left);
	rcListNew.bottom = rcChanged.bottom - (rcParent.bottom - rcList.bottom);
	rcListNew.right = rcChanged.right- (rcParent.right - rcList.right);
	
	ScreenToClient(&rcListNew);
	m_SvcList.MoveWindow(rcListNew);
}


void CSvcMgrDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
	AdjustSize(*pRect);
}





void CSvcMgrDlg::OnNMRClickListSvc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CPoint ptCursor;
	GetCursorPos(&ptCursor);

	int nIndex = GetSelectedItem();

	CMenu MainMenu, *pMenuSub = NULL;
	MainMenu.LoadMenu(IDM_POPUP);
	if (MainMenu.GetSafeHmenu() != NULL)
	{
		pMenuSub  = MainMenu.GetSubMenu(0);
		if (pMenuSub != NULL)
		{
			pMenuSub->TrackPopupMenu(TPM_LEFTALIGN, ptCursor.x, ptCursor.y,this);
		}
	}
	
	MainMenu.DestroyMenu();
	*pResult = 0;
}


void CSvcMgrDlg::OnServiceStart()
{
	// TODO: 在此添加命令处理程序代码
	if (pSvc->StartSvc(strSvcName))
	{
		UPDATETEXT(COL_STATUS, TEXT("已启动"))
	}
	
}


BOOL CSvcMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F5)
	{
		REFRESH
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSvcMgrDlg::AddListHeaders(void)
{
	m_SvcList.InsertColumn(COL_SVC,TEXT("服务名"),LVCFMT_CENTER,150);
	m_SvcList.InsertColumn(COL_STATUS,TEXT("状态"),LVCFMT_CENTER,50);
	m_SvcList.InsertColumn(COL_TYPE,TEXT("类型"),LVCFMT_CENTER,50);
	m_SvcList.InsertColumn(COL_NAME,TEXT("显示名称"),LVCFMT_LEFT,150);
	m_SvcList.InsertColumn(COL_PATH,TEXT("映像路径"),LVCFMT_LEFT,400);
	m_SvcList.InsertColumn(COL_COMPANY,TEXT("公司"),LVCFMT_LEFT,150);
}


int CSvcMgrDlg::GetSelectedItem(void)
{
	UINT nIndex = 0;
	POSITION pos = m_SvcList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nIndex = m_SvcList.GetNextSelectedItem(pos);
		strSvcName = m_SvcList.GetItemText(nIndex,0);

	}
	return nIndex;
}




void CSvcMgrDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	
	// TODO: 在此处添加消息处理程序代码
	if (this->GetSafeHwnd() != NULL && m_SvcList.GetSafeHwnd() != NULL)
	{
		CRect rcParentNew;
		this->GetWindowRect(&rcParentNew);
		AdjustSize(rcParentNew);
	}
	
}



void CSvcMgrDlg::AddCompany(int nItem, LPCTSTR lpSvcFullName)
{
	LPTSTR lpCompany = NULL;
	DWORD dwRequiredSize = pSvc->GetResSize(lpSvcFullName);
	HLOCAL hMem = LocalAlloc(LPTR, dwRequiredSize);
	if (hMem != NULL)
	{
		lpCompany = (LPTSTR)LocalAlloc(LPTR, 50);
      
		pSvc->GetSvcCompany(hMem, lpSvcFullName, (LPTSTR*)&lpCompany,50);

		if (_tcscmp(lpCompany,TEXT("0")) != 0)
		{
			m_SvcList.SetItemText(nItem, COL_COMPANY, lpCompany);
		}
		
	}
	LocalFree(hMem);
}


void CSvcMgrDlg::OnServiceStop()
{
	// TODO: 在此添加命令处理程序代码
	if (pSvc->StopSvc(strSvcName))
	{
		UPDATETEXT(COL_STATUS, TEXT("已停止"));
	}
	
}





void CSvcMgrDlg::OnServiceSuspend()
{
	// TODO: 在此添加命令处理程序代码
	/*
	pSvc->SuspendSvc(strSvcName);
	td.pDlg = this;
	td.strSvcName = strSvcName;
	td.dwStatus  = SERVICE_PAUSED;
	HANDLE hThread = (HANDLE)AfxBeginThread(MonitorThread, (LPVOID)&td, 0, 0, NULL, NULL);
	CloseHandle(hThread);
	*/
	if (pSvc->SuspendSvc(strSvcName))
	{
		UPDATETEXT(COL_STATUS, TEXT("已暂停"))
	}

}

/*
LRESULT CSvcMgrDlg::OnRefresh(WPARAM wParam, LPARAM lParam)
{
	REFRESH
    return 0;
}
*/
/*
UINT  __cdecl CSvcMgrDlg::MonitorThread(LPVOID pParam)
{
    #define SVC  ptd->pDlg->pSvc
	SERVICE_STATUS_PROCESS  SvcSS;
	PTHREADDATA ptd  = (PTHREADDATA)pParam;
	SC_HANDLE   hSvc = OpenService(SVC->hSCM, ptd->strSvcName, SERVICE_QUERY_STATUS);
	SVC->QuerySvcInfo(hSvc, &SvcSS, sizeof(SvcSS));
	while (SvcSS.dwCurrentState == ptd->dwStatus)
	{
		break;
	}
	::SendMessage(ptd->pDlg->GetSafeHwnd(), WM_REFRESH, 0, 0);
	Sleep(2000);
	return 0;
}
*/

void CSvcMgrDlg::OnServiceResume()
{
	// TODO: 在此添加命令处理程序代码
	if (pSvc->ResumeSvc(strSvcName))
	{
		UPDATETEXT(COL_STATUS, TEXT("已启动"))
	}
}


void CSvcMgrDlg::OnServiceRestart()
{
	// TODO: 在此添加命令处理程序代码
	OnServiceStop();
	OnServiceStart();
	
}


void CSvcMgrDlg::OnBootAuto()
{
	// TODO: 在此添加命令处理程序代码
	if (pSvc->ChangeSvcStartType(strSvcName, SERVICE_AUTO_START))
	{
		UPDATETEXT(COL_TYPE,TEXT("自动"));
	}
	
}


void CSvcMgrDlg::OnBootManual()
{
	// TODO: 在此添加命令处理程序代码

	if (pSvc->ChangeSvcStartType(strSvcName, SERVICE_DEMAND_START))
	{
		UPDATETEXT(COL_TYPE,TEXT("手动"));
	}
}


void CSvcMgrDlg::OnBootDisable()
{
	// TODO: 在此添加命令处理程序代码
	if (pSvc->ChangeSvcStartType(strSvcName, SERVICE_DISABLED))
	{
		UPDATETEXT(COL_TYPE,TEXT("已禁用"));
	}
}


void CSvcMgrDlg::OnServiceUninstall()
{
	// TODO: 在此添加命令处理程序代码
	if (!pSvc->DeleteSvc(strSvcName))
	{
		MessageBox(TEXT("删除服务失败"), TEXT("SvcMgr"), MB_ICONERROR);
	}
	else
	{
	    m_SvcList.DeleteItem(GetSelectedItem());
		//REFRESH
	}
}


void CSvcMgrDlg::OnServiceInstall()
{
	// TODO: 在此添加命令处理程序代码
	CSvcAddDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (!pSvc->AddSvc(dlg.sii.strSvcName, dlg.sii.strDisplay, dlg.sii.strSvcPath, dlg.sii.dwServiceType,
			dlg.sii.dwStartType))
		{
			MessageBox(TEXT("服务创建失败!"), TEXT("SvcMgr"), MB_ICONERROR);
		}
		else
		{
			MessageBox(TEXT("服务成功!"), TEXT("SvcMgr"), MB_ICONINFORMATION);
			REFRESH
		}
	}
   
	
	
}


void CSvcMgrDlg::OnSearchGoogle()
{
	// TODO: 在此添加命令处理程序代码
	CString strGoogle = TEXT("http://www.google.com.hk/search?q=");
	ShellExecute(NULL, TEXT("open"), strGoogle + strSvcName, NULL, NULL, SW_SHOW);
}


void CSvcMgrDlg::OnSearchBing()
{
	// TODO: 在此添加命令处理程序代码
	CString strBing = TEXT("http://cn.bing.com/search?q=sd");
	ShellExecute(NULL, TEXT("open"), strBing + strSvcName, NULL, NULL, SW_SHOW);
}


void CSvcMgrDlg::OnSearchBaidu()
{
	// TODO: 在此添加命令处理程序代码
    CString strBaidu = TEXT("http://www.baidu.com/s?wd=");
	ShellExecute(NULL, TEXT("open"), strBaidu + strSvcName, NULL, NULL, SW_SHOW);
}
