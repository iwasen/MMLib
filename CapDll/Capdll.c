/****************************************************************************
 |
 |   			îƒópÅ@ÉJÉÅÉââÊëúéÊÇËçûÇ›ÇcÇkÇk
 |
 |			Ãß≤Ÿñº :		capdll.c
 |			çÏê¨é“ :		m.hama
 |			ì˙ïtÅF			96/07/24
 |
 |
 ***************************************************************************/
#pragma warning(disable: 4819)

#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <mmsystem.h>
#include <vfw.h>
//#include <msvideo.h>
#include <mmreg.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <dos.h>

//#include <avicap.h>
#include <msacm.h>
#include "cap.h"
#include "capdll.h"
#include "extern.h"
#include "resource.h"
#include "dib.h"

#define ENABLE_ERROR_CALLBACK           1
#define ENABLE_STATUS_CALLBACK          1
#define ENABLE_VIDEOFRAME_CALLBACKS     1
#define ENABLE_VIDEOSTREAM_CALLBACKS    1

#define	YSPACE	80

#define MoveTo(p1,p2,p3)	MoveToEx(p1, p2, p3, NULL)

//
// Global Variables
//
char	gachAppName[]  = "PANCapDLL" ;
char    gachMCIDeviceName[21] = "VideoDisc" ;  // default MCI device
char	gfilename[64];
char	gfilename2[64];
char	gfilename3[64];

HINSTANCE      ghInst ;
HWND           ghWndCap, ghWnd;
WORD           gwDeviceIndex ;
WORD           gwPalFrames = DEF_PALNUMFRAMES ;
WORD           gwPalColors = DEF_PALNUMCOLORS ;
WORD           gwCapFileSize = 1 ;
DWORD          gdwFrameNum ;
DWORD          gdwVideoNum ;
BOOL           fHaveDialogUp = FALSE;
HANDLE	ghDIB;
int		gMode;

CAPSTATUS      gCapStatus ;
CAPDRIVERCAPS  gCapDriverCaps ;
CAPTUREPARMS   gCapParms ;

static HPEN	hPenBlack;
static HPEN	hPenLightGray;
static HBRUSH	hBrushGray;
static HBRUSH	hBrushLightGray;
static	int	gWidth, gHeight;
static	BOOL	gCapDoFlag;
static BOOL bCapRequest;

//
// Function prototypes
//
int	APIENTRY PSCD_GetDib(HWND, LPSTR);
int APIENTRY CapProc(HWND, UINT, UINT, LONG);
int APIENTRY PSCD_GetAvi(HWND, LPSTR);
int APIENTRY GetAviProc(HWND, UINT, UINT, LONG);

LRESULT APIENTRY ErrorCallbackProc(HWND, int, LPSTR) ;
LRESULT APIENTRY StatusCallbackProc(HWND, int, LPSTR) ;
LRESULT APIENTRY FrameCallbackProc(HWND, LPVIDEOHDR) ;
LRESULT APIENTRY VideoCallbackProc(HWND, LPVIDEOHDR) ;

static long MenuProc(HWND, WPARAM, LPARAM);
static long GetAviMenuProc(HWND, WPARAM, LPARAM);

static void PaintWindow(HWND, HWND);
static void	chgSize(HWND, HWND);
static HANDLE GetDIB(HWND hWnd, LPVIDEOHDR lpVHdr);

BOOL	WINAPI	DllMain(HINSTANCE hInstDLL, DWORD fdwReason,
						LPVOID lpvReserved)
{
	ghInst = hInstDLL;
	return TRUE;
}

/************************************************************************
 |
 |		DIBéBâe
 |
 ***********************************************************************/
int	APIENTRY PSCD_GetDib(HWND hWnd, LPSTR filename)
{
	gCapDoFlag = FALSE;
	lstrcpy((LPSTR)gfilename, filename);
	gMode = 0;
	fHaveDialogUp = TRUE;
	DialogBox(ghInst, MAKEINTRESOURCE(IDD_CAPDIALOG), hWnd, (DLGPROC)CapProc) ;
	fHaveDialogUp = FALSE;
	return(gCapDoFlag);
}

HANDLE	APIENTRY PSCD_GetBmp(HWND hWnd)
{
	gCapDoFlag = FALSE;
	ghDIB = NULL;
	gMode = 1;
	fHaveDialogUp = TRUE;
	DialogBox(ghInst, MAKEINTRESOURCE(IDD_CAPDIALOG), hWnd, (DLGPROC)CapProc) ;
	fHaveDialogUp = FALSE;
	return(ghDIB);
}

