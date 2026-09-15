/****************************************************************************
 |
 |   			TWAIN‘Î‰@½·¬Å°‰æ‘œæ‚è‚İ‚c‚k‚k
 |
 |			Ì§²Ù–¼ :		dca_acq.c
 |			ì¬Ò :		m.hama
 |			“ú•tF			96/09/03
 |
 |
 ***************************************************************************/
#include <windows.h>			// Req. for twain.h type defs and ...
#include <memory.h>
#include <stdio.h>          // needed for sprintf
//#include <toolhelp.h>
#include "twain.h"				// needed for TWAIN definitions
#include "code.h"
#include "extern.h"
#include "data.h"
#include "scan.h"

#define StartWait() hcurSave = SetCursor(LoadCursor(NULL,IDC_WAIT))
#define EndWait()   SetCursor(hcurSave)

static	int	EventQuit;
static   HCURSOR hcurSave;

/*
----------------------------------------------------------------------
							F u n c t i o n s
----------------------------------------------------------------------
*/
static void DoNativeTransfer(HWND hWnd);

/*************************************************************************
 * FUNCTION: TWAcquire
 *
 * ARGS:	 None
 * RETURNS: None
 *
 * NOTES: - opendsm, open the Source Manager
 *			 - opends, open the Source
 *			 - enable Source
 *			 - wait for a message from Source (usually XFERREADY)
 *		Flag =	0 Do Not Accept MSG_XFERREADY
 *				1 Disable/CloseDS/CloseDSM
 *				2 Disable Only
 *				3 Do Not Disable - only if ShowUI=TRUE

 */
void	TWAcquire (HWND hWnd, BOOL Show)
{
	TW_INT16	result=FALSE;

	EventQuit = 0;
	if (TWOpenDSM () == TRUE)
	{
		StartWait();
		// Please note that another Source may change the system default while
		// your not looking and simply getting the default will not guarentee
		// you get what you want.  Suggest saving the dsID structure privately
		// after you open it to assure subsequent connections are to your
		// intended Source -- ed
		// Also note the the DSM will maintain a static list of all Sources in
		// the system at the moment it is opened.  Changes in available Sources
		// while the DSM is open may cause unpredictable results. -- ed
		if (TWOpenDS() == TRUE)
		{
			if (TWXferMech (hWnd) == TWRC_SUCCESS)
			{
				if (TWAutofeedMenu (hWnd) == TWRC_SUCCESS)
				{
					if (!TWIsDSEnabled())
					{
						result = TWEnableDS ((TW_BOOL)Show);

					}
				}
			}
		}

		EndWait();

		if (result != FALSE){
				MSG oMsg;
			 	for (;;){
					if (EventQuit)
						break;
					while (PeekMessage(&oMsg, NULL, 0, 0, PM_REMOVE)) {
					    if ((!TWIsDSOpen()) || (!ProcessTWMessage ((LPMSG)&oMsg, hWnd))){
							TranslateMessage(&oMsg);
							DispatchMessage(&oMsg);
						}
						if (EventQuit)
							break;
					}
				}
		}
//MessageBox(hWnd, "QUIT LOOP", "AAA", MB_OK);
		if (TWIsDSEnabled())
			TWDisableDS();
		if (TWIsDSOpen())
			TWCloseDS();
		if (TWIsDSMOpen())
			TWCloseDSM();

	} //If DSM Open
}	/* TW Acquire */

/*************************************************************************
 * FUNCTION: ProcessTWMessage
 *
 * ARGS:	 lpMsg  Pointer to Windows msg retrieved by GetMessage
 *			 hWnd	Application's main window handle
 *
 * RETURNS: TRUE  if application should process message as usual
 *			 	FALSE if application should skip processing of this message
 *
 * NOTES:	1). be sure both Source Manager and Source are open
 *			 	2). two way message traffic:
 *					- relay windows messages down to Source's modeless dialog
 *					- retrieve TWAIN messages from the Source
 *
 * COMMENT: ProcessSourceMessage is designed for applications that can only
 * have one Source open at a time.  If you wish your application to have more
 * than one Source open at a time please consult the TWAIN ToolKit for
 * event handling instructions.
 */
