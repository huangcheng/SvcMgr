//////////////////////////////////////////////////////////////////////////
//模块名称:Svc
//模块描述:封装了一系列系统服务相关的函数
//编写者  :黄诚
//最后跟新日期: 2011/7/1 23:55
//////////////////////////////////////////////////////////////////////////

#pragma once
#include <Windows.h>
#include <WinSvc.h>
#include <crtdbg.h>
#include <strsafe.h>
#include <Winver.h>


//显示错误信息
#define  ErrMsg(lpszMsg)  MessageBox(NULL, lpszMsg, TEXT("SvcMgr"), MB_ICONERROR)
//显示错误代码
#define  SucMsg(lpszMsg)  MessageBox(NULL, lpszMsg, TEXT("SvcMgr"), MB_ICONINFORMATION)
#define  ShowErr          OutPutErrorCode(GetLastError())

#define  OpenSvc(dwRight)          OpenService(hSCM, lpSvcName, dwRight)
#define  CloseSvc                CloseServiceHandle(hSvc)
class CSvc
{
public:
	CSvc(void);
	~CSvc(void);
	DWORD EnumServices(LPENUM_SERVICE_STATUS_PROCESS pSvcStatus, DWORD dwSize);

private:
	DWORD OutPutErrorCode(DWORD dwErrorID);

private:
	SC_HANDLE hSCM;
	SC_HANDLE hSvc;
	HLOCAL    hMem;

public:
	DWORD dwServicesReturned;
	DWORD dwSizeNeeded;

	//获取服务信息(可执行文件路径之类的)
	BOOL GetSvcInfo(LPCTSTR lpSvcName, LPQUERY_SERVICE_CONFIG lpServiceConfig, DWORD cbBufSize);
	BOOL StartSvc(LPCTSTR lpSvcName);
	BOOL StopSvc(LPCTSTR lpSvcName);

	//查询服务信息(启动类型,服务类型之类的)
	BOOL QuerySvcInfo(SC_HANDLE hSvc, LPSERVICE_STATUS_PROCESS lpssStatus, DWORD dwBufSize);

	//获取公司信息, 不支持XP  废弃
	BOOL GetSvcCompanyEx(LPVOID lpResAddr, LPCTSTR lpSvcFullName, LPTSTR *lpCompany, DWORD dwBufSize);

	//获取资源文件的大小, 不支持XP 废弃
	DWORD GetResSizeEx(LPCTSTR lpSvcName);
	DWORD GetResSize(LPCTSTR lpSvcName);

	//用来改变服务的状态(如 启动 停止 暂停)
	BOOL ModifySvcStatus(DWORD dwRight, DWORD dwControl,LPCTSTR lpSvcName, LPSERVICE_STATUS lpSvcSS);
	BOOL SuspendSvc(LPCTSTR lpSvcName);
	BOOL ResumeSvc(LPCTSTR lpSvcName);

	//修改服务启动类型
	BOOL ChangeSvcStartType(LPCTSTR lpSvcName, DWORD dwStartType);
	BOOL DeleteSvc(LPCTSTR lpSvcName);
	BOOL GetSvcCompany(LPVOID lpResAddr, LPCTSTR lpSvcFullName, LPTSTR *lpCompany, DWORD dwBufSize);
	BOOL AddSvc(LPCTSTR lpSvcName, LPCTSTR lpDisplayName, LPCTSTR lpBinaryPathName, DWORD dwSvcType, DWORD dwStartType);
};

