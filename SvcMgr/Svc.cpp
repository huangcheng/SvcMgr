#include "StdAfx.h"
#include "Svc.h"



CSvc::CSvc(void)
{
	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		ErrMsg(TEXT("Unable to open Local SCManager"));
		CloseServiceHandle(hSCM);
	}
	
}


CSvc::~CSvc(void)
{
	hSCM == NULL? CloseServiceHandle(hSCM):NULL ;
	hMem == NULL? LocalFree(hMem):NULL;
}


DWORD CSvc::EnumServices(LPENUM_SERVICE_STATUS_PROCESS pSvcStatus, DWORD dwSize)
{
	dwSizeNeeded = 0;
	dwServicesReturned = 0;
	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO ,SERVICE_WIN32,
		SERVICE_STATE_ALL,(LPBYTE)pSvcStatus, dwSize,
		&dwSizeNeeded,&dwServicesReturned, NULL, NULL);

	if (GetLastError()== ERROR_MORE_DATA)
	{
		return dwSizeNeeded;
	}
	

	return 0;

}

BOOL CSvc::GetSvcInfo(LPCTSTR lpSvcName, LPQUERY_SERVICE_CONFIG lpServiceConfig, DWORD cbBufSize)
{
	/*
	TCHAR szTemp[MAX_PATH] = {0};
	TCHAR szReg[] = TEXT("SYSTEM\\CurrentControlSet\\services\\");
	DWORD dwType = REG_SZ;
	StringCchCopy(szTemp,_countof(szTemp),szReg);
	StringCchCat(szTemp,_countof(szTemp),lpSvcName);
	
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,szTemp, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
	{
		ErrMsg(TEXT("´ò¿ª×¢²á±í!"));
		return FALSE;
	}

	if (RegQueryValueEx(hKey, TEXT("ImagePath"), NULL, &dwType, (LPBYTE)lpImgPath, &cbBufSize) != ERROR_SUCCESS)
	{
		ErrMsg(TEXT("Unable to read registry value!"));
		RegCloseKey(hKey);
	}
	
	RegCloseKey(hKey);
	*/

	hSvc = OpenService(hSCM, lpSvcName, SERVICE_QUERY_CONFIG);
	if (hSvc != NULL)
	{
		QueryServiceConfig(hSvc, lpServiceConfig, cbBufSize, &dwSizeNeeded);
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			CloseServiceHandle(hSvc);
			return FALSE;
		}
	}
	CloseServiceHandle(hSvc);
	return TRUE;
}

DWORD CSvc::OutPutErrorCode(DWORD dwErrorID)
{
	TCHAR *lpErr = new TCHAR[50];
	StringCchPrintf(lpErr, 50, TEXT("Error Code is %d"),dwErrorID);
	ErrMsg(lpErr);
	delete lpErr;
	return dwErrorID;
}





