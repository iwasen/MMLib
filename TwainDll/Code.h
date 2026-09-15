/****************************************************************************
 |
 |   			TWAIN‘Î‰ž@½·¬Å°‰æ‘œŽæ‚èž‚Ý‚c‚k‚k
 |
 |			Ì§²Ù–¼ :		code.h
 |			ì¬ŽÒ :		m.hama
 |			“ú•tF			96/09/03
 |
 |
 ***************************************************************************/
#ifdef _WIN64
#define	DSNAME	(LPSTR)"TWAINDSM.DLL"
#else
#define	DSNAME	(LPSTR)"TWAIN_32.DLL"
#endif

// Generic access to TWAIN DSM
#define  IDS_DSMNAME        0
#define  IDS_WINDOWTITLE    1

// Resource based standard App error messages
#define IDS_NODSM           16
#define IDS_NODS            17
#define IDS_LOWMEMORY       18
#define IDS_MAXCONNECTIONS  19
#define IDS_BUMMER          20
#define IDS_BADCAP          21
#define IDS_BADPROTOCOL     22
#define IDS_BADVALUE        23
#define IDS_OPERATIONERROR  24
#define IDS_SEQERROR        25
#define IDS_SUCCESS         26
#define IDS_NODETAIL        27
#define IDS_DEFAULT         28
#define IDS_BADDEST			29

#define MM_FILE				0
#define MM_SPECIAL			1
#define MM_DG               2
#define MM_DAT              3
#define MM_MSG              4
#define MM_SEND				5
#define MM_HELP				6

#define TW_APP_ACQUIRE	      	100
#define TW_APP_SELECT_SOURCE	101
#define TW_APP_NATIVE			110
#define TW_APP_FILE           	111
#define TW_APP_BUFFER			112
#define TW_APP_SHOW				120
#define TW_APP_DISABLE_AFTER	121
#define TW_APP_QUIT	        	130

#define TW_APP_MSG_NONE			150
#define TW_APP_MSG_ERROR		151
#define TW_APP_MSG_INFO			152
#define TW_APP_MSG_FULL			153


#define TW_APP_AUTOFEED			160

#define TW_APP_RUN_TEST			180

#define TW_APP_OPENSM			201
#define TW_APP_OPENDS			202
#define TW_APP_SEND				203
#define TW_APP_ENABLE           204
#define TW_APP_TRANSFER			205
#define TW_APP_DISABLE			206
#define TW_APP_CLOSEDS			207
#define TW_APP_CLOSESM			208

#define TW_APP_ABOUT	        700

#define TW_APP_ABOUT_CO	    	120
#define TW_APP_ABOUT_VER	    121
#define TW_APP_ABOUT_COPYRIGHT 	122
#define TW_APP_ABOUT_OKAY	    123

#define DLG_SEND				100
#define IDDG					101
#define IDDAT					102
#define IDMSG					103
#define IDCAPTEXT				104
#define IDCAP					105
#define IDTYPETEXT				106
#define IDTYPE					107
#define IDEDITTEXT				108
#define IDEDIT					109
#define IDSTATUS				110
#define	IDDEST					111

#define  PM_XFERDONE           	WM_USER + 0

#define ML_NONE 	0
#define ML_ERROR	1
#define ML_INFO		2
#define ML_FULL 	3

#define E_CLOSEDSM          (LPSTR)"Error Closing DSM.  "
#define E_CLOSEDS           (LPSTR)"Error Closing DS.  "
#define E_USERSELECT        (LPSTR)"Error Accessing DS.  "
#define E_SETUPMEMXFER      (LPSTR)"Error Setting up memory transfer.  "
#define E_DISABLEDS         (LPSTR)"Error Disabling DS.  "
#define E_ENABLEDS          (LPSTR)"Error Enabling DS.  "
#define E_GETFIRST          (LPSTR)"Error Getting first Data Source.  "
#define E_GETNEXT           (LPSTR)"Error Getting Next Data Source.  "
#define E_CAPPIXELGET       (LPSTR)"Error Getting Cap Pixel Type.  "
#define E_CAPPIXELSET       (LPSTR)"Error Setting Cap Pixel Type.  "
#define E_CAPABILITY		(LPSTR)"Error Setting Capability.  "


/***********************************************************************/
/* Function prototypes from module DCA_GLUE.C */
/***********************************************************************/
// Candy routines

#define VALID_HANDLE    32      // valid windows handle SB >= 32

#define PALVERSION      0x300
#define MAXPALETTE      256      /* max. # supported palette entries */
#define LOWOVERHEAD     100     // different call in main event loop uses flags
