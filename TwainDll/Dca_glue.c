/****************************************************************************
 |
 |   			TWAINëŒâûÅ@Ω∑¨≈∞âÊëúéÊÇËçûÇ›ÇcÇkÇk
 |
 |			Ãß≤Ÿñº :		dca_glue.c
 |			çÏê¨é“ :		m.hama
 |			ì˙ïtÅF			96/09/03
 |
 |
 ***************************************************************************/
#include <windows.h>    // Note: twain.h also REQUIRES windows defs
#include <stdio.h>		// required for sprintf
#include <string.h>		// required for strlen
#include "twain.h"         // for TW data type defines
#include "code.h"
#include "extern.h"
#include "data.h"

// Globals to this module ONLY
static BOOL TWDSMOpen = FALSE;    // glue code flag for an Open Source Manager
static HANDLE hDSMDLL = NULL;     // handle to Source Manager for explicit load
static HWND hMainWnd;                 // global for window handle
static BOOL TWDSOpen  = FALSE;           // glue code flag for an Open Source
static BOOL TWDSEnabled  = FALSE;           // glue code flag for an Open Source

/***********************************************************************
 * FUNCTION: TWInitialize
 *
 * ARGS:    pIdentity information about the App
 *          hMainWnd  handle to App main window
 *
 * RETURNS: none
 *
 * NOTES:   This simple copy to a static structure, appID, does not support
 *          multiple connections. TODO, upgrade.
 *
 */
VOID TWInitialize (pTW_IDENTITY pIdentity, HWND hWnd)
{
    appID = *pIdentity;    // get copy of app info
    hMainWnd = hWnd;       // get copy of app window handle
} // TWInitialize

/***********************************************************************
 * FUNCTION: TWOpenDSM
 *
 * ARGS:    none
 *
 * RETURNS: current state of the Source Manager
 *
 * NOTES:     1). be sure SM is not already open
 *            2). explicitly load the .DLL for the Source Manager
 *            3). call Source Manager to:
 *                - opens/loads the SM
 *                - pass the handle to the app's window to the SM
 *                - get the SM assigned appID.id field
 *
 */
BOOL TWOpenDSM (VOID)
{
    TW_UINT16     twRC;
//    OFSTRUCT      OpenFiles;
    #define       WINDIRPATHSIZE 160
    char          WinDir[WINDIRPATHSIZE];


    // Only open SM if currently closed
    if (TWDSMOpen!=TRUE){

        // Open the SM, Refer explicitly to the library so we can post a nice
        // message to the the user in place of the "Insert TWAIN.DLL in drive A:"
        // posted by Windows if the SM is not found.

		GetWindowsDirectory (WinDir, WINDIRPATHSIZE);
		// Hardcode to seperate dca_main.c & dca_glue.c more completely
		// lstrcpy (DSMName, "TWAIN.DLL");

		if (WinDir[strlen(WinDir)-1] != '\\')
			lstrcat (WinDir, "\\");
		lstrcat (WinDir, DSNAME);


	    if ((hDSMDLL =     LoadLibrary(DSNAME)) != NULL &&
	    	(hDSMDLL >=    (HANDLE)VALID_HANDLE) &&
	    	(lpDSM_Entry = (DSMENTRYPROC)GetProcAddress(hDSMDLL, "DSM_Entry")) != NULL)
	    {

	    	/* This call performs four important functions:
			- opens/loads the SM
                	- passes the handle to the app's window to the SM
                	- returns the SM assigned appID.id field
                	- be sure to test the return code for SUCCESSful open of SM
            	*/
            twRC = (*lpDSM_Entry)(&appID,
                                  NULL,
                                  DG_CONTROL,
                                  DAT_PARENT,
                                  MSG_OPENDSM,
                                  (TW_MEMREF)&hMainWnd);

            switch (twRC)
            {
                case TWRC_SUCCESS:
                // Needed for house keeping.  Do single open and do not
                // close SM which is not already open ....
        	        TWDSMOpen = TRUE;
		    		if (MessageLevel() >= ML_FULL)
			    		ShowRC_CC(hMainWnd, 0, 0, 0,
		    				"Source Manager was Opened successfully",
		    				"TWAIN Information");
	            break;

                case TWRC_FAILURE:
                default:
	                // Trouble opening the SM, inform the user
                	TWDSMOpen = FALSE;
	        		if (MessageLevel() >= ML_ERROR)
	    	            ShowRC_CC(hMainWnd, 1, twRC, 0,	//Source Manager
        	        		"",
            	    		"DG_CONTROL/DAT_PARENT/MSG_OPENDSM");
                break;
            }
        } else {
        	if (MessageLevel() >= ML_ERROR)
                ShowRC_CC(hMainWnd, 0, 0, 0,
                	"Error in Open, LoadLibrary, or GetProcAddress",
                	"TWAIN.DLL");
        }
    }  // end of if open
    // Let the caller know what happened
    return (TWDSMOpen);
} // TWOpenDSM