BOOL ProcessTWMessage(LPMSG lpMsg, HWND hWnd)
{
	 TW_UINT16	twRC=TWRC_NOTDSEVENT;
	 TW_EVENT	twEvent;


	 // Only ask Source Manager to process event if there is a Source connected.

//	 Debug ("ProcessTWMessage");

	 if ((TWIsDSMOpen()) && (TWIsDSOpen()))
	 {
			/* A Source provides a modeless dialog box as its user interface.
			* The following call relays Windows messages down to the Source's
			* UI that were intended for its dialog box.  It also retrieves TWAIN
			* messages sent from the Source to our  Application.
			*/

			twEvent.pEvent = (TW_MEMREF)lpMsg;
			twRC = (*lpDSM_Entry)(&appID,
										&dsID,
										DG_CONTROL,
										DAT_EVENT,
										MSG_PROCESSEVENT,
										(TW_MEMREF)&twEvent);

			switch (twEvent.TWMessage)
			{
			case MSG_XFERREADY:
				TWTransferImage(hWnd);
				if (mode == 2)
					/*SetForegroundWindow(lpMsg->hwnd)*/;
				else
					EventQuit = 1;
//MessageBox(hWnd, "MSG_XFERREADY -> EventQuit", "AAA", MB_OK);
				break;

			case MSG_CLOSEDSREQ:
			{
				EventQuit = 1;
//MessageBox(hWnd, "MSG_CLOSEDSREQ -> EventQuit", "AAA", MB_OK);
				if (TWIsDSEnabled())
					TWDisableDS();
				if (TWIsDSOpen())
					TWCloseDS();
				if (TWIsDSMOpen())
					TWCloseDSM();
			}
			break;

// No message from the Source to the App break;
// possible new message
			case MSG_NULL:
				break;
			default:
				break;
			}	/* switch */
	 } /* if */
					//SetForegroundWindow(lpMsg->hwnd);

	 // tell the caller what happened
	 return (twRC==TWRC_DSEVENT);			// returns TRUE or FALSE

} // ProcessTWMessage


/*************************************************************************
 * FUNCTION: TWTransferImage
 *
 * ARGS:	 hWnd
 *
 * RETURNS: none
 *
 * NOTES:	1). delete any bit maps laying around
 *			 2). mention those who do not want Native need CAP nego. ICAP_XFERMECH
 *			 3). get a little information about image, for form, I do not use it
 *			 4). set up a for form loop to pull image(s) from the Source
 *			 5). call for GetCompleteImage from Source
 *			 6). be sure to send a MSG_ENDXFER as a seperator between images
 *			 7). after the images are transfered I like to shut down the Source
 *
 * COMMENTS: Setup for a transfer in the routine called as a response to
 * XFERREADY.  Then has a nested loop do/while on the routine which
 * actually pulls in the image or GetCompleteImage.  The GetCompleteImage
 * routine also deals with the cancel, xferdone, success messages from
 * Source.
 */
VOID TWTransferImage(HWND hWnd)
{

	DoNativeTransfer(hWnd);

}	// TWTransferImage


/*---------------------------------------------------------------------------*/
static void DoNativeTransfer(HWND hWnd)

/*
** Parameters:
** Globals:
** Operation:
** Return:
** History:
*/

