/****************************************************************************
 |
 |   			TWAIN‘Î‰@½·¬Å°‰æ‘œæ‚è‚İ‚c‚k‚k
 |
 |			Ì§²Ù–¼ :		main.c
 |			ì¬Ò :		m.hama
 |			“ú•tF			96/09/03
 |
 |
 ***************************************************************************/
#include <windows.h>
#include "twain.h"         // for TW data type defines
#include "code.h"
#include "extern.h"
#include "data.h"

BOOL	WINAPI	DllMain(HINSTANCE hInstDLL, DWORD fdwReason,
						LPVOID lpvReserved)
{
	ghInst = hInstDLL;
	return TRUE;
}
