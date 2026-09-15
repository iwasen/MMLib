/****************************************************************************
 |
 |   			TWAIN対応　ｽｷｬﾅｰ画像取り込みＤＬＬ
 |
 |			ﾌｧｲﾙ名 :		tool.c
 |			作成者 :		m.hama
 |			日付：			96/09/03
 |
 |
 ***************************************************************************/

#include <windows.h>         // Req. for twain.h type defs and ...
#include <stdio.h>
#include <string.h>
#include "twain.h"
#include "code.h"
#include "extern.h"
#include "data.h"

#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')
#define BYTES_PER_READ  32767

#define CAP_AUTOFINAL CAP_CUSTOMBASE+1


/*
----------------------------------------------------------------------
							V a r i b l e s
----------------------------------------------------------------------
*/

static	int	UseManualFeedBOOL = FALSE;
static	int	AutoFeedBOOL = FALSE;
static int MessageLevelVAR = ERROR;						//To display what is happening

/*
----------------------------------------------------------------------
							F u n c t i o n s
----------------------------------------------------------------------
*/
void GetItem (char * Text, TW_UINT16 datatype, LPSTR pData);
static	WORD DibNumColors (VOID FAR *);
static	DWORD PASCAL lwrite (int, VOID FAR *, DWORD);

/*=================================================================
 |
 |		ＤＩＢファイルに書き込み
 |
 |		VOID	WriteDibFile(HBITMAP hDib, LPSTR filename);
 |
 *=================================================================*/
VOID	WriteDibFile(HBITMAP hDib, LPSTR filename)
{
    BITMAPFILEHEADER	hdr;
    LPBITMAPINFOHEADER  lpDib, lpBi;
    LPSTR               lpBits;
    DWORD               dwColorTableSize;
	HFILE	hFile;

	/* BITMAPINFOHEADER　ﾛｯｸ */
	if (hDib == NULL) return;
    lpDib = (LPBITMAPINFOHEADER) GlobalLock(hDib);
    if (lpDib == NULL)
    	return;

	/* DIBﾌｧｲﾙ　ｸﾘｴｲﾄ */
	if (! (hFile = _lcreat((LPSTR)filename, 0))){
		GlobalUnlock(hDib);
		return;
	}

    lpBi = lpDib;

    // Calculate the color table size, then point past the BITMAPINFOHEADER
    // and color table, to the byte array of bitmap bits.
    dwColorTableSize = (DWORD)(DibNumColors (lpDib) * sizeof(RGBQUAD));
    lpBits = (LPSTR)lpDib + lpBi->biSize + dwColorTableSize;

    /* ファイルヘッダーの各メンバに値を設定 */
    hdr.bfType		= DIB_HEADER_MARKER;
    hdr.bfSize		= (int)GlobalSize (hDib) + sizeof (BITMAPFILEHEADER);
    hdr.bfReserved1     = 0;
    hdr.bfReserved2     = 0;
    hdr.bfOffBits       = (DWORD)sizeof(BITMAPFILEHEADER) + lpBi->biSize +
                          dwColorTableSize;

    /* ファイルヘッダーを書き込む */
    if (!_lwrite (hFile, (LPSTR)&hdr, sizeof (BITMAPFILEHEADER))){
		GlobalUnlock (hDib);
		_lclose (hFile);
		return;
	}
    /* DIBヘッダーとビットを書き込む */
	lwrite (hFile, (LPSTR)lpBi, (int)GlobalSize(hDib));

	GlobalUnlock (hDib);
	_lclose (hFile);
}

/****************************************************************************
 * FUNCTION: DibNumColors
 *
 * ARGS:    pv  pointer to bitmap data
 *
 * RETURNS: number of colors, 0, 2, 16, 256, in the DIB
 *
 * NOTES:
 *
 ***********************************************************************/

static	WORD DibNumColors (VOID FAR *pv)
{
    int                   Bits;
    LPBITMAPINFOHEADER    lpbi;
    LPBITMAPCOREHEADER    lpbc;

    lpbi = ((LPBITMAPINFOHEADER)pv);
    lpbc = ((LPBITMAPCOREHEADER)pv);

    /*    With the BITMAPINFO format headers, the size of the palette
     *    is in biClrUsed, whereas in the BITMAPCORE - style headers, it
     *    is dependent on the bits per pixel ( = 2 raised to the power of
     *    bits/pixel).
     */
    if (lpbi->biSize != sizeof(BITMAPCOREHEADER)){
        if (lpbi->biClrUsed != 0)
            return (WORD)lpbi->biClrUsed;
        Bits = lpbi->biBitCount;
    }
    else Bits = lpbc->bcBitCount;

    switch (Bits){
        case 1:
        return 2;

        case 4:
        return 16;

        case 8:
        return 256;

        default:
        // A 24 bitcount DIB has no color table
        return 0;
    }
}