void	APIENTRY PSCD_GetBmpContinue(HWND hWnd)
{
	gCapDoFlag = FALSE;
	ghDIB = NULL;
	gMode = 2;
	fHaveDialogUp = TRUE;
	DialogBox(ghInst, MAKEINTRESOURCE(IDD_CAPDIALOG), hWnd, (DLGPROC)CapProc) ;
	fHaveDialogUp = FALSE;
}

/************************************************************************
 |
 |		AVIéBâe
 |
 ***********************************************************************/
int	APIENTRY PSCD_GetAvi(HWND hWnd, LPSTR AviFileName)
{
	BOOL	rt;

	lstrcpy((LPSTR)gfilename, AviFileName);

	fHaveDialogUp = TRUE;
	rt = (BOOL)DialogBox(ghInst, MAKEINTRESOURCE(IDD_GETAVI), hWnd, (DLGPROC)GetAviProc) ;
	fHaveDialogUp = FALSE;

	return(rt);
}

LRESULT APIENTRY ErrorCallbackProc(HWND hWnd, int nErrID, LPSTR lpErrorText)
{
    if (nErrID == 0)            // Starting a new major function
        return TRUE;            // Clear out old errors...

    // Show the error ID and text
    MessageBox(hWnd, lpErrorText, (LPSTR)"Message",
#ifdef BIDI
                MB_RTL_READING |
#endif
                MB_OK | MB_ICONEXCLAMATION) ;

    return (LRESULT) TRUE ;
}


LRESULT APIENTRY StatusCallbackProc(HWND hWnd, int nID, LPSTR lpStatusText)
{
    if (nID == 0) {              // Zero means clear old status messages
        return (LRESULT) TRUE ;
    }

    // Show the status ID and status text...
    // unless we get the "end of capture" message
    if (nID != IDS_CAP_END) {
		;
    }

    return (LRESULT) TRUE ;
}

LRESULT APIENTRY FrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	if (bCapRequest) {
		ghDIB = GetDIB(hWnd, lpVHdr);
		if (gMode == 2)
			SendMessage(GetParent(GetParent(hWnd)), WM_FRAMEBMP, (WPARAM)ghDIB, 0);
		else
			PostMessage(GetParent(hWnd), WM_CLOSE, 0, 0);
		bCapRequest = FALSE;
	}
    return (LRESULT) TRUE ;
}


LRESULT APIENTRY VideoCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
{
    return (LRESULT) TRUE ;
}

/************************************************************************
 |
 |		ÉLÉÉÉvÉ`ÉÉÉEÉBÉìÉhÉEíÜêSÇ÷
 |
 ***********************************************************************/
static void CenterCaptureWindow(HWND hWndM, HWND hWndC)
{
    RECT       MainRect ;
    RECT       CapRect;
    WORD       wCapXPos ;
    WORD       wCapYPos ;

    // Get the sizes of main and capture windows and
    // calculate the location for centering
    GetClientRect(hWndM, &MainRect) ;
    GetClientRect(hWndC, &CapRect) ;
    wCapXPos = (WORD)max(0, (Width(MainRect) - Width(CapRect)) / 2) ;
    wCapYPos = (WORD)max(YSPACE, (Height(MainRect) - Height(CapRect)) / 2);

    // Position the capture window at the required location
    MoveWindow(hWndC, wCapXPos, wCapYPos, Width(CapRect), Height(CapRect), TRUE) ;
	InvalidateRect(hWndM, NULL, TRUE);

}

/************************************************************************
 |
 |		É_ÉCÉAÉçÉOÉTÉCÉYí≤êÆ
 |
 ***********************************************************************/
static	void	chgSize(HWND hWnd, HWND ghWndCap)
{
	capGetStatus(ghWndCap, &gCapStatus, sizeof(CAPSTATUS)) ;
	SetWindowPos(ghWndCap, NULL, 0, 0, gCapStatus.uiImageWidth,
                        gCapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE) ;
	gWidth =  max(gCapStatus.uiImageWidth, 640);
	gHeight = max(gCapStatus.uiImageHeight, 480);
	{
		RECT	MainRect;

    	GetClientRect(hWnd, &MainRect) ;
		if (Width(MainRect) - gWidth < 50
				|| Height(MainRect) - gHeight < YSPACE+50){
		    	SetWindowPos(hWnd, NULL, 0, 0
		    		, max(gWidth+50, Width(MainRect))
		    		, max(gHeight+YSPACE+50, Height(MainRect))
		    		, SWP_NOZORDER | SWP_NOMOVE) ;
		}
	}

	CenterCaptureWindow(hWnd, ghWndCap) ;
}

