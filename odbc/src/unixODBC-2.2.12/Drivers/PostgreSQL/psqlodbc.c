
/* Module:          psqlodbc.c
 *
 * Description:     This module contains the main entry point (DllMain) for the library.
 *                  It also contains functions to get and set global variables for the
 *                  driver in the registry.
 *
 * Classes:         n/a
 *
 * API functions:   none
 *
 * Comments:        See "notice.txt" for copyright and license information.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "psqlodbc.h"
#include "dlg_specific.h"

#ifndef WIN32

#ifdef UNIXODBC
#include <sql.h>
#include <odbcinst.h>
#else
#include "iodbc.h"
#include "isql.h"
#include "isqlext.h"
#endif

#else
#include <winsock.h>
#include <windows.h>
#include <sql.h>
#include <odbcinst.h>
#endif

GLOBAL_VALUES globals;

/*
BOOL _init(void);
BOOL _fini(void);
*/
RETCODE SQL_API SQLDummyOrdinal(void);

#ifdef WIN32
HINSTANCE NEAR s_hModule;               /* Saved module handle. */

/*	This is where the Driver Manager attaches to this Driver */
BOOL WINAPI DllMain(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
WORD wVersionRequested;
WSADATA wsaData;

	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		s_hModule = hInst;				/* Save for dialog boxes */

		/*	Load the WinSock Library */
		wVersionRequested = MAKEWORD(1, 1);

		if ( WSAStartup(wVersionRequested, &wsaData))
			return FALSE;

		/*	Verify that this is the minimum version of WinSock */
		if ( LOBYTE( wsaData.wVersion ) != 1 ||
			HIBYTE( wsaData.wVersion ) != 1 ) {

			WSACleanup();
			return FALSE;
		}

		getGlobalDefaults(DBMS_NAME, ODBCINST_INI, FALSE);
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_PROCESS_DETACH:

		WSACleanup();

		return TRUE;

	case DLL_THREAD_DETACH:
		break;

	default:
		break;
	}

	return TRUE;

	UNREFERENCED_PARAMETER(lpReserved);
}

#else	/* WIN32 */

#ifndef TRUE
#define TRUE	(BOOL)1
#endif
#ifndef FALSE
#define FALSE	(BOOL)0
#endif

#ifdef DONT_DO_IT

/* These two functions do shared library initialziation on UNIX, well at least
 * on Linux. I don't know about other systems.
 * Should work on SUN - PAH
 */
BOOL
_init(void)
{
	getGlobalDefaults(DBMS_NAME, ODBCINST_INI, FALSE);
	return TRUE;
}

BOOL
_fini(void)
{
	return TRUE;
}

#endif

#endif

/*	This function is used to cause the Driver Manager to
	call functions by number rather than name, which is faster.
	The ordinal value of this function must be 199 to have the
	Driver Manager do this.  Also, the ordinal values of the
	functions must match the value of fFunction in SQLGetFunctions()
*/
RETCODE SQL_API SQLDummyOrdinal(void)
{
	return SQL_SUCCESS;
}