/****************************************************************************

  関数:   lwrite(int fh, VOID FAR *pv, DWORD ul)

  目的:   データを32Kバイトずつ、すべてのデータが書き込まれるまで書き込みます。

  戻り値: 正常に書き込めなかった場合は、0を返します。
	  それ以外の場合は、書き込んだバイト数を返します。

 ****************************************************************************/
static	DWORD PASCAL lwrite (int fh, VOID FAR *pv, DWORD ul)
{
   DWORD     ulT = ul;
   BYTE *hp = pv;

   while (ul > BYTES_PER_READ)
      {
      if (_lwrite(fh, (LPSTR)hp, (WORD)BYTES_PER_READ) != BYTES_PER_READ)
		   return 0;

      ul -= BYTES_PER_READ;
      hp += BYTES_PER_READ;
      }

   if (_lwrite(fh, (LPSTR)hp, (WORD)ul) != (WORD)ul)
      return 0;

   return ulT;
}

/***********************************************************************
* TWXferMech
*
* Set the current transfer mechanism for Twain based on the menus
* that have been checked by the user.
***********************************************************************/

TW_INT16 TWXferMech(HWND hWnd)

{
	TW_CAPABILITY 	cap;
   	pTW_ONEVALUE  	pval;
   	TW_INT16      	status;

	cap.Cap = ICAP_XFERMECH;
   	cap.ConType = TWON_ONEVALUE;
	if (cap.hContainer = GlobalAlloc(GHND, sizeof(TW_ONEVALUE)))
   	{
    	pval = (pTW_ONEVALUE)GlobalLock(cap.hContainer);
		pval->ItemType = TWTY_UINT16;

//       	pval->Item = TWSX_MEMORY;
//       	pval->Item = TWSX_FILE;
      	pval->Item = TWSX_NATIVE;

      	GlobalUnlock(cap.hContainer);

		status = (*lpDSM_Entry) (&appID,
									&dsID,
									DG_CONTROL,
									DAT_CAPABILITY,
									MSG_SET,
									(TW_MEMREF)&cap);
      	GlobalFree((HANDLE)cap.hContainer);

		if (status != TWRC_SUCCESS)
		{
		if (MessageLevel() >= ML_ERROR)
			ShowRC_CC(hWnd, 1, status, 1, "",
				"MSG_SET of ICAP_XFERMECH");

		} // not TWRC_SUCCESS
	} else {
   		status = TWRC_FAILURE;
   		if (MessageLevel() >= ML_ERROR)
			ShowRC_CC(hWnd, 0, 0, 0, "Memory Allocation Failed",
				"MSG_SET of ICAP_XFERMECH");
	}
   return status;
}  /* TWXferMech */


/***********************************************************************
* TWAutofeed
*
* Set the current Autofeed Options and Flags based on the menus
* that have been checked by the user.
***********************************************************************/

