/* bandwidth.c
 *
 * Brandon Falk <bfalk@gamozolabs.com>
 * (C) Gamozo Labs 2012
 *
 * Free code :D
 * Use at your own risk
 */

#include "bandwidth.h"

/* Globals of doom */
EXT_API_VERSION       apiver = { 5, 5, EXT_API_VERSION_NUMBER64, 0 };
WINDBG_EXTENSION_APIS ExtensionApis;

/* WinDbgExtensionDllInit()
 *
 * Summary:
 *
 * This function is called by WinDbg to give us our API function pointers,
 * makes us all warm and fuzzy inside.
 */
void
WinDbgExtensionDllInit(PWINDBG_EXTENSION_APIS lpExtensionApis,
		USHORT MajorVersion, USHORT MinorVersion)
{
	UNREFERENCED_PARAMETER(MajorVersion);
	UNREFERENCED_PARAMETER(MinorVersion);

	ExtensionApis = *lpExtensionApis;

	return;
}

/* ExtensionApiVersion()
 *
 * Summary:
 *
 * WinDbg calls this to ask us version info. Be warned, lots of code here!
 */
LPEXT_API_VERSION
ExtensionApiVersion(void)
{
	return &apiver;
}

/* bandwidth()
 *
 * Summary:
 *
 * This is the extention that WinDbg calls when you do !bandwidth. It allocates
 * a buffer, reads virtual memory, then reads physical memory and reports the
 * transfer rates to you for a low, low price!
 */
DECLARE_API(bandwidth)
{
	char   *buf;
	double  elapsed, txd;

	ULONG     read;
	ULONG_PTR offs, tx, tc, waittime;

	UNREFERENCED_PARAMETER(dwProcessor);
	UNREFERENCED_PARAMETER(hCurrentThread);
	UNREFERENCED_PARAMETER(hCurrentProcess);

	/* Read in the time to wait, and do some sanity checks just incase
	 * someone made a typo.
	 */
	waittime = GetExpression(args);
	if(waittime > 10000)
		waittime = 10000;
	else if(waittime == 0)
		waittime = 5000;

	/* Allocate buffer */
	buf = malloc(BUF_SIZE);
	if(!buf){
		dprintf("malloc() error\n");
		return;
	}

	/* Read for 5 seconds */
	tc   = GetTickCount64();
	tx   = 0;
	offs = dwCurrentPc;
	for( ; ; ){
		ReadMemory(offs, buf, BUF_SIZE, &read);
		offs += read;
		tx   += read;

		if((GetTickCount64() - tc) > waittime)
			break;
	}

	/* Calculate transfer rate */
	elapsed = ((double)(GetTickCount64() - tc)) / 1000;
	txd     = ((double)tx) / 1024;
	dprintf("Read Virt: %20.5f KB/s\n", txd / elapsed);

	/* Read for 5 seconds */
	tc   = GetTickCount64();
	offs = 0;
	for( ; ; ){
		ReadPhysical(offs, buf, BUF_SIZE, &read);
		offs += read;

		if((GetTickCount64() - tc) > waittime)
			break;
	}

	/* Calculate transfer rate */
	elapsed = ((double)(GetTickCount64() - tc)) / 1000;
	txd     = ((double)offs) / 1024;
	dprintf("Read Phys: %20.5f KB/s\n", txd / elapsed);

	free(buf);
	return;
}