/************************************************************************
 |
 |		ÉLÉÉÉvÉ`ÉÉÅ[ÉEÉBÉìÉhÉEòg
 |
 ***********************************************************************/
static void PaintWindow(HWND hWndM, HWND hWndC)
{
    RECT       MainRect ;
    RECT       CapRect, rect;
    WORD       wCapXPos ;
    WORD       wCapYPos ;
	HDC	hDC;
	int	dot;
	HPEN hPenOrg;

    // Get the sizes of main and capture windows and
    // calculate the location for centering
    GetClientRect(hWndM, &MainRect) ;
    GetClientRect(hWndC, &CapRect) ;
    wCapXPos = (WORD)max(10, (Width(MainRect) - Width(CapRect)) / 2) ;
    wCapYPos = (WORD)max(YSPACE, (Height(MainRect) - Height(CapRect)) / 2) ;

    hDC = GetDC(hWndM);
	dot = 4;

	/* ç∂ */
	rect.left = wCapXPos - dot;
	rect.top = wCapYPos - dot;
	rect.right = wCapXPos;
	rect.bottom = wCapYPos + Height(CapRect) + dot;
	FillRect(hDC, &rect, hBrushLightGray);

	hPenOrg = SelectObject(hDC, hPenLightGray);
	MoveTo(hDC, rect.left, rect.top);
	LineTo(hDC, rect.left, rect.bottom);

	SelectObject(hDC, hPenBlack);
	MoveTo(hDC, rect.left+dot-1, rect.top+dot);
	LineTo(hDC, rect.left+dot-1, rect.bottom-dot);

	/* âE */
	rect.left = wCapXPos  + Width(CapRect);
	rect.top = wCapYPos - dot;
	rect.right = wCapXPos + Width(CapRect) + dot;
	rect.bottom = wCapYPos + Height(CapRect) + dot;
	FillRect(hDC, &rect, hBrushLightGray);

	SelectObject(hDC, hPenLightGray);
	MoveTo(hDC, rect.right-dot+1, rect.top+dot-1);
	LineTo(hDC, rect.right-dot+1, rect.bottom-dot+1);

	SelectObject(hDC, hPenBlack);
	MoveTo(hDC, rect.right, rect.top);
	LineTo(hDC, rect.right, rect.bottom);
	/* è„ */
	rect.left = wCapXPos - dot;
	rect.top = wCapYPos - dot;
	rect.right = wCapXPos + Width(CapRect) + dot;
	rect.bottom = wCapYPos;
	FillRect(hDC, &rect, hBrushLightGray);

	SelectObject(hDC, hPenLightGray);
	MoveTo(hDC, rect.left, rect.top);
	LineTo(hDC, rect.right, rect.top);

	SelectObject(hDC, hPenBlack);
	MoveTo(hDC, rect.left+dot-1, rect.bottom);
	LineTo(hDC, rect.right-dot, rect.bottom);

	/* â∫ */
	rect.left = wCapXPos - dot;
	rect.top = wCapYPos + Height(CapRect);
	rect.right = wCapXPos + Width(CapRect) + dot;
	rect.bottom = wCapYPos + Height(CapRect) + dot;
	FillRect(hDC, &rect, hBrushLightGray);

	SelectObject(hDC, hPenLightGray);
	MoveTo(hDC, rect.left+dot-1, rect.top);
	LineTo(hDC, rect.right-dot, rect.top);

	SelectObject(hDC, hPenBlack);
	MoveTo(hDC, rect.left, rect.bottom);
	LineTo(hDC, rect.right, rect.bottom);

	SelectObject(hDC, hPenOrg);

    ReleaseDC(hWndM, hDC);
}


//
// StartNewVideoChannel: Gets Selected Driver's Caps -- Updates menu,
//                       Checks Image Size -- Resizes display window,
//                       Enables Preview (at 15 FPS rate)
//
static void StartNewVideoChannel(HWND hWndM, HWND hWndC, WORD wIndex)
{
////////////////////////////////////////////////////////////////////////
//  hWndM:      Application main window handle
//  hWndC:      Capture window handle
//  wIndex:     Selected capture driver index
////////////////////////////////////////////////////////////////////////

    capDriverGetCaps(hWndC, &gCapDriverCaps, sizeof(CAPDRIVERCAPS)) ;
    // Get video format and adjust capture window
    capGetStatus(hWndC, &gCapStatus, sizeof(CAPSTATUS)) ;
    SetWindowPos(hWndC, NULL, 0, 0, gCapStatus.uiImageWidth,
                 gCapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE) ;

    // Start preview by default
    capPreviewRate(hWndC, MS_FOR_15FPS) ;
    capPreview(hWndC, TRUE) ;
}