TW_INT16 TWAutofeed(HWND hWnd)
{
	TW_CAPABILITY 	cap;
   	pTW_ONEVALUE  	pval;
   	TW_INT16      	status;
   	char			Details[128];

	status = TWRC_SUCCESS;
	AutoFeedBOOL = FALSE;
	strcpy(Details, "");

	// If Automatic Feeding > CAP_AUTOFEED On is checked
//	{
		//Autofeed is not checked on the menu
		//Return success
		return (status);
//	}

//-----CAP_FEEDERENABLED
	cap.Cap = CAP_FEEDERENABLED;
   	cap.ConType = TWON_ONEVALUE;
	status = (*lpDSM_Entry) (&appID,
							&dsID,
							DG_CONTROL,
							DAT_CAPABILITY,
							MSG_GET,
							(TW_MEMREF)&cap);

    if (status != TWRC_SUCCESS)
	{
		//CAP_FEEDERENABLED MSG_GET failed cannot use Autofeed
		status = TWRC_FAILURE;
		strcpy(Details, "CAP_FEEDERENABLED");

	} else {	//TWRC_SUCCESS
		//MSG_GET succeeded
   		//Check value returned
		pval = (pTW_ONEVALUE)GlobalLock(cap.hContainer);
		if (pval->Item == TRUE)
		{	//Feeder is enabled
			GlobalUnlock(cap.hContainer);
			GlobalFree((HANDLE)cap.hContainer);
		} else {
			//Try to turn on CAP_FEEDERENABLED
			pval->ItemType = TWTY_BOOL;
			pval->Item = TRUE;
        	GlobalUnlock(cap.hContainer);
        	status = (*lpDSM_Entry) (&appID,
											&dsID,
											DG_CONTROL,
											DAT_CAPABILITY,
											MSG_SET,
											(TW_MEMREF)&cap);
			GlobalFree((HANDLE)cap.hContainer);
			if (status == TWRC_SUCCESS)
			{	//Verify that CAP_FEEDERENABLED is now TRUE
		        status = (*lpDSM_Entry) (&appID,
											&dsID,
											DG_CONTROL,
											DAT_CAPABILITY,
											MSG_GET,
											(TW_MEMREF)&cap);
				if (status == TWRC_SUCCESS)
				{
					pval = (pTW_ONEVALUE)GlobalLock(cap.hContainer);
					if (pval->Item != TRUE)
					{
						status = TWRC_FAILURE;
						strcpy(Details, "CAP_FEEDERENABLED");
						//CAP_FEEDERENABLED is FALSE even after successful set to TRUE
					}
					GlobalUnlock(cap.hContainer);
					GlobalFree((HANDLE)cap.hContainer);
				} else {
					status = TWRC_FAILURE;
					strcpy(Details, "CAP_FEEDERENABLED");
				}
			} else {	//MSG_SET of CAP_FEEDERENABLED to TRUE did not succeed
				strcpy(Details, "CAP_FEEDERENABLED");
				status = TWRC_FAILURE;
			}

		}
		//End of CAP_FEEDERENABLED

		if (status == TWRC_SUCCESS)
		{
//-----CAP_AUTOFEED
			cap.Cap = CAP_AUTOFEED;
	   		cap.ConType = TWON_ONEVALUE;
			status = (*lpDSM_Entry) (&appID,
									&dsID,
									DG_CONTROL,
									DAT_CAPABILITY,
									MSG_GET,
									(TW_MEMREF)&cap);
			if (status != TWRC_SUCCESS)
			{
				//MSG_GET on CAP_AUTOFEED did not succeed
				status = TWRC_FAILURE;
				strcpy(Details, "CAP_AUTOFEED");
			}
			//MSG_GET on CAP_AUTOFEED returned success
			//Check value returned
   			pval = (pTW_ONEVALUE)GlobalLock(cap.hContainer);
			if (pval->Item == TRUE)
			{
				//CAP_AUTOFEED is on
	   			GlobalUnlock(cap.hContainer);
				GlobalFree((HANDLE)cap.hContainer);
			} else {
				// Try to set CAP_AUTOFEED to TRUE
				pval->ItemType = TWTY_BOOL;
				pval->Item = TRUE;
	   			GlobalUnlock(cap.hContainer);
				status = (*lpDSM_Entry) (&appID,
										&dsID,
										DG_CONTROL,
										DAT_CAPABILITY,
										MSG_SET,
										(TW_MEMREF)&cap);
				GlobalFree((HANDLE)cap.hContainer);

				if (status == TWRC_SUCCESS)
				{   //Verify that CAP_AUTOFEED is on
					status = (*lpDSM_Entry) (&appID,
												&dsID,
												DG_CONTROL,
												DAT_CAPABILITY,
												MSG_GET,
												(TW_MEMREF)&cap);
					if (status == TWRC_SUCCESS)
					{
		      			pval = (pTW_ONEVALUE)GlobalLock(cap.hContainer);
						if (pval->Item != TRUE)
						{
							status = TWRC_FAILURE;
							strcpy(Details, "CAP_AUTOFEED");
							//CAP_AUTOFEED returns FALSE even after succesful set to TRUE
   						}
   						GlobalUnlock(cap.hContainer);
						GlobalFree((HANDLE)cap.hContainer);
					} else {
						status = TWRC_FAILURE;
						strcpy(Details, "CAP_AUTOFEED");
					}
				} else {
					//MSG_SET of CAP_AUTOFEED to TRUE did not succeed
					status = TWRC_FAILURE;
					strcpy(Details, "CAP_AUTOFEED");
				}

			}	//else check for CAP_AUTOFEED
		}	//end If CAP_FEEDERENABLED successful
	}	//CAP_FEEDERENABLED MSG_GET

//-----Set Local Autofeed Varible on/off
	if (status == TWRC_SUCCESS)
	{
		AutoFeedBOOL = TRUE;
	} else {
		AutoFeedBOOL = FALSE;
		if (MessageLevel() >= ML_ERROR)
			ShowRC_CC(hWnd, 0, 0, 0, Details, "CAP_AUTOFEED ERROR");
	}

	return status;
}  /* TWAutofeed */

 /***********************************************************************
 * FUNCTION: AutoFeedOn
 *
 * ARGS:    none
 *
 * RETURNS: current state of Source Manager (open/closed)
 *
 * NOTES:   Just a way to reduce the number of global vars
 */
BOOL AutoFeedOn (VOID)
{
    return (AutoFeedBOOL);
}

