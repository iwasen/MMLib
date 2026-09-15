/****************************************************************************
 |
 |   			TWAINëŒâûÅ@Ω∑¨≈∞âÊëúéÊÇËçûÇ›ÇcÇkÇk
 |
 |			Ãß≤Ÿñº :		data.c
 |			çÏê¨é“ :		m.hama
 |			ì˙ïtÅF			96/09/03
 |
 |
 ***************************************************************************/
#include <windows.h>    // Note: twain.h also REQUIRES windows defs
#include "twain.h"         // for TW data type defines

TW_IDENTITY appID, dsID;          // storage for App and DS (Source) states
DSMENTRYPROC lpDSM_Entry;         // entry point to the SM
char		Filename[256];
HINSTANCE	ghInst ;
TW_STR255	IniFile;
HANDLE		hStruct;
LPSTR		pStruct;
HANDLE   	hInst;             // current instance
HCURSOR		hWait;
HCURSOR		hReady;
BOOL	WriteDibFlag;
HANDLE hBitmap;
int	mode;
TW_IMAGEINFO twImageInfo;
