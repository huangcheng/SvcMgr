
// SvcMgrDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SvcMgr.h"
#include "SvcMgrDlg.h"
#include "afxdialogex.h"

//#include "CmnHeader.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CSvcMgrDlg �Ի���




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


// CSvcMgrDlg ��Ϣ�������

BOOL CSvcMgrDlg::OnInitDialog()
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

	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSvcMgrDlg::OnPaint()
{
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSvcMgrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSvcMgrDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
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
		::ErrMsg(TEXT("�����ڴ�ʧ��!"));
		return FALSE;
	}

	pResult= (LPENUM_SERVICE_STATUS_PROCESS)hMem;
	pSvc->EnumServices(pResult,dwRequiedSize);

	strResult.Format(TEXT("Services Manager  ��%d������"),pSvc->dwServicesReturned);
	SetWindowText(strResult);

	for (UINT i = 0; i<pSvc->dwServicesReturned; ++i)
	{

		nItem = m_SvcList.InsertItem(COL_SVC,pResult[i].lpServiceName);

		//m_SvcList.SetItemText(nItem,1,pResult[i].lpDisplayName);

		switch(pResult[i].ServiceStatusProcess.dwCurrentState)
		{
		case SERVICE_STOPPED:
			strStatus = TEXT("��ֹͣ");
			break;

		case SERVICE_RUNNING:
			strStatus = TEXT("������");
			break;

		case SERVICE_PAUSED:
			strStatus = TEXT("����ͣ");
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

			
			//�˶�������������ַ����е�����
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
				strType = TEXT("�Զ�");
				break;
			case SERVICE_DISABLED:
				strType = TEXT("�ѽ���");
				break;
			case SERVICE_DEMAND_START:
				strType = TEXT("�ֶ�");
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

	// TODO: �ڴ˴������Ϣ����������
	AdjustSize(*pRect);
}





void CSvcMgrDlg::OnNMRClickListSvc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���������������
	if (pSvc->StartSvc(strSvcName))
	{
		UPDATETEXT(COL_STATUS, TEXT("������"))
	}
	
}


BOOL CSvcMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F5)
	{
		REFRESH
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSvcMgrDlg::AddListHeaders(void)
{
	m_SvcList.InsertColumn(COL_SVC,TEXT("������"),LVCFMT_CENTER,150);
	m_SvcList.InsertColumn(COL_STATUS,TEXT("״̬"),LVCFMT_CENTER,50);
	m_SvcList.InsertColumn(COL_TYPE,TEXT("����"),LVCFMT_CENTER,50);
	m_SvcList.InsertColumn(COL_NAME,TEXT("��ʾ����"),LVCFMT_LEFT,150);
	m_SvcList.InsertColumn(COL_PATH,TEXT("ӳ��·��"),LVCFMT_LEFT,400);
	m_SvcList.InsertColumn(COL_COMPANY,TEXT("��˾"),LVCFMT_LEFT,150);
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
	
	// TODO: �ڴ˴������Ϣ����������
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
	// TODO: �ڴ���������������
	if (pSvc->StopSvc(strSvcName))
	{
		UPDATETEXT(COL_STATUS, TEXT("��ֹͣ"));
	}
	
}





void CSvcMgrDlg::OnServiceSuspend()
{
	// TODO: �ڴ���������������
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
		UPDATETEXT(COL_STATUS, TEXT("����ͣ"))
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
	// TODO: �ڴ���������������
	if (pSvc->ResumeSvc(strSvcName))
	{
		UPDATETEXT(COL_STATUS, TEXT("������"))
	}
}


void CSvcMgrDlg::OnServiceRestart()
{
	// TODO: �ڴ���������������
	OnServiceStop();
	OnServiceStart();
	
}


void CSvcMgrDlg::OnBootAuto()
{
	// TODO: �ڴ���������������
	if (pSvc->ChangeSvcStartType(strSvcName, SERVICE_AUTO_START))
	{
		UPDATETEXT(COL_TYPE,TEXT("�Զ�"));
	}
	
}


void CSvcMgrDlg::OnBootManual()
{
	// TODO: �ڴ���������������

	if (pSvc->ChangeSvcStartType(strSvcName, SERVICE_DEMAND_START))
	{
		UPDATETEXT(COL_TYPE,TEXT("�ֶ�"));
	}
}


void CSvcMgrDlg::OnBootDisable()
{
	// TODO: �ڴ���������������
	if (pSvc->ChangeSvcStartType(strSvcName, SERVICE_DISABLED))
	{
		UPDATETEXT(COL_TYPE,TEXT("�ѽ���"));
	}
}


void CSvcMgrDlg::OnServiceUninstall()
{
	// TODO: �ڴ���������������
	if (!pSvc->DeleteSvc(strSvcName))
	{
		MessageBox(TEXT("ɾ������ʧ��"), TEXT("SvcMgr"), MB_ICONERROR);
	}
	else
	{
	    m_SvcList.DeleteItem(GetSelectedItem());
		//REFRESH
	}
}


void CSvcMgrDlg::OnServiceInstall()
{
	// TODO: �ڴ���������������
	CSvcAddDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (!pSvc->AddSvc(dlg.sii.strSvcName, dlg.sii.strDisplay, dlg.sii.strSvcPath, dlg.sii.dwServiceType,
			dlg.sii.dwStartType))
		{
			MessageBox(TEXT("���񴴽�ʧ��!"), TEXT("SvcMgr"), MB_ICONERROR);
		}
		else
		{
			MessageBox(TEXT("����ɹ�!"), TEXT("SvcMgr"), MB_ICONINFORMATION);
			REFRESH
		}
	}
   
	
	
}


void CSvcMgrDlg::OnSearchGoogle()
{
	// TODO: �ڴ���������������
	CString strGoogle = TEXT("http://www.google.com.hk/search?q=");
	ShellExecute(NULL, TEXT("open"), strGoogle + strSvcName, NULL, NULL, SW_SHOW);
}


void CSvcMgrDlg::OnSearchBing()
{
	// TODO: �ڴ���������������
	CString strBing = TEXT("http://cn.bing.com/search?q=sd");
	ShellExecute(NULL, TEXT("open"), strBing + strSvcName, NULL, NULL, SW_SHOW);
}


void CSvcMgrDlg::OnSearchBaidu()
{
	// TODO: �ڴ���������������
    CString strBaidu = TEXT("http://www.baidu.com/s?wd=");
	ShellExecute(NULL, TEXT("open"), strBaidu + strSvcName, NULL, NULL, SW_SHOW);
}