/***********************************************************************
* TWAutofeedMenu
*
* Set the current Autofeed Options and Flags based on the menus
* that have been checked by the user.
***********************************************************************/
TW_INT16 TWAutofeedMenu(HWND hWnd)
{
   TW_INT16      status;

	status = TWAutofeed(hWnd);
	return (status);
}

/*************************************************************************
* FloatToFIX32
*
* Convert a floating point value into a FIX32.
*************************************************************************/

TW_FIX32 FloatToFIX32 (float floater)
{
	TW_FIX32 Fix32_value;
	TW_INT32 value = (TW_INT32) (floater * 65536.0 + 0.5);
	Fix32_value.Whole = (short)(value >> 16);
	Fix32_value.Frac = (unsigned short)(value & 0x0000ffffL);
	return(Fix32_value);
}

/*************************************************************************
* FIX32ToFloat
*
* Convert a FIX32 value into a floating point value
*************************************************************************/

float FIX32ToFloat (TW_FIX32 fix32)
{
	float 	floater;
	floater = (float) (fix32.Whole + (float) fix32.Frac / 65536.0);
	return(floater);
}

/*************************************************************************
* ShowImageInfo
*
*************************************************************************/

void ShowImageInfo (HWND hWndofApp)

{
   	TW_UINT16       twRC;
   	TW_UINT16       ccRC;
   	TW_IMAGEINFO	twImageInfo;
	char			Details [255];
	char			Details2 [1024];
	pTW_FIX32      	pTWFix32;
   	TW_UINT16       index;
   	TW_STATUS       twStatus;

//=====Check ImageInfo information
  	twRC = (*lpDSM_Entry)(&appID,
						&dsID,
						DG_IMAGE,
					    DAT_IMAGEINFO,
						MSG_GET,
						(TW_MEMREF)&twImageInfo);

	//Return Code from Operation
	sprintf(Details2, "%s %d\n", "RETURN CODE:", twRC);

	switch (twRC)
	{
		case TWRC_SUCCESS:

			sprintf (Details, "\n%s %f", "XResolution:", FIX32ToFloat(twImageInfo.XResolution));
			strcat(Details2, Details);

			pTWFix32 = &(twImageInfo.YResolution);
			sprintf (Details, "\n%s %f", "YResolution:", FIX32ToFloat(*pTWFix32));
			strcat(Details2, Details);

			sprintf (Details, "\n%s %d", "ImageWidth:", twImageInfo.ImageWidth);
			strcat(Details2, Details);

			sprintf (Details, "\n%s %d", "ImageLength:", twImageInfo.ImageLength);
			strcat(Details2, Details);

			sprintf (Details, "\n%s %d", "SamplesPerPixel:", twImageInfo.SamplesPerPixel);
			strcat(Details2, Details);

			sprintf (Details, "\n%s %d", "BitsPerSample:", twImageInfo.BitsPerSample[0]);
			strcat(Details2, Details);
			for (index = 1; index < 8; index++)
			{
				sprintf (Details, ", %d", twImageInfo.BitsPerSample[index]);
				strcat(Details2, Details);
			}

			sprintf (Details, "\n%s %d", "BitsPerPixel:", twImageInfo.BitsPerPixel);
			strcat(Details2, Details);

			sprintf (Details, "\n%s %d", "Planar:", twImageInfo.Planar);
			strcat(Details2, Details);

			sprintf (Details, "\n%s %d", "PixelType:", twImageInfo.PixelType);
			strcat(Details2, Details);

			sprintf (Details, "\n%s %d", "Compression:", twImageInfo.Compression);
			strcat(Details2, Details);

			break;

		case TWRC_FAILURE:
		default:
			// determine details of failure from Source
			ccRC = (*lpDSM_Entry)(&appID,
							&dsID,
                            DG_CONTROL,
                            DAT_STATUS,
                            MSG_GET,
                            (TW_MEMREF)&twStatus);

			if (ccRC == TWRC_SUCCESS)
			{
				sprintf(Details, "\n%s %d", "Condition Code:", twStatus.ConditionCode);
				strcat(Details2, Details);
			} else {
				sprintf(Details, "\n%s", "Condition Code Check Failed");
				strcat(Details2, Details);
			}
			break;
	}

	MessageBox (hWndofApp, Details2, "TW_IMAGEINFO.", MB_OK);
}
/*************************************************************************
* ShowImageLayout
*
*************************************************************************/

void ShowImageLayout (HWND hWndofApp)

