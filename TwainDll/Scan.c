/****************************************************************************
 |
 |   			TWAIN対応　ｽｷｬﾅｰ画像取り込みＤＬＬ
 |
 |			ﾌｧｲﾙ名 :		scan.c
 |			作成者 :		m.hama
 |			日付：			96/09/03
 |
 |
 ***************************************************************************/
#include <windows.h>         // Req. for twain.h type defs and ...
#include <commdlg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "twain.h"
#include "code.h"
#include "extern.h"
#include "data.h"
#include "scan.h"

static	VOID	InitData(HWND);

/****************************************************************************
 |
 |   		ｽｷｬﾅｰ画像取り込み
 |
 |		int	FAR PASCAL _export ScanDib(HWND hWnd, LPSTR DibFileName);
 |
 |			HWND 	hWnd;			呼び元ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ
 |			LPSTR	DibFileName;	DIBﾌｧｲﾙ名
 |
 ***************************************************************************/
int	APIENTRY ScanDib(HWND hWnd, LPSTR DibFileName)
{
	/* ｱﾌﾟﾘｹｰｼｮﾝID編集 */
	InitData(hWnd);

	lstrcpy((LPSTR)Filename, DibFileName);

	EnableWindow(hWnd, FALSE);

	mode = 0;
  	TWAcquire(hWnd, TRUE);

	EnableWindow(hWnd, TRUE);
	SetFocus(hWnd);

	return(WriteDibFlag);
}

HANDLE	APIENTRY ScanBmp(HWND hWnd, SCAN_IMAGE_INFO *pImageInfo)
{
	/* ｱﾌﾟﾘｹｰｼｮﾝID編集 */
	InitData(hWnd);

	EnableWindow(hWnd, FALSE);

	mode = 1;
	hBitmap = NULL;

  	TWAcquire(hWnd, TRUE);

	EnableWindow(hWnd, TRUE);
	SetFocus(hWnd);

	if (hBitmap != NULL) {
		pImageInfo->XResolution = twImageInfo.XResolution.Whole;
		pImageInfo->YResolution = twImageInfo.YResolution.Whole;
		pImageInfo->ImageWidth = twImageInfo.ImageWidth;
		pImageInfo->ImageLength = twImageInfo.ImageLength;
		pImageInfo->BitsPerPixel = twImageInfo.BitsPerPixel;
	}

	return(hBitmap);
}

void APIENTRY ScanBmpContinue(HWND hWnd)
{
	/* ｱﾌﾟﾘｹｰｼｮﾝID編集 */
	InitData(hWnd);

	EnableWindow(hWnd, FALSE);

	mode = 2;
	hBitmap = NULL;
  	TWAcquire(hWnd, TRUE);

	EnableWindow(hWnd, TRUE);
	//SetFocus(hWnd);
}

BOOL APIENTRY SelectTwainDriver(HWND hWnd)
{
	InitData(hWnd);

	if (!TWOpenDSM())
		return FALSE;

	TWSelectDS();
	TWCloseDSM();
	return TRUE;
}

BOOL APIENTRY GetDefaultDriver(HWND hWnd, TW_IDENTITY *dsID)
{
	BOOL rc;

	InitData(hWnd);

	if (!TWOpenDSM())
		return FALSE;

	rc = TWGetDefault(dsID);
	TWCloseDSM();

	return rc;
}

static	VOID	InitData(HWND hWnd)
{
    TW_IDENTITY AppIdentity;

	/* ｱﾌﾟﾘｹｰｼｮﾝID編集 */
    AppIdentity.Id = 0; // init to 0, but Source Manager will assign real value
    AppIdentity.Version.MajorNum = 1;
    AppIdentity.Version.MinorNum = 5;
    AppIdentity.Version.Language = TWLG_USA;
    AppIdentity.Version.Country  = TWCY_USA;
    lstrcpy (AppIdentity.Version.Info,  "YMEC TWAIN Interface DLL 1.1");

    AppIdentity.ProtocolMajor =    TWON_PROTOCOLMAJOR;
    AppIdentity.ProtocolMinor =    TWON_PROTOCOLMINOR;
    AppIdentity.SupportedGroups =  DG_IMAGE | DG_CONTROL;
    lstrcpy (AppIdentity.Manufacturer,  "YMEC");
    lstrcpy (AppIdentity.ProductFamily, "M.HAMA/S.AIZAWA");
    lstrcpy (AppIdentity.ProductName,   "SCAN32.DLL");

    // pass app particulars to glue code
    TWInitialize (&AppIdentity, hWnd);
	SetMessageLevel(ML_NONE);

	dsID.Id = 0;
	dsID.ProductName[0] = 0;

	WriteDibFlag = FALSE;
}