{
	TW_PENDINGXFERS		twPendingXfer;
	TW_UINT16			twRC;
	TW_UINT16			twRC2;
	HBITMAP				hBitMap;
	HANDLE				hbm_acq;	 // handle to bit map from Source to ret to App

//=====Do until there are no more pending transfers
	// explicitly initialize the our flags
	twPendingXfer.Count = 0;
	do {

		if (MessageLevel() >= ML_INFO)
		{
			ShowImageInfo(hWnd);
			ShowImageLayout(hWnd);
			ShowCapability(hWnd, ICAP_PIXELFLAVOR);
			ShowCapability(hWnd, ICAP_PIXELTYPE);
		}

//=====Initiate Native Transfer
		twRC = (*lpDSM_Entry)(&appID,
									&dsID,
									DG_IMAGE,
									DAT_IMAGENATIVEXFER,
									MSG_GET,
									(TW_MEMREF)&hBitMap);

		switch (twRC)
		{
			case TWRC_XFERDONE:  // Session is in State 7
			//Successful Transfer
				if (MessageLevel() >= ML_FULL)
					ShowRC_CC(hWnd, 0, 0, 0, "TWRC_XFERDONE",
					"DG_IMAGE/DAT_IMAGENATIVEXFER/MSG_GET");
//Doesn't work for NT
//				hbm_acq = (HBITMAP)LOWORD(hBitMap);
				hbm_acq = (HBITMAP)hBitMap;
				if (hbm_acq >= (HANDLE)VALID_HANDLE){
					twRC = (*lpDSM_Entry)(&appID,
									&dsID,
									DG_IMAGE,
									DAT_IMAGEINFO,
									MSG_GET,
									(TW_MEMREF)&twImageInfo);

					switch (mode) {
					case 0:
						WriteDibFile(hbm_acq, (LPSTR)Filename);
						GlobalFree((HANDLE)hBitMap);
						break;
					case 1:
						if (hBitmap != NULL)
							GlobalFree(hBitmap);

						hBitmap = (HBITMAP)hBitMap;
						break;
					case 2:
						{
						SCAN_IMAGE_INFO imageInfo;
						imageInfo.XResolution = twImageInfo.XResolution.Whole;
						imageInfo.YResolution = twImageInfo.YResolution.Whole;
						imageInfo.ImageWidth = twImageInfo.ImageWidth;
						imageInfo.ImageLength = twImageInfo.ImageLength;
						imageInfo.BitsPerPixel = twImageInfo.BitsPerPixel;
						SendMessage(hWnd, WM_SCANBMP, (WPARAM)hBitMap, (LPARAM)&imageInfo);
						}
						break;
					}

					WriteDibFlag = TRUE;
				}

				//Acknowledge the end of the transfer
				// and transition to state 6/5
				twRC2 = (*lpDSM_Entry)(&appID,
									&dsID,
									DG_CONTROL,
									DAT_PENDINGXFERS,
									MSG_ENDXFER,
									(TW_MEMREF)&twPendingXfer);
//if (twPendingXfer.Count != 0) {MessageBox(hWnd, "PendingXfer", "AAA", MB_OK);}
//twPendingXfer.Count = 0;
				if (twRC2 != TWRC_SUCCESS)
					if (MessageLevel()  >= ML_ERROR)
						ShowRC_CC(hWnd, 1, twRC2, 1,
						"",
						"DG_CONTROL / DAT_PENDINGXFERS / MSG_ENDXFER");

				if (MessageLevel() >= ML_INFO || AutoFeedOn())
					ShowPendingXfers(hWnd);

			break;

// the user canceled or wants to rescan the image
// something wrong, abort the transfer and delete the image
// pass a null ptr back to App

			case TWRC_CANCEL:	// Session is in State 7
				if (MessageLevel()  >= ML_ERROR)
					ShowRC_CC(hWnd, 0, 0, 0, "TWRC_CANCEL",
					"DG_IMAGE/DAT_IMAGENATIVEXFER/MSG_GET");
				//Source (or User) Canceled Transfer
				// transistion to state 6/5
				twRC2 = (*lpDSM_Entry)(&appID,
									&dsID,
									DG_CONTROL,
									DAT_PENDINGXFERS,
									MSG_ENDXFER,
									(TW_MEMREF)&twPendingXfer);
				if (twRC2 != TWRC_SUCCESS)
					if (MessageLevel()  >= ML_ERROR)
						ShowRC_CC(hWnd, 1, twRC2, 1,
						"",
						"DG_CONTROL / DAT_PENDINGXFERS / MSG_ENDXFER");

				SendMessage(hWnd, PM_XFERDONE, 0, 0);
				if (MessageLevel() >= ML_INFO || AutoFeedOn())
					ShowPendingXfers(hWnd);
			break;

			case TWRC_FAILURE:	//Session is in State 6
			//The transfer failed
				if (MessageLevel()  >= ML_ERROR)
					ShowRC_CC(hWnd, 1, TWRC_FAILURE, 1,
					"", "DG_IMAGE/DAT_IMAGENATIVEXFER/MSG_GET");

				// Abort the image
				// Enhancement: Check Condition Code and attempt recovery
				twRC2 = (*lpDSM_Entry)(&appID,
									&dsID,
									DG_CONTROL,
									DAT_PENDINGXFERS,
									MSG_ENDXFER,
									(TW_MEMREF)&twPendingXfer);
				if (twRC2 != TWRC_SUCCESS)
					if (MessageLevel()  >= ML_ERROR)
						ShowRC_CC(hWnd, 1, twRC2, 1,
						"",
						"DG_CONTROL / DAT_PENDINGXFERS / MSG_ENDXFER");

				if (MessageLevel() >= ML_INFO || AutoFeedOn())
					ShowPendingXfers(hWnd);
			break;
			default:	//Sources should never return any other RC
				if (MessageLevel()  >= ML_ERROR)
					ShowRC_CC(hWnd, 0, 0, 0, "Unknown Return Code",
					"DG_IMAGE/DAT_IMAGENATIVEXFER/MSG_GET");
				// Abort the image
				// Enhancement: Check Condition Code and attempt recovery instead
				twRC2 = (*lpDSM_Entry)(&appID,
									&dsID,
									DG_CONTROL,
									DAT_PENDINGXFERS,
									MSG_ENDXFER,
									(TW_MEMREF)&twPendingXfer);

				if (twRC2 != TWRC_SUCCESS)
					if (MessageLevel()  >= ML_ERROR)
						ShowRC_CC(hWnd, 1, twRC2, 1,
						"",
						"DG_CONTROL / DAT_PENDINGXFERS / MSG_ENDXFER");

				if (MessageLevel() >= ML_INFO || AutoFeedOn())
					ShowPendingXfers(hWnd);
				break;
		}	/* switch */

	// The count of images pending was obtained in the
	// DG_CONTROL / DAT_PENDINGXFERS / MSG_ENDXFER call

	} while (twPendingXfer.Count != 0);
//MessageBox(hWnd, "Return DoNativeTransfer", "AAA", MB_OK);
}	/* DoNativeTransfer */