{
   	TW_UINT16       twRC;
   	TW_UINT16       ccRC;
   	TW_IMAGELAYOUT	twImageLayout;
	char			Details [255];
	char			Details2 [1024];
   	TW_STATUS		twStatus;

//=====Check ImageLayout information
  	twRC = (*lpDSM_Entry)(&appID,
							&dsID,
							DG_IMAGE,
						    DAT_IMAGELAYOUT,
							MSG_GET,
							(TW_MEMREF)&twImageLayout);

	//Return Code from Operation
	sprintf(Details2, "%s %d\n", "RETURN CODE:", twRC);

	switch (twRC)
	{
		case TWRC_SUCCESS:

			sprintf (Details, "\n%s %f", "Frame.Left:", FIX32ToFloat(twImageLayout.Frame.Left));
			strcat(Details2, Details);
			sprintf (Details, "\n%s %f", "Frame.Top:", FIX32ToFloat(twImageLayout.Frame.Top));
			strcat(Details2, Details);
			sprintf (Details, "\n%s %f", "Frame.Right:", FIX32ToFloat(twImageLayout.Frame.Right));
			strcat(Details2, Details);
			sprintf (Details, "\n%s %f", "Frame.Bottom:", FIX32ToFloat(twImageLayout.Frame.Bottom));
			strcat(Details2, Details);

			sprintf (Details, "\n%s %d", "DocumentNumber:", twImageLayout.DocumentNumber);
			strcat(Details2, Details);

			sprintf (Details, "\n%s %d", "PageNumber:", twImageLayout.PageNumber);
			strcat(Details2, Details);

			sprintf (Details, "\n%s %d", "FrameNumber:", twImageLayout.FrameNumber);
			strcat(Details2, Details);

			break;

		case TWRC_FAILURE:
		default:
			// determine details of failure from Source
			ccRC = (*lpDSM_Entry)(&appID,
								&dsID,
	                            DG_CONTROL,
	                            DAT_STATUS,
	                            MSG_GET,
	                            (TW_MEMREF)&twStatus);

			if (ccRC == TWRC_SUCCESS)
			{
				sprintf(Details, "\n%s %d", "Condition Code:", twStatus.ConditionCode);
				strcat(Details2, Details);
			} else {
				sprintf(Details, "\n%s", "Condition Code Check Failed");
				strcat(Details2, Details);
			}
			break;
	}

	MessageBox (hWndofApp, Details2, "TW_IMAGELAYOUT.", MB_OK);
}
/*************************************************************************
* ShowTW_ID
*
*************************************************************************/

void ShowTW_ID (HWND hWndofApp, TW_IDENTITY twID, LPSTR text)

{
	char			Details [255];
	char			Details2 [1024];

	sprintf(Details2, "\n%s %d", "Id:", twID.Id);
	sprintf (Details, "\n%s %d", "TW_VERSION.MajorNum:", twID.Version.MajorNum);
	strcat(Details2, Details);
	sprintf (Details, "\n%s %d", "TW_VERSION.MinorNum:", twID.Version.MinorNum);
	strcat(Details2, Details);
	sprintf (Details, "\n%s %d", "TW_VERSION.Language:", twID.Version.Language);
	strcat(Details2, Details);
	sprintf (Details, "\n%s %d", "TW_VERSION.Country:", twID.Version.Country);
	strcat(Details2, Details);
	sprintf (Details, "\n%s %s", "TW_VERSION.Info:", twID.Version.Info);
	strcat(Details2, Details);
	sprintf (Details, "\n%s %d", "ProtocolMajor:", twID.ProtocolMajor);
	strcat(Details2, Details);
	sprintf (Details, "\n%s %d", "ProtocolMinor:", twID.ProtocolMinor);
	strcat(Details2, Details);
	sprintf (Details, "\n%s %d", "SupportedGroups:", twID.SupportedGroups);
	strcat(Details2, Details);
	sprintf (Details, "\n%s %s", "Manufacturer:", twID.Manufacturer);
	strcat(Details2, Details);
	sprintf (Details, "\n%s %s", "ProductFamily:", twID.ProductFamily);
	strcat(Details2, Details);
	sprintf (Details, "\n%s %s", "ProductName:", twID.ProductName);
	strcat(Details2, Details);

	MessageBox (hWndofApp, Details2, text, MB_OK);
}
/*************************************************************************
* ShowCapability
*
*************************************************************************/

void ShowCapability (HWND hWndofApp, TW_UINT16 Cap)