/***********************************************************************
 * FUNCTION: TWCloseDSM
 *
 * ARGS:    none
 *
 * RETURNS: current state of Source Manager
 *
 * NOTES:    1). be sure SM is already open
 *           2). call Source Manager to:
 *               - request closure of the Source identified by appID info
 *
 */
BOOL TWCloseDSM (VOID)
{
    TW_UINT16 twRC;

	if (!TWDSMOpen)
	{
		if (MessageLevel()  >= ML_ERROR)
			ShowRC_CC(hMainWnd, 0, 0, 0,
				"Cannot Close Source Manager\nSource Manager Not Open",
				"Sequence Error");
	} else {
	    if (TWDSOpen==TRUE)
	    {
	   		if (MessageLevel()  >= ML_ERROR)
			ShowRC_CC(hMainWnd, 0, 0, 0,
				"A Source is Currently Open", "Cannot Close Source Manager");
	    } else {
		    // Only close something which is already open
		    if (TWDSMOpen==TRUE){
		        /* This call performs one important function:
		           - tells the SM which application, appID.id, is requesting SM to close
		           - be sure to test return code, failure indicates SM did not close !!
		        */
		        twRC = (*lpDSM_Entry)(&appID,
		                              NULL,
		                              DG_CONTROL,
		                              DAT_PARENT,
		                              MSG_CLOSEDSM,
		                              &hMainWnd);

		        if (twRC != TWRC_SUCCESS){
		            // Trouble closing the SM, inform the user
		        	if (MessageLevel() >= ML_ERROR)
		                ShowRC_CC(hMainWnd, 1, twRC, 0,
		                	"",
		                	"DG_CONTROL/DAT_PARENT/MSG_CLOSEDSM");
		        } else {
		            TWDSMOpen = FALSE;
		            // Explicitly free the SM library
		            if (hDSMDLL){
		                FreeLibrary (hDSMDLL);
		                hDSMDLL=NULL;
		            }
			    	if (MessageLevel() >= ML_FULL)
			    		ShowRC_CC(hMainWnd, 0, 0, 0,
			    			"Source Manager was Closed successfully",
			    			"TWAIN Information");
		        }
		    }
		}
	}
    // Let the caller know what happened
    return (twRC==TWRC_SUCCESS);
} // TWCloseDSM



/***********************************************************************
 * FUNCTION: TWIsDSMOpen
 *
 * ARGS:    none
 *
 * RETURNS: current state of Source Manager (open/closed)
 *
 * NOTES:   Just a way to reduce the number of global vars and keep the
 *          state of SM information local to this module.  Let the caller,
 *          app, know current state of the SM.
 */
BOOL TWIsDSMOpen (VOID)
{
    return (TWDSMOpen);
} // TWIsDSMOpen



/***********************************************************************
 * FUNCTION: TWOpenDS
 *
 * ARGS:    none
 *
 * RETURNS: current state of select Source
 *
 * NOTES:    1). only attempt to open a source if it is currently closed
 *           2). call Source Manager to:
 *                - open the Source indicated by info in dsID
 *                - SM will fill in the unique .Id field
 */