//
// MenuProc: Processes All Menu-based Operations
//
static	long MenuProc(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
////////////////////////////////////////////////////////////////////////
//  hWnd:      Application main window handle
//  hMenu:     Application menu handle
//  wParam:    Menu option
//  lParam:    Additional info for any menu option
////////////////////////////////////////////////////////////////////////

    WORD         wIndex ;
	char		achBuffer[_MAX_PATH];

	/* èÛë‘éÊìæ */
    capGetStatus(ghWndCap, &gCapStatus, sizeof(CAPSTATUS)) ;

	/*∑¨Ãﬂ¡¨∞íÜÇÕ“Øæ∞ºﬁÇÕñ≥éã */
    if (gCapStatus.fCapturingNow) {
        MessageBeep ((UINT) -1);
        return FALSE;
    }

    switch (wParam) {
        case IDM_E_COPY:
            capEditCopy(ghWndCap) ;
            break;

        case IDM_E_PASTEPALETTE:
            capPalettePaste(ghWndCap) ;
            break;

        case IDM_O_VIDEOFORMAT:
            if (gCapDriverCaps.fHasDlgVideoFormat) {
                // Only if the driver has a "Video Format" dialog box
                fHaveDialogUp = TRUE;
                if (capDlgVideoFormat(ghWndCap)) {  // If successful,
                    // Get the new image dimension and center capture window
                    capGetStatus(ghWndCap, &gCapStatus, sizeof(CAPSTATUS)) ;
                    SetWindowPos(ghWndCap, NULL, 0, 0, gCapStatus.uiImageWidth,
                        gCapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE) ;
                    CenterCaptureWindow(hWnd, ghWndCap) ;
                }
                fHaveDialogUp = FALSE;
				SetFocus(hWnd);
            }
            break;

        case IDOK :
			/* ÇcÇhÇaéÊìæ */
//			capFileSaveDIB(ghWndCap, (LPSTR)gfilename);
//			gCapDoFlag = TRUE;
			bCapRequest = TRUE;
			if (gMode == 2)
				capGrabFrameNoStop(ghWndCap);
			else
				capGrabFrame(ghWndCap);
			break;

        case IDCANCEL:
			EndDialog(hWnd, TRUE) ;
			return TRUE ;

		case IDC_COLOR:
			/* ÇQÇTÇUêFÉpÉåÉbÉg1ÉtÉåÅ[ÉÄÇ©ÇÁê›íË */
            StartNewVideoChannel(hWnd, ghWndCap, gwDeviceIndex) ;
			capPaletteAuto(ghWndCap, 1, 256) ;
			break;
        case IDM_O_VIDEOSOURCE:
            if (gCapDriverCaps.fHasDlgVideoSource) {
                // Only if the driver has a "Video Source" dialog box
                fHaveDialogUp = TRUE;
                capDlgVideoSource(ghWndCap) ;
                fHaveDialogUp = FALSE;
				SetFocus(hWnd);
            }
            break ;
        default:
            // There is a chance, a driver change has been requested
            if ( IsDriverIndex(wParam) ) {
                // If it's a valid driver index...
                if (wParam - IDM_O_DRIVERS != gwDeviceIndex) {
                    // and a different one too then we need to do the rest

                    // Turn off preview/overlay, uncheck current driver option
                    capPreview(ghWndCap, FALSE) ;
                    capOverlay(ghWndCap, FALSE) ;

                    // Connect to requested driver
                    if ( capDriverConnect(ghWndCap, (wIndex = (WORD)wParam - IDM_O_DRIVERS)) ) {
                        // Connect worked fine -- update menu, start new driver...
                        gwDeviceIndex = (WORD)wParam - IDM_O_DRIVERS ;
                        CenterCaptureWindow(hWnd, ghWndCap) ;
                    }
                    else {
                        // if connect failed, re-connect back to previous driver
                        if (! capDriverConnect(ghWndCap, gwDeviceIndex)) {
                            MessageBox(hWnd, "Now can't connect back to previous driver !!",
                                       "Error",
                            MB_OK | MB_ICONSTOP) ;
                            return -1L ;
                        }
                        else
                            // Re-start previous driver as it was before
                            StartNewVideoChannel(hWnd, ghWndCap, gwDeviceIndex) ;
                            CenterCaptureWindow(hWnd, ghWndCap) ;
                    }
                }   // end of if ( != gwDeviceIndex)
            }   // end of if (IsDriverIndex())
            else {
                wsprintf(achBuffer, "How could you specify this (%u) Driver Index ?",
                         wParam - IDM_O_DRIVERS) ;
                MessageBox(hWnd, achBuffer, "Oops!!",
                MB_OK | MB_ICONEXCLAMATION) ;
            }

            break ;
    }

    return 0L ;
}