{
   	TW_UINT16         	twRC;
   	TW_UINT16         	ccRC;
   	TW_CAPABILITY		twCapability;
	char				Details [255];
	char				Details2 [1024];
   	TW_UINT16         	index;
   	TW_STATUS         	twStatus;
	pTW_ENUMERATION		pEnum;
	pTW_ONEVALUE		pOneV;
	pTW_RANGE			pRange;
	pTW_ARRAY			pArray;
	LPSTR				pData;
	TW_INT16			datasize;

	twCapability.Cap = Cap;
	twCapability.ConType = TWON_DONTCARE16;

//=====Check Capability information
  	twRC = (*lpDSM_Entry)(&appID,
								&dsID,
								DG_CONTROL,
								DAT_CAPABILITY,
								MSG_GET,
								(TW_MEMREF)&twCapability);

	//Return Code from Operation
	sprintf(Details, "%s %d\n", "RETURN CODE:", twRC);
	strcpy(Details2, Details);

	//Show TW_CAPABILITY Contents
	MatchTwainInt("CAP", twCapability.Cap, Details, sizeof(Details) );
	strcat(Details2, "\nCap: ");
	strcat(Details2, Details);

	switch (twRC)
	{
		case TWRC_SUCCESS:

			MatchTwainInt("ConType", twCapability.ConType, Details, sizeof(Details) );
			strcat(Details2, "\nConType: ");
			strcat(Details2, Details);

			//Show Container Contents
			strcat(Details2, "\nhContainer.");
			switch(twCapability.ConType)
			{
				case TWON_ONEVALUE:

					pOneV = (pTW_ONEVALUE)GlobalLock(twCapability.hContainer);

					strcat(Details2, "\n     ItemType: ");
					MatchTwainInt("ItemType", pOneV->ItemType, Details, sizeof(Details) );
					strcat(Details2, Details);

					strcat(Details2, "\n     Item: ");
					GetItem(Details, pOneV->ItemType, (LPSTR)&(pOneV->Item));
					strcat(Details2, Details);

					GlobalUnlock(twCapability.hContainer);
					GlobalFree((HANDLE)twCapability.hContainer);

					break;

				case TWON_ENUMERATION:
					pEnum = (pTW_ENUMERATION)GlobalLock(twCapability.hContainer);

					strcat(Details2, "\n     ItemType: ");
					MatchTwainInt("ItemType", pEnum->ItemType, Details, sizeof(Details) );
	           	datasize = GetPrivateProfileInt("ItemSize", Details, 4, IniFile);
					strcat(Details2, Details);

					sprintf (Details, "\n     %s %d", "NumItems:", pEnum->NumItems);
					strcat(Details2, Details);

					sprintf (Details, "\n     %s %d", "CurrentIndex:", pEnum->CurrentIndex);
					strcat(Details2, Details);

					sprintf (Details, "\n     %s %d", "DefaultIndex:", pEnum->DefaultIndex);
					strcat(Details2, Details);

					strcat(Details2, "\n     ItemList: ");
					pData = pEnum->ItemList;
					//index=0
					GetItem(Details, pEnum->ItemType, pData);
					strcat(Details2, Details);
					pData+=datasize;
					for (index=1; index < pEnum->NumItems; index++)
					{
						strcat(Details2, ", ");
						GetItem(Details, pEnum->ItemType, pData);
						strcat(Details2, Details);
						pData+=datasize;
					}
					GlobalFree(twCapability.hContainer);

					break;

				case TWON_RANGE:
					pRange = (pTW_RANGE)GlobalLock(twCapability.hContainer);

					strcat(Details2, "\n     ItemType: ");
					MatchTwainInt("ItemType", pRange->ItemType, Details, sizeof(Details) );
					strcat(Details2, Details);

					strcat(Details2, "\n     MinValue: ");
					GetItem(Details, pRange->ItemType, (LPSTR)&(pRange->MinValue));
					strcat(Details2, Details);

					strcat(Details2, "\n     MaxValue: ");
					GetItem(Details, pRange->ItemType, (LPSTR)&(pRange->MaxValue));
					strcat(Details2, Details);

					strcat(Details2, "\n     StepSize: ");
					GetItem(Details, pRange->ItemType, (LPSTR)&(pRange->StepSize));
					strcat(Details2, Details);

					strcat(Details2, "\n     DefaultValue: ");
					GetItem(Details, pRange->ItemType, (LPSTR)&(pRange->DefaultValue));
					strcat(Details2, Details);

					strcat(Details2, "\n     CurrentValue: ");
					GetItem(Details, pRange->ItemType, (LPSTR)&(pRange->CurrentValue));
					strcat(Details2, Details);

					GlobalFree(twCapability.hContainer);
					break;

				case TWON_ARRAY:
					pArray = (pTW_ARRAY)GlobalLock(twCapability.hContainer);

					strcat(Details2, "\n     ItemType: ");
					MatchTwainInt("ItemType", pArray->ItemType, Details, sizeof(Details) );
	           		datasize = GetPrivateProfileInt("ItemSize", Details, 4, IniFile);
					strcat(Details2, Details);

					sprintf (Details, "\n     %s %d", "NumItems:", pArray->NumItems);
					strcat(Details2, Details);

					strcat(Details2, "\n     ItemList: ");
					pData = pArray->ItemList;
					//index=0
					GetItem(Details, pArray->ItemType, pData);
					strcat(Details2, Details);
					pData+=datasize;
					for (index=1; index < pArray->NumItems; index++)
					{
						strcat(Details2, ", ");
						GetItem(Details, pArray->ItemType, pData);
						strcat(Details2, Details);
						pData+=datasize;
					}
					GlobalFree(twCapability.hContainer);

					break;

				default:
					break;
			}

			break;

		case TWRC_FAILURE:
		default:
			// determine details of failure from Source
			ccRC = (*lpDSM_Entry)(&appID,
								&dsID,
	                           	DG_CONTROL,
	                           	DAT_STATUS,
	                           	MSG_GET,
	                           	(TW_MEMREF)&twStatus);

			if (ccRC == TWRC_SUCCESS)
			{
				sprintf(Details, "\n%s %d", "Condition Code:", twStatus.ConditionCode);
				strcat(Details2, Details);
			} else {
				sprintf(Details, "\n%s %d", "Condition Code:", twStatus.ConditionCode);
				strcat(Details2, Details);
			}
			break;
	}

	MessageBox (hWndofApp, Details2, "TW_CAPABILITY.", MB_OK);
}
/*************************************************************************
*  GetItem
*
*************************************************************************/
void GetItem (char * Text, TW_UINT16 datatype, LPSTR pData)
{

	switch (datatype)
 	{
	 	case 0: //TWTY_INT8
 		{
 			TW_INT8	value = *(pTW_INT8)pData;
  			sprintf(Text, "%d", (TW_INT16)value);
    	}	/* case */
 		break;

	 	case 3: // TWTY_UINT8
 		{
 			TW_UINT8	value = *(pTW_UINT8)pData;
  			sprintf(Text,"%u", (TW_UINT16)value);
    	}	/* case */
 		break;

	 	case 1: // TWTY_INT16
	 	case 44: // TWTY_HANDLE
 		{
 			TW_INT16	value = *(pTW_INT16)pData;
  			sprintf(Text,"%d", value);
    	}	/* case */
 		break;

	 	case 4: // TWTY_UINT16
	 	case 6: // TWTY_BOOL
 		{
 			TW_UINT16	value = *(pTW_UINT16)pData;
  			sprintf(Text,"%u", value);
    	}	/* case */
 		break;

	 	case 2: // TWTY_INT32
 		{
 			TW_INT32	value = *(pTW_INT32)pData;
  			sprintf(Text,"%ld", value);
    	}	/* case */
 		break;

	 	case 5: // TWTY_UINT32
	 	case 43: // TWTY_MEMREF
 		{
 			TW_UINT32	value = *(pTW_UINT32)pData;
  			sprintf(Text,"%lu", value);
    	}	/* case */
 		break;

 	case 7:	// TWTY_FIX32
 		{
 			float	value;
			TW_FIX32 fix32;

 			fix32.Whole = ((pTW_FIX32)pData)->Whole;
 			fix32.Frac = ((pTW_FIX32)pData)->Frac;
			value = FIX32ToFloat(fix32);
			sprintf(Text, "%f", value);

 		}	/* case */
 		break;

	case 9: //TWTY_STR32
	case 10: //TWTY_STR64
	case 11: //TWTY_STR128
	case 12: //TWTY_STR255
 	default:
 		lstrcpy(Text, pData);
 		break;
 	}	/* switch */
}
/*************************************************************************
* ShowPendingXfers
*
*************************************************************************/