BOOL TWOpenDS (VOID)
{
    TW_UINT16		twRC=TWRC_FAILURE;

    if (TWDSMOpen==FALSE)
    {
    	if (MessageLevel() >= ML_ERROR)
    		ShowRC_CC(hMainWnd, 0, 0, 0,
 				"Cannot Open Source\nSource Manager not Open",
 				"TWAIN Error");
 	} else {
 		//Source Manager is open
	    if (TWDSOpen==TRUE)
	    {
	       	if (MessageLevel() >= ML_FULL)
	       		ShowRC_CC(hMainWnd, 0, 0, 0,
 					"Source is already open", "TWAIN Information");
 		} else {
		    // This will open the Source.
        	twRC = (*lpDSM_Entry)(&appID,
                              NULL,
                              DG_CONTROL,
                              DAT_IDENTITY,
                              MSG_OPENDS,
                              &dsID);

			switch (twRC)
			{
				case TWRC_SUCCESS:
    	    		// do not change flag unless we successfully open
		            TWDSOpen = TRUE;
		           	if (MessageLevel() >= ML_INFO)
		            	ShowTW_ID(hMainWnd, dsID,
	                		"DG_CONTROL/DAT_IDENTITY/MSG_OPENDS TWRC_SUCCESS");
	            break;

	            case TWRC_FAILURE:
					// Trouble opening the Source
					//Determine Condition Code
					if (MessageLevel()  >= ML_ERROR)
		       			ShowRC_CC(hMainWnd, 1, twRC, 0, //Source Manager RC
					        "",
							"DG_CONTROL/DAT_IDENTITY/MSG_OPENDS");
				break;
	            default:
			    	if (MessageLevel()  >= ML_ERROR)
		       			ShowRC_CC(hMainWnd, 0, 0, 0,
					        "Unknown Return Code",
							"DG_CONTROL/DAT_IDENTITY/MSG_OPENDS");
				break;
			}
		}
	}

    return TWDSOpen;
} // TWOpenDS


/***********************************************************************
 * FUNCTION: TWCloseDS
 *
 * ARGS:    none
 *
 * RETURNS: none
 *
 * NOTES:    1). only attempt to close an open Source
 *           2). call Source Manager to:
 *                - ask that Source identified by info in dsID close itself
 */
BOOL TWCloseDS (VOID)
{
    TW_UINT16         twRC=TWRC_FAILURE;

	if (!TWDSOpen)
	{
		if (MessageLevel()  >= ML_ERROR)
			ShowRC_CC(hMainWnd, 0, 0, 0,
				"Cannot Close Source\nSource Not Open",
				"Sequence Error");
    } else {
	    if (TWDSEnabled == TRUE)
	    {
	   		if (MessageLevel()  >= ML_ERROR)
			ShowRC_CC(hMainWnd, 0, 0, 0,
				"Source is Currently Enabled", "Cannot Close Source");
	    } else {
		    if (TWDSOpen==TRUE){
		        // Close an open Source
		        twRC = (*lpDSM_Entry)(&appID,
		                              NULL,
		                              DG_CONTROL,
		                              DAT_IDENTITY,
		                              MSG_CLOSEDS,
		                              &dsID);
		        // show error on close
		        if (twRC!= TWRC_SUCCESS)
		        {
		        	if (MessageLevel() >= ML_ERROR)
		                ShowRC_CC(hMainWnd, 1, twRC, 0,
		                	"",
		                	"DG_CONTROL/DAT_IDENTITY/MSG_CLOSEDS");
		        } else {
		            TWDSOpen = FALSE;
			        dsID.Id = 0;
			        dsID.ProductName[0] = 0;
			    	if (MessageLevel() >= ML_FULL)
						ShowRC_CC(hMainWnd, 0, 0, 0,
							"Source was Closed successfully",
							"TWAIN Information");
		        }

		    }
		}
	}
    return(twRC==TWRC_SUCCESS);
} // TWCloseDS



/***********************************************************************
 * FUNCTION: TWEnableDS
 *
 * ARGS:    none
 *
 * RETURNS: BOOL for TRUE=open; FALSE=not open/fail
 *
 * NOTES:    1). only enable an open Source
 *           2). call the Source Manager to:
 *                - bring up the Source's User Interface
 */