int APIENTRY CapProc(HWND hWnd, UINT Message, UINT wParam, LONG lParam)
{
////////////////////////////////////////////////////////////////////////
//  hWnd:      Application main window handle
//  Message:   Next message to be processed
//  wParam:    WORD param for the message
//  lParam:    LONG param for the message
////////////////////////////////////////////////////////////////////////

    switch (Message) {
        case WM_COMMAND:
            MenuProc(hWnd, wParam, lParam) ;
            break ;
        case WM_INITDIALOG:
        {
            char    achDeviceName[80] ;
            char    achDeviceVersion[100] ;
            WORD    wDriverCount = 0 ;
            WORD    wIndex ;
            WORD    wError ;

            // First create the capture window
            ghWndCap = capCreateCaptureWindow((LPSTR)"Capture Window",
                             WS_CHILD | WS_VISIBLE,
                             0, 0, 160, 120, (HWND) hWnd, (int) 0);

			if (! ghWndCap){
				EndDialog(hWnd, TRUE);
				break;
			}
			/* ògópPen & Brush */
			hPenBlack = CreatePen(PS_SOLID, 0, RGB(100,100,100));
			hPenLightGray = CreatePen(PS_SOLID, 0, RGB(224,224,224));
			hBrushGray = GetStockObject(GRAY_BRUSH);
			hBrushLightGray = GetStockObject(LTGRAY_BRUSH);

            // Try to connect one of the MSVIDEO drivers
            for (wIndex = 0 ; wIndex < MAXVIDDRIVERS ; wIndex++) {
                if (capGetDriverDescription(wIndex,
                           (LPSTR)achDeviceName, sizeof(achDeviceName),
                           (LPSTR)achDeviceVersion, sizeof(achDeviceVersion))) {

                    if (wDriverCount++ == 0) {
                        // Only if no other driver is already connected
                        if (wError = capDriverConnect(ghWndCap, wIndex)) {
                            gwDeviceIndex = wIndex ;
                        }
                    }
                } // end of if (capGetDriverDesc..())
            }


			/*{
				BITMAPINFOHEADER bih;

				bih.biSize            = sizeof(BITMAPINFOHEADER);
				bih.biCompression     = BI_RGB;
				bih.biWidth           = 640;
				bih.biHeight          = 480;
				bih.biPlanes          = 1;
				bih.biBitCount        = 24;
				bih.biSizeImage       = bih.biWidth * bih.biHeight *
										bih.biPlanes * bih.biBitCount / 8;
				bih.biXPelsPerMeter   = 0;
				bih.biYPelsPerMeter   = 0;
				bih.biClrUsed         = 0;
				bih.biClrImportant    = 0;


				capSetVideoFormat(ghWndCap, &bih, sizeof (bih));
			}*/

            // Now refresh menu, position capture window, start driver etc
			chgSize(hWnd, ghWndCap);
            StartNewVideoChannel(hWnd, ghWndCap, gwDeviceIndex) ;

			bCapRequest = FALSE;
            capSetCallbackOnFrame(ghWndCap, FrameCallbackProc) ;

			/* èâä˙Ç…ê›íËÇµÇƒÇµÇ‹Ç§ */
//			capPaletteAuto(ghWndCap, 1, 256) ;
        }
			break;
        case WM_SIZE:
        case WM_MOVE:
            CenterCaptureWindow(hWnd, ghWndCap) ;
            break ;

        case WM_PALETTECHANGED:
        case WM_QUERYNEWPALETTE:
            // Pass the buck to Capture window proc
            SendMessage(ghWndCap, Message, wParam, lParam) ;
            break ;

        case WM_PAINT:
        {
            HDC           hDC ;
            PAINTSTRUCT   ps ;

            hDC = BeginPaint(hWnd, &ps) ;

            // Included in case the background is not a pure color
            SetBkMode(hDC, TRANSPARENT) ;

            EndPaint(hWnd, &ps) ;
			PaintWindow(hWnd, ghWndCap);
            break ;
        }
        case WM_QUERYENDSESSION:        // Don't allow endsession if dialog
            if (fHaveDialogUp) {        //   is up.
                MessageBox(NULL, "Close dialogs before exiting Windows.",
                                gachAppName,
                                MB_OK | MB_ICONEXCLAMATION) ;
                BringWindowToTop (hWnd);
                return FALSE;
            }
            else
                return TRUE;
            break ;
		case WM_DESTROY:
            capSetCallbackOnFrame(ghWndCap, NULL) ;
	    	DeleteObject(hPenLightGray);
	    	DeleteObject(hPenBlack);
			break;
    }

    return 0L;
}   // End of CapProc


