#ifndef _STRUCT_H_
#define _STRUCT_H_

typedef struct _SERVICE_INSTALL_INFO
{
	DWORD dwServiceType;
	DWORD dwStartType;
	CString strSvcName;
	CString strSvcPath;
	CString strDisplay;
}SERVICEINSTALLINFO,*PSERVICEINSTALLINFO;

#endif