void ShowPendingXfers (HWND hWndofApp)

{
	TW_UINT16         	twRC;
	TW_UINT16         	ccRC;
	TW_PENDINGXFERS		twPendingXfers;
	char				Details [255];
	char				Details2 [1024];
	TW_STATUS         	twStatus;

//=====Inquire Pending Xfers
  	twRC = (*lpDSM_Entry)(&appID,
							&dsID,
							DG_CONTROL,
					        DAT_PENDINGXFERS,
							MSG_GET,
							(TW_MEMREF)&twPendingXfers);

	//Return Code from Operation
	sprintf(Details2, "%s %d\n", "RETURN CODE:", twRC);

	switch (twRC)
	{
		case TWRC_SUCCESS:

			sprintf (Details, "\n%s %d", "Count:", twPendingXfers.Count);
			strcat(Details2, Details);
			break;

		case TWRC_FAILURE:
		default:
			// determine details of failure from SM
			ccRC = (*lpDSM_Entry)(&appID,
								&dsID,
	                           	DG_CONTROL,
	                           	DAT_STATUS,
	                           	MSG_GET,
	                           	(TW_MEMREF)&twStatus);

			if (ccRC == TWRC_SUCCESS)
			{
				sprintf(Details, "\n%s %d", "Condition Code:", twStatus.ConditionCode);
				strcat(Details2, Details);
			} else {
				sprintf(Details, "\n%s %d", "Condition Code:", twStatus.ConditionCode);
				strcat(Details2, Details);
			}
			break;
	}

	MessageBox (hWndofApp, Details2, "TW_PENDINGXRFERS.", MB_OK);
}
/*************************************************************************
* ShowRC_CC
*
*************************************************************************/
void ShowRC_CC (HWND		hWndofApp,
				TW_UINT16	DisplayRC, 	//0-No, 1-Yes
				TW_UINT16	ReturnCode,
				TW_UINT16	Dest,		//0-Source Manager or 1-Source
				LPSTR		Text,		//Additional Information
				LPSTR		Title)		//Triplet or Cap