static	long GetAviMenuProc(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    WORD         wIndex ;
    BOOL         fResult, rt ;
	char		achBuffer[_MAX_PATH];

    capGetStatus(ghWndCap, &gCapStatus, sizeof(CAPSTATUS)) ;

    // If yielding during capture, don't allow other messages
    if (gCapStatus.fCapturingNow) {
        MessageBeep ((UINT) -1);
        return FALSE;
    }

    switch (wParam) {
        case IDM_F_ALLOCATESPACE:
            fHaveDialogUp = TRUE;
            if (DialogBox(ghInst, MAKEINTRESOURCE(IDD_AllocCapFileSpace),
                          hWnd, (DLGPROC)AllocCapFileProc))
                // If user has hit OK then alloc requested capture file space
                if (! capFileAlloc(ghWndCap, (long) gwCapFileSize * ONEMEG))
                    MessageBox(NULL, "Can't pre-allocate capture file space",
                               "Error",
#ifdef BIDI
                                MB_RTL_READING |
#endif
                                MB_OK | MB_ICONEXCLAMATION) ;
            fHaveDialogUp = FALSE;
            break ;

        case IDM_E_COPY:
            capEditCopy(ghWndCap) ;
            break;

        case IDM_E_PASTEPALETTE:
            capPalettePaste(ghWndCap) ;
            break;

        case IDM_O_VIDEOFORMAT:
            if (gCapDriverCaps.fHasDlgVideoFormat) {
                // Only if the driver has a "Video Format" dialog box
                fHaveDialogUp = TRUE;
                if (capDlgVideoFormat(ghWndCap)) {  // If successful,
                    // Get the new image dimension and center capture window
                    capGetStatus(ghWndCap, &gCapStatus, sizeof(CAPSTATUS)) ;
                    SetWindowPos(ghWndCap, NULL, 0, 0, gCapStatus.uiImageWidth,
                        gCapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE) ;
                    CenterCaptureWindow(hWnd, ghWndCap) ;
                }
                fHaveDialogUp = FALSE;
            }
            break;

        case IDM_O_CHOOSECOMPRESSOR:
            fHaveDialogUp = TRUE;
            capDlgVideoCompression(ghWndCap);
            fHaveDialogUp = FALSE;
            break;

        case IDOK :
            // Now refresh menu, position capture window, start driver etc
			chgSize(hWnd, ghWndCap);
			PaintWindow(hWnd, ghWndCap);

            // Capture video sequence
            gdwFrameNum = 0 ;  // Reset single frame counter
            gdwVideoNum = 0 ;  // Reset video stream counter

			/* ò^âÊ */
            fResult = capCaptureSequence(ghWndCap) ;
			/* no break */

        case IDCANCEL:
			if (wParam == IDOK)
				rt = TRUE;
			else
				rt = FALSE;
#if ENABLE_ERROR_CALLBACK
            capSetCallbackOnError(ghWndCap, NULL) ;
#endif
#if ENABLE_STATUS_CALLBACK
            capSetCallbackOnStatus(ghWndCap, NULL) ;
#endif
#if ENABLE_VIDEOFRAME_CALLBACKS
#endif
#if ENABLE_VIDEOSTREAM_CALLBACKS
            capSetCallbackOnVideoStream(ghWndCap, NULL) ;
#endif
	    	DeleteObject(hPenLightGray);
	    	DeleteObject(hPenBlack);

            DestroyWindow(ghWndCap) ;
			EndDialog(hWnd, rt) ;
			return TRUE ;

        case IDM_O_VIDEOSOURCE:
            if (gCapDriverCaps.fHasDlgVideoSource) {
                // Only if the driver has a "Video Source" dialog box
                fHaveDialogUp = TRUE;
                capDlgVideoSource(ghWndCap) ;
                fHaveDialogUp = FALSE;
            }
            break ;

        case IDM_C_CAPTURESETTINGS:
            // Get the current setup for video capture
            capCaptureGetSetup(ghWndCap, &gCapParms, sizeof(CAPTUREPARMS)) ;

            // Invoke a Dlg box to setup all the params
            fHaveDialogUp = TRUE;
            if (DialogBox(ghInst, MAKEINTRESOURCE(IDD_CapSetUp), hWnd, (DLGPROC)CapSetUpProc)) {
                // If the user has hit OK, set the new setup info

                capCaptureSetSetup(ghWndCap, &gCapParms, sizeof(CAPTUREPARMS)) ;
            }
            fHaveDialogUp = FALSE;
            break;
		case IDC_COLOR:
            StartNewVideoChannel(hWnd, ghWndCap, gwDeviceIndex) ;
			capPaletteAuto(ghWndCap, 1, 256) ;
			break;

        default:
            // There is a chance, a driver change has been requested
            if ( IsDriverIndex(wParam) ) {
                // If it's a valid driver index...
                if (wParam - IDM_O_DRIVERS != gwDeviceIndex) {
                    // and a different one too then we need to do the rest

                    // Turn off preview/overlay, uncheck current driver option
                    capPreview(ghWndCap, FALSE) ;
                    capOverlay(ghWndCap, FALSE) ;

                    // Connect to requested driver
                    if ( capDriverConnect(ghWndCap, (wIndex = (WORD)wParam - IDM_O_DRIVERS)) ) {
                        // Connect worked fine -- update menu, start new driver...
                        gwDeviceIndex = (WORD)wParam - IDM_O_DRIVERS ;
                        CenterCaptureWindow(hWnd, ghWndCap) ;
                    }
                    else {
                        // if connect failed, re-connect back to previous driver
                        if (! capDriverConnect(ghWndCap, gwDeviceIndex)) {
                            MessageBox(hWnd, "Now can't connect back to previous driver !!",
                                       "Error",
                            MB_OK | MB_ICONSTOP) ;
                            return -1L ;
                        }
                        else
                            // Re-start previous driver as it was before
                            StartNewVideoChannel(hWnd, ghWndCap, gwDeviceIndex) ;
                            CenterCaptureWindow(hWnd, ghWndCap) ;
                    }
                }   // end of if ( != gwDeviceIndex)
            }   // end of if (IsDriverIndex())
            else {
                wsprintf(achBuffer, "How could you specify this (%u) Driver Index ?",
                         wParam - IDM_O_DRIVERS) ;
                MessageBox(hWnd, achBuffer, "Oops!!",
                MB_OK | MB_ICONEXCLAMATION) ;
            }

            break ;
    }

    return 0L ;
}