BOOL TWEnableDS (TW_BOOL Show)
{
    BOOL              Result=FALSE;
    TW_UINT16         twRC=TWRC_FAILURE;
    TW_USERINTERFACE  twUI;

	if (TWDSOpen==FALSE)
	{
    	if (MessageLevel() >= ML_ERROR)
			ShowRC_CC(hMainWnd, 0, 0, 0,
				"Cannot Enable Source\nNo Source Open",
				"TWAIN Error");

	} else {	// only enable open Source's
		if (TWDSEnabled==TRUE)	//Source is alredy enabled
		{
	    	if (MessageLevel() >= ML_ERROR)
				ShowRC_CC(hMainWnd, 0, 0, 0,
					"Cannot Enable Source, already enabled",
					"TWAIN Error");
		} else {
	        // This will display the Source User Interface. The Source should only display
	        // a user interface that is compatible with the group defined
	        // by appID.SupportedGroups (in our case DG_IMAGE | DG_CONTROL)
	        twUI.hParent = hMainWnd;
	        twUI.ShowUI  = Show;
	        twRC = (*lpDSM_Entry)(&appID,
	                              &dsID,
	                              DG_CONTROL,
	                              DAT_USERINTERFACE,
	                              MSG_ENABLEDS,
	                              (TW_MEMREF)&twUI);

	        if (twRC!=TWRC_SUCCESS)
	        {
	           	if (MessageLevel() >= ML_ERROR)
	                ShowRC_CC(hMainWnd, 1, twRC, 1,
	                	"",
	                	"DG_CONTROL/DAT_USERINTERFACE/MSG_ENABLEDS");
			} else {
				Result = TRUE;
				TWDSEnabled = TRUE;
	           	if (MessageLevel() >= ML_FULL)
	                ShowRC_CC(hMainWnd, 0, 0, 0,
	                	"Source Enabled",
	                	"DG_CONTROL/DAT_USERINTERFACE/MSG_ENABLEDS");
			}
		}
    }
    return (Result);
} // TWEnableDS

/***********************************************************************
 * FUNCTION: TWDisableDS
 *
 * ARGS:    none
 *
 * RETURNS: twRC
 *
 * NOTES:    1). only disable an open Source
 *           2). call Source Manager to:
 *                - ask Source to hide it's User Interface
 */
BOOL TWDisableDS (VOID)
{
    TW_UINT16         twRC=TWRC_FAILURE;
    TW_USERINTERFACE  twUI;

    // only disable enabled Source's
    if (TWDSEnabled!=TRUE) {
		if (MessageLevel()  >= ML_ERROR)
			ShowRC_CC(hMainWnd, 0, 0, 0,
				"Cannot Disable Source\nSource Not Enabled",
				"Sequence Error");
    } else {
        twUI.ShowUI  = TWON_DONTCARE8;

        twRC = (*lpDSM_Entry)(&appID,
                              &dsID,
                              DG_CONTROL,
                              DAT_USERINTERFACE,
                              MSG_DISABLEDS,
                              (TW_MEMREF)&twUI);

        if (twRC == TWRC_SUCCESS)
        {
			TWDSEnabled = FALSE;
           	if (MessageLevel() >= ML_FULL)
                ShowRC_CC(hMainWnd, 0, 0, 0,
                	"Source Disabled",
                	"DG_CONTROL/DAT_USERINTERFACE/MSG_DISABLEDS");
		} else {
        	if (MessageLevel() >= ML_ERROR)
                ShowRC_CC(hMainWnd, 1, twRC, 1,
               	"",
               	"DG_CONTROL/DAT_USERINTERFACE/MSG_DISABLEDS");
		}
    }
    return (twRC==TWRC_SUCCESS);
} // TWDisableDS

/***********************************************************************
 * FUNCTION: TWIsDSOpen
 *
 * ARGS:    none
 *
 * RETURNS: current state of the Source
 *
 * NOTES:    Just a way to reduce the number of global vars and keep the
 *           state of Source information local to this module.  Let the caller,
 *           app, know current state of the Source.
 */
BOOL TWIsDSOpen (VOID)
{
    return (TWDSOpen);
} // TWIsDSOpen

/***********************************************************************
 * FUNCTION: TWIsDSEnabled
 *
 * ARGS:    none
 *
 * RETURNS: current state of Source (Enabled?)
 *
 * NOTES:   Just a way to reduce the number of global vars and keep the
 *          state of SM information local to this module.  Let the caller,
 *          app, know current state of the SM.
 */
BOOL TWIsDSEnabled (VOID)
{
    return (TWDSEnabled);
}



/***********************************************************************
 * FUNCTION: TWSelectDS
 *
 * ARGS:    none
 *
 * RETURNS: twRC TWAIN status return code
 *
 * NOTES:   1). call the Source Manager to:
 *              - have the SM put up a list of the available Sources
 *              - get information about the user selected Source from
 *                NewDSIdentity, filled by Source
 */
