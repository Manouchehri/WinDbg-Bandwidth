/* bandwidth.h
 *
 * Brandon Falk <bfalk@gamozolabs.com>
 * (C) Gamozo Labs 2012
 *
 * Free code :D
 * Use at your own risk
 */

#ifndef _BANDWIDTH_H_
#define _BANDWIDTH_H_

#define KDEXT_64BIT

#pragma warning(push, 0)
#include <windows.h>
#include <wdbgexts.h>
#pragma warning(pop)

#define BUF_SIZE 8192

void
WinDbgExtensionDllInit(PWINDBG_EXTENSION_APIS lpExtensionApis,
		USHORT MajorVersion, USHORT MinorVersion);

LPEXT_API_VERSION
ExtensionApiVersion(void);

DECLARE_API(bandwidth);

#endif