BOOL CSvc::StartSvc(LPCTSTR lpSvcName)
{
	hSvc = OpenSvc(SERVICE_START | SERVICE_QUERY_STATUS);
	if (hSvc != NULL)
	{
		SERVICE_STATUS_PROCESS ssStatus;
		if (QuerySvcInfo(hSvc, &ssStatus, sizeof(SERVICE_STATUS_PROCESS)))
		{
			if (ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
			{
				return TRUE;
			}
			else
			{
				if (StartService(hSvc, 0, NULL))
				{
					return TRUE;
				}
			}
		}
		

	}
	CloseSvc;
	return FALSE;
}


BOOL CSvc::QuerySvcInfo(SC_HANDLE hSvc, LPSERVICE_STATUS_PROCESS lpssStatus, DWORD dwBufSize)
{
	if (!QueryServiceStatusEx(hSvc,  SC_STATUS_PROCESS_INFO, (LPBYTE)lpssStatus, dwBufSize, &dwSizeNeeded))
	{
		ShowErr;
		return FALSE;
	}
    
	return TRUE;
}


BOOL CSvc::GetSvcCompanyEx(LPVOID lpResAddr, LPCTSTR lpSvcFullName, LPTSTR *lpCompany, DWORD dwBufSize)
{
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate;
    
	TCHAR szSubBlock[50];
	UINT uSizeRet;
	

	DWORD dwResSize = GetResSize(lpSvcFullName);
	if (dwResSize != 0)
	{
		if (lpResAddr != NULL)
		{
			if (GetFileVersionInfoEx(FILE_VER_GET_LOCALISED, lpSvcFullName, 0, dwResSize, lpResAddr))
			{
				
				if (!VerQueryValue(lpResAddr, TEXT("\\VarFileInfo\\Translation"),(LPVOID*)&lpTranslate, &uSizeRet))
				{
					ShowErr;
					return FALSE;
				}
				StringCchPrintf(szSubBlock, 50,
					TEXT("\\StringFileInfo\\%04x%04x\\CompanyName"),
					lpTranslate[0].wLanguage,
					lpTranslate[0].wCodePage);
				VerQueryValue(lpResAddr, 
					szSubBlock, 
					(LPVOID*)&lpCompany, 
					(PUINT)&dwBufSize);
			}
		}
	}
	return TRUE;
}


DWORD CSvc::GetResSizeEx(LPCTSTR lpSvcName)
{
	return GetFileVersionInfoSizeEx(FILE_VER_GET_LOCALISED, lpSvcName, NULL);
}

DWORD CSvc::GetResSize(LPCTSTR lpSvcName)
{
	return GetFileVersionInfoSize(lpSvcName, NULL);
}

BOOL CSvc::StopSvc(LPCTSTR lpSvcName)
{
	SERVICE_STATUS ss;
	ModifySvcStatus(SERVICE_STOP | SERVICE_QUERY_STATUS, SERVICE_CONTROL_STOP, lpSvcName, &ss);
	if (ss.dwCurrentState == SERVICE_STOPPED || ss.dwCurrentState == SERVICE_STOP_PENDING)
	{
		return TRUE;
	}
	
	return FALSE;
}

BOOL CSvc::ModifySvcStatus(DWORD dwRight, DWORD dwControl,LPCTSTR lpSvcName,LPSERVICE_STATUS lpSvcSS)
{
	hSvc = OpenSvc(dwRight);
	if (hSvc != NULL)
	{
		SERVICE_STATUS_PROCESS ssStatus;
		if (QuerySvcInfo(hSvc, &ssStatus, sizeof(SERVICE_STATUS_PROCESS)))
		{
			if (ControlService(hSvc, dwControl, lpSvcSS))
			{
				return TRUE;
			}
		}
	}
	CloseSvc;
	return FALSE;
}

BOOL CSvc::SuspendSvc(LPCTSTR lpSvcName)
{
	SERVICE_STATUS ss;
	ModifySvcStatus( SERVICE_PAUSE_CONTINUE | SERVICE_QUERY_STATUS, SERVICE_CONTROL_PAUSE, lpSvcName, &ss);
	if (ss.dwCurrentState == SERVICE_PAUSED || ss.dwCurrentState == SERVICE_PAUSE_PENDING)
	{
		return TRUE;
	}

	return FALSE;
}


BOOL CSvc::ResumeSvc(LPCTSTR lpSvcName)
{
	SERVICE_STATUS ss;
	ModifySvcStatus( SERVICE_PAUSE_CONTINUE | SERVICE_QUERY_STATUS, SERVICE_CONTROL_CONTINUE, lpSvcName, &ss);
	if (ss.dwCurrentState == SERVICE_RUNNING || ss.dwCurrentState == SERVICE_CONTINUE_PENDING)
	{
		return TRUE;
	}

	return FALSE;
}


BOOL CSvc::ChangeSvcStartType(LPCTSTR lpSvcName, DWORD dwStartType)
{
    hSvc = OpenSvc(SERVICE_QUERY_STATUS | SERVICE_CHANGE_CONFIG);
	SERVICE_STATUS_PROCESS SvcSS;
	if (hSvc != NULL)
	{
		if (QuerySvcInfo(hSvc, &SvcSS, sizeof(SERVICE_STATUS_PROCESS)))
		{
			if (ChangeServiceConfig(hSvc, SvcSS.dwServiceType, dwStartType, SERVICE_ERROR_NORMAL, NULL, NULL, 
				                    NULL, NULL, NULL, NULL, NULL))
			{
				return TRUE;
			}
		}
	}
	CloseSvc;
	return FALSE;
}


BOOL CSvc::DeleteSvc(LPCTSTR lpSvcName)
{
	hSvc = OpenSvc(DELETE);
	if (hSvc != NULL)
	{
		if (!DeleteService(hSvc))
		{
			//ShowErr;
			return FALSE;
		}
	}
	CloseSvc;
	return TRUE;
}


BOOL CSvc::GetSvcCompany(LPVOID lpResAddr, LPCTSTR lpSvcFullName, LPTSTR *lpCompany, DWORD dwBufSize)
{
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate;

	TCHAR szSubBlock[50];
	UINT uSizeRet;


	DWORD dwResSize = GetResSize(lpSvcFullName);
	if (dwResSize != 0)
	{
		if (lpResAddr != NULL)
		{
			if (GetFileVersionInfo(lpSvcFullName, 0, dwResSize, lpResAddr))
			{

				if (!VerQueryValue(lpResAddr, TEXT("\\VarFileInfo\\Translation"),(LPVOID*)&lpTranslate, &uSizeRet))
				{
					ShowErr;
					return FALSE;
				}
				StringCchPrintf(szSubBlock, 50,
					TEXT("\\StringFileInfo\\%04x%04x\\CompanyName"),
					lpTranslate[0].wLanguage,
					lpTranslate[0].wCodePage);
				VerQueryValue(lpResAddr, 
					szSubBlock, 
					(LPVOID*)lpCompany, 
					(PUINT)&dwBufSize);
			}
		}
	}
	return TRUE;
}


BOOL CSvc::AddSvc(LPCTSTR lpSvcName, LPCTSTR lpDisplayName, LPCTSTR lpBinaryPathName, DWORD dwSvcType, DWORD dwStartType)
{
	if (CreateService(hSCM, lpSvcName, lpDisplayName, SERVICE_ALL_ACCESS, dwSvcType,
		              dwStartType, SERVICE_ERROR_NORMAL, lpBinaryPathName, NULL, NULL, 
		              NULL, NULL, NULL) == NULL)
	{
		return FALSE;
	}
	return TRUE;
}