BOOL TWSelectDS (VOID)
{
    TW_UINT16 twRC;
    TW_IDENTITY NewDSIdentity;

    if (TWDSOpen)
    {
    	//A Source is already open
    	if (MessageLevel() >= ML_ERROR)
            ShowRC_CC(hMainWnd, 0, 0, 0,
               	"A Source is already open\nClose Source before Selecting a New Source",
               	"DG_CONTROL/DAT_IDENTITY/MSG_USERSELECT");
        twRC=TWRC_FAILURE;
    } else {

	    // I will settle for the system default.  Shouldn't I get a highlight
	    // on system default without this call?
	    twRC = (*lpDSM_Entry)(&appID,
	                          NULL,
	                          DG_CONTROL,
	                          DAT_IDENTITY,
	                          MSG_GETDEFAULT,
	                          (TW_MEMREF)&NewDSIdentity);


	    /* This call performs one important function:
	    - should cause SM to put up dialog box of available Source's
	    - tells the SM which application, appID.id, is requesting, REQUIRED
	    - returns the SM assigned NewDSIdentity.id field, you check if changed
	      (needed to talk to a particular Data Source)
	    - be sure to test return code, failure indicates SM did not close !!
	    */
	    twRC = (*lpDSM_Entry)(&appID,
	                          NULL,
	                          DG_CONTROL,
	                          DAT_IDENTITY,
	                          MSG_USERSELECT,
	                          (TW_MEMREF)&NewDSIdentity);

	    /* Check if the user changed the Source and react as apporpriate.
	    - TWRC_SUCCESS, log in new Source
	    - TWRC_CANCEL,  keep the current Source
	    - default,      check down the codes in a status message, display result
	    */

	    switch (twRC){
	        case TWRC_SUCCESS:
	        	dsID = NewDSIdentity;
	           	if (MessageLevel() >= ML_INFO)
		            ShowTW_ID(hMainWnd, dsID,
	                	"DG_CONTROL/DAT_IDENTITY/MSG_USERSELECT");
	        break;
	        case TWRC_CANCEL:
	           	if (MessageLevel() >= ML_INFO)
		            ShowRC_CC(hMainWnd, 1, twRC, 0,
	                	"",
	                	"DG_CONTROL/DAT_IDENTITY/MSG_USERSELECT");
	        break;
			case TWRC_FAILURE:
	        default:
	           	if (MessageLevel() >= ML_ERROR)
		            ShowRC_CC(hMainWnd, 1, twRC, 0,
	                	"",
	                	"DG_CONTROL/DAT_IDENTITY/MSG_USERSELECT");
	        break;
	    }
	}
    // Let the caller know what happened
    return (twRC);
}  // TWSelectDS

BOOL TWGetDefault (TW_IDENTITY *pDsID)
{
    TW_UINT16 twRC;

    if (TWDSOpen)
    {
    	//A Source is already open
    	if (MessageLevel() >= ML_ERROR)
            ShowRC_CC(hMainWnd, 0, 0, 0,
               	"A Source is already open\nClose Source before Selecting a New Source",
               	"DG_CONTROL/DAT_IDENTITY/MSG_USERSELECT");
        twRC=TWRC_FAILURE;
    } else {

	    // I will settle for the system default.  Shouldn't I get a highlight
	    // on system default without this call?
	    twRC = (*lpDSM_Entry)(&appID,
	                          NULL,
	                          DG_CONTROL,
	                          DAT_IDENTITY,
	                          MSG_GETDEFAULT,
	                          (TW_MEMREF)pDsID);



	    /* Check if the user changed the Source and react as apporpriate.
	    - TWRC_SUCCESS, log in new Source
	    - default,      check down the codes in a status message, display result
	    */

	    switch (twRC){
	        case TWRC_SUCCESS:
	           	if (MessageLevel() >= ML_INFO)
		            ShowTW_ID(hMainWnd, dsID,
	                	"DG_CONTROL/DAT_IDENTITY/MSG_USERSELECT");
	        break;
	        default:
	           	if (MessageLevel() >= ML_ERROR)
		            ShowRC_CC(hMainWnd, 1, twRC, 0,
	                	"",
	                	"DG_CONTROL/DAT_IDENTITY/MSG_USERSELECT");
	        break;
	    }
	}
    // Let the caller know what happened
    return (twRC==TWRC_SUCCESS);
}  // TWSelectDS
