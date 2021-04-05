//////////////////////////////////////////////////////////////////////////
//ģ������:Svc
//ģ������:��װ��һϵ��ϵͳ������صĺ���
//��д��  :�Ƴ�
//����������: 2011/7/1 23:55
//////////////////////////////////////////////////////////////////////////

#pragma once
#include <Windows.h>
#include <WinSvc.h>
#include <crtdbg.h>
#include <strsafe.h>
#include <Winver.h>


//��ʾ������Ϣ
#define  ErrMsg(lpszMsg)  MessageBox(NULL, lpszMsg, TEXT("SvcMgr"), MB_ICONERROR)
//��ʾ�������
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

	//��ȡ������Ϣ(��ִ���ļ�·��֮���)
	BOOL GetSvcInfo(LPCTSTR lpSvcName, LPQUERY_SERVICE_CONFIG lpServiceConfig, DWORD cbBufSize);
	BOOL StartSvc(LPCTSTR lpSvcName);
	BOOL StopSvc(LPCTSTR lpSvcName);

	//��ѯ������Ϣ(��������,��������֮���)
	BOOL QuerySvcInfo(SC_HANDLE hSvc, LPSERVICE_STATUS_PROCESS lpssStatus, DWORD dwBufSize);

	//��ȡ��˾��Ϣ, ��֧��XP  ����
	BOOL GetSvcCompanyEx(LPVOID lpResAddr, LPCTSTR lpSvcFullName, LPTSTR *lpCompany, DWORD dwBufSize);

	//��ȡ��Դ�ļ��Ĵ�С, ��֧��XP ����
	DWORD GetResSizeEx(LPCTSTR lpSvcName);
	DWORD GetResSize(LPCTSTR lpSvcName);

	//�����ı�����״̬(�� ���� ֹͣ ��ͣ)
	BOOL ModifySvcStatus(DWORD dwRight, DWORD dwControl,LPCTSTR lpSvcName, LPSERVICE_STATUS lpSvcSS);
	BOOL SuspendSvc(LPCTSTR lpSvcName);
	BOOL ResumeSvc(LPCTSTR lpSvcName);

	//�޸ķ�����������
	BOOL ChangeSvcStartType(LPCTSTR lpSvcName, DWORD dwStartType);
	BOOL DeleteSvc(LPCTSTR lpSvcName);
	BOOL GetSvcCompany(LPVOID lpResAddr, LPCTSTR lpSvcFullName, LPTSTR *lpCompany, DWORD dwBufSize);
	BOOL AddSvc(LPCTSTR lpSvcName, LPCTSTR lpDisplayName, LPCTSTR lpBinaryPathName, DWORD dwSvcType, DWORD dwStartType);
};