int APIENTRY GetAviProc(HWND hWnd, UINT Message, UINT wParam, LONG lParam)
{
    switch (Message) {
        case WM_COMMAND:
            GetAviMenuProc(hWnd, wParam, lParam) ;
            break ;
        case WM_INITDIALOG:
        {
            char    achDeviceName[80] ;
            char    achDeviceVersion[100] ;
            WORD    wDriverCount = 0 ;
            WORD    wIndex ;
            WORD    wError ;

			ghWnd = hWnd;
            // First create the capture window
            ghWndCap = capCreateCaptureWindow((LPSTR)"Capture Window",
                             WS_CHILD | WS_VISIBLE,
                             0, 0, 160, 120, (HWND) hWnd, (int) 0);

#if ENABLE_ERROR_CALLBACK
            // Register the status and error callbacks before driver connect
            capSetCallbackOnError(ghWndCap, ErrorCallbackProc) ;
#endif
#if ENABLE_STATUS_CALLBACK
            capSetCallbackOnStatus(ghWndCap, StatusCallbackProc) ;
#endif
#if ENABLE_VIDEOFRAME_CALLBACKS
            capSetCallbackOnFrame(ghWndCap, FrameCallbackProc) ;
#endif
#if ENABLE_VIDEOSTREAM_CALLBACKS
            capSetCallbackOnVideoStream(ghWndCap, VideoCallbackProc) ;
#endif
			/* ògópPen & Brush */
			hPenBlack = CreatePen(PS_SOLID, 0, RGB(100,100,100));
			hPenLightGray = CreatePen(PS_SOLID, 0, RGB(224,224,224));
			hBrushGray = GetStockObject(GRAY_BRUSH);
			hBrushLightGray = GetStockObject(LTGRAY_BRUSH);

            // Try to connect one of the MSVIDEO drivers
            for (wIndex = 0 ; wIndex < MAXVIDDRIVERS ; wIndex++) {
                if (capGetDriverDescription(wIndex,
                           (LPSTR)achDeviceName, sizeof(achDeviceName),
                           (LPSTR)achDeviceVersion, sizeof(achDeviceVersion))) {

                    if (wDriverCount++ == 0) {
                        // Only if no other driver is already connected
                        if (wError = capDriverConnect(ghWndCap, wIndex)) {
                            gwDeviceIndex = wIndex ;
                        }
                    }
                } // end of if (capGetDriverDesc..())
            }

			/* ÉLÉÉÉvÉ`ÉÉÉtÉ@ÉCÉãê›íË */
			capFileSetCaptureFile(ghWndCap, (LPSTR)gfilename);
			/* óÃàÊÇPÇlÉoÉCÉg */
			capFileAlloc(ghWndCap, (long) 30L * ONEMEG);
			/* ÉLÉÉÉvÉ`ÉÉê›íËÅ@ÇRÇOÉtÉåÅ[ÉÄÅ^ïbÇ…ê›íË */
			capCaptureGetSetup (ghWndCap, &gCapParms, sizeof (CAPTUREPARMS));
			gCapParms.dwRequestMicroSecPerFrame = (DWORD) (1.0E6 / 30);
//			gCapParms.fYield = TRUE;

			capCaptureSetSetup (ghWndCap, &gCapParms, sizeof (CAPTUREPARMS));


			{
				/* èâä˙ÇÕÇPÅDÇUÇlêFÅAò^âÊéûÇÕÇQÇTÇUêF */
				BITMAPINFOHEADER bih;

				bih.biSize            = sizeof(BITMAPINFOHEADER);
				bih.biCompression     = BI_RGB;
				bih.biWidth           = 640;
				bih.biHeight          = 480;
				bih.biPlanes          = 1;
				bih.biBitCount        = 24;
				bih.biSizeImage       = bih.biWidth * bih.biHeight *
										bih.biPlanes * bih.biBitCount / 8;
				bih.biXPelsPerMeter   = 0;
				bih.biYPelsPerMeter   = 0;
				bih.biClrUsed         = 0;
				bih.biClrImportant    = 0;


				capSetVideoFormat(ghWndCap, &bih, sizeof (bih));
			}

            // Now refresh menu, position capture window, start driver etc
			chgSize(hWnd, ghWndCap);
            StartNewVideoChannel(hWnd, ghWndCap, gwDeviceIndex) ;

			/* èâä˙Ç…ê›íËÇµÇƒÇµÇ‹Ç§ */
//			capPaletteAuto(ghWndCap, 1, 256) ;
        }
			break;
        case WM_MOVE:
        case WM_SIZE:
            CenterCaptureWindow(hWnd, ghWndCap) ;
            break ;

        case WM_PALETTECHANGED:
        case WM_QUERYNEWPALETTE:
            // Pass the buck to Capture window proc
            SendMessage(ghWndCap, Message, wParam, lParam) ;
            break ;

        case WM_PAINT:
        {
            HDC           hDC ;
            PAINTSTRUCT   ps ;

            hDC = BeginPaint(hWnd, &ps) ;

            // Included in case the background is not a pure color
            SetBkMode(hDC, TRANSPARENT) ;

            EndPaint(hWnd, &ps) ;
			PaintWindow(hWnd, ghWndCap);
            break ;
        }
        case WM_QUERYENDSESSION:        // Don't allow endsession if dialog
            if (fHaveDialogUp) {        //   is up.
                MessageBox(NULL, "Close dialogs before exiting Windows.",
                                gachAppName,
                                MB_OK | MB_ICONEXCLAMATION) ;
                BringWindowToTop (hWnd);
                return FALSE;
            }
            else
                return TRUE;
            break ;
    }

    return 0L;
}   // End of GetAviProc

static HANDLE GetDIB(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	char bmi[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
	HANDLE hDIB;
	LPSTR pDIB;
	int nHeaderSize;

	capGetVideoFormat(hWnd, bmi, sizeof(bmi));
	nHeaderSize = sizeof(BITMAPINFOHEADER) + GetDibNumColors(bmi) * sizeof(RGBQUAD);
	if ((hDIB = GlobalAlloc(GHND, nHeaderSize + lpVHdr->dwBytesUsed)) == NULL)
		return NULL;

	pDIB = GlobalLock(hDIB);
	memcpy(pDIB, bmi, nHeaderSize);
	memcpy(pDIB + nHeaderSize, lpVHdr->lpData, lpVHdr->dwBytesUsed);
	GlobalUnlock(hDIB);

	return hDIB;
}
