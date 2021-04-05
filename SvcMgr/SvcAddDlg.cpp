// SvcAddDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SvcMgr.h"
#include "SvcAddDlg.h"
#include "afxdialogex.h"


// CSvcAddDlg 对话框

IMPLEMENT_DYNAMIC(CSvcAddDlg, CDialogEx)

CSvcAddDlg::CSvcAddDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSvcAddDlg::IDD, pParent)
	, nSvcType(0)
	, nStartType(0)
	, m_strSvcName(_T(""))
	, m_StrSvcPath(_T(""))
	, m_strDisplay(_T(""))
{

}

CSvcAddDlg::~CSvcAddDlg()
{
}

void CSvcAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_SERVICE_PROCESS, nSvcType);
	DDX_Radio(pDX, IDC_RADIO_AUTO, nStartType);
	DDX_Text(pDX, IDC_EDIT_SVCNAME, m_strSvcName);
	DDX_Text(pDX, IDC_EDIT_SVCPATH, m_StrSvcPath);
	DDX_Text(pDX, IDC_EDIT_SVCDISPLAY, m_strDisplay);
}


BEGIN_MESSAGE_MAP(CSvcAddDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_INSTALL, &CSvcAddDlg::OnBnClickedButtonInstall)
	ON_BN_CLICKED(IDC_BUTTON_CANCLE, &CSvcAddDlg::OnBnClickedButtonCancle)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CSvcAddDlg::OnBnClickedButtonBrowse)
END_MESSAGE_MAP()


// CSvcAddDlg 消息处理程序


BOOL CSvcAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSvcAddDlg::OnBnClickedButtonInstall()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	if (m_strSvcName.IsEmpty())
	{
		MessageBox(TEXT("请输入服务名称"),TEXT("SvcMgr"), MB_ICONWARNING);
	}
	sii.strSvcName = m_strSvcName;

	if (m_StrSvcPath.IsEmpty())
	{
		MessageBox(TEXT("请输入服务所在路径"),TEXT("SvcMgr"), MB_ICONWARNING);
	}
	sii.strSvcPath = m_StrSvcPath;

	switch(nSvcType)
	{
	case 0:
		sii.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
		break;
	case 1:
		sii.dwServiceType = SERVICE_FILE_SYSTEM_DRIVER;
		break;
	case 2:
		sii.dwServiceType = SERVICE_KERNEL_DRIVER;
		break;
	}

	switch(nSvcType)
	{
	case 0:
		sii.dwStartType = SERVICE_AUTO_START;
		break;
	case 1:
		sii.dwStartType = SERVICE_DEMAND_START;
		break;
	case 3:
		sii.dwStartType = SERVICE_DISABLED;
		break;
	}

	sii.strDisplay = m_strDisplay;
	if (!sii.strSvcName.IsEmpty() && !sii.strSvcPath.IsEmpty())
	{
		OnOK();
	}
}


void CSvcAddDlg::OnBnClickedButtonCancle()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


void CSvcAddDlg::OnBnClickedButtonBrowse()
{
	// TODO: 在此添加控件通知处理程序代码

	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrFilter = TEXT("EXE Files(*.exe)\0*.exe\0SYS Files(*.sys)\0*.sys\0All Files(*.*)\0*.*\0\0");
	if (dlg.DoModal())
	{
		m_StrSvcPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}