{
   	TW_UINT16       twRC;
	char			Details [255];
	char			Details2 [1024];
   	TW_STATUS       twStatus;

	strcpy(Details, "");
	strcpy(Details2, "");
	if(DisplayRC == 1)	//Display Return Code
	{
//=====Get Return Code to be displayed
		MatchTwainInt("ReturnCode", (TW_INT32)ReturnCode, Details2, sizeof(Details2));
		//Details2 contains Return Code

	//=====Check Condition Code if RC != TWRC_SUCCESS
		if (ReturnCode != TWRC_SUCCESS)
		{
			if (Dest == 0) //Source Manager
			{
				if (TWIsDSMOpen())	//The Source Manager is open
				{
				  	twRC = (*lpDSM_Entry)(&appID,
									NULL,
									DG_CONTROL,
					         		DAT_STATUS,
									MSG_GET,
									(TW_MEMREF)&twStatus);
				} else {
					//Source Manager not open
					twRC = TWRC_FAILURE;
				}
			} else {	//Source
			//Check that a DS is open
				if (TWIsDSOpen())
				{
				  	twRC = (*lpDSM_Entry)(&appID,
									&dsID,
									DG_CONTROL,
					         		DAT_STATUS,
									MSG_GET,
									(TW_MEMREF)&twStatus);
				} else {
					//Source is not open
					twRC = TWRC_FAILURE;
				}
			}
		}
		if (twRC == TWRC_SUCCESS)
		{
			MatchTwainInt("ConditionCode", (TW_INT32)twStatus.ConditionCode, Details, sizeof(Details));
			//Details contains Condition Code
		} else {
			sprintf (Details, "%s", "CC FAILED");
		}
		strcat(Details2, "/");
		strcat(Details2, Details);
		strcat(Text, "\n");
		strcat(Text, Details2);
	}
	MessageBox (hWndofApp, Text, Title, MB_OK);
}
/***********************************************************************
 * FUNCTION: MessageLevel
 *
 * ARGS:    none
 *
 * RETURNS: current state of Source Manager (open/closed)
 *
 * NOTES:   Just a way to reduce the number of global vars
 */
int MessageLevel (VOID)
{
    return (MessageLevelVAR);
}

/***********************************************************************
 * FUNCTION: SetMessageLevel
 *
 * ARGS:    none
 *
 * RETURNS: current state of Source Manager (open/closed)
 *
 * NOTES:   Just a way to reduce the number of global vars
 */
void SetMessageLevel (int Level)
{
	MessageLevelVAR = Level;
}

/*---------------------------------------------------------------------------*/
TW_INT16 MatchTwainInt(LPSTR pName, TW_INT32 Id, LPSTR pString
	, TW_INT16 MaxCount)

/*
**	Parameters:	pName
				Id
				pString
				MaxCount
**	Globals:
**	Operation:
**	Return:
**	History:
*/

{
	HANDLE		hfield;
	LPSTR		pfield=NULL;
	TW_INT16	result = 0;
	TW_INT16	number;

	if (hfield = GlobalAlloc(GHND, 2048))
	{
		pfield = GlobalLock(hfield);
		if (GetPrivateProfileString(pName, NULL, "", pfield, 2048, IniFile))
	    {
		    for (; *pfield; pfield+=lstrlen(pfield)+1)
		    {
		    	number = GetPrivateProfileInt(pName, pfield, -1, IniFile);
		    	if (number == Id)
		    	{
		    		if (lstrlen(pfield) > MaxCount) *(pfield+MaxCount-1) = 0;
		    		lstrcpy(pString, pfield);
		    		result = 1;
		    		break;
		    	}	/* if */
			}	/* for */
		}	/* if */
		if (pfield) GlobalUnlock(hfield);
		GlobalFree(hfield);
	}	/* if */
	return	result;
}	/* MatchTwainInt */


