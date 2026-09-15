/****************************************************************************
 |
 |   			TWAINëŒâûÅ@Ω∑¨≈∞âÊëúéÊÇËçûÇ›ÇcÇkÇk
 |
 |			Ãß≤Ÿñº :		extern.h
 |			çÏê¨é“ :		m.hama
 |			ì˙ïtÅF			96/09/03
 |
 |
 ***************************************************************************/
/* tool.c */
extern	TW_INT16 TWXferMech(HWND);
extern	TW_INT16 TWAutofeedMenu(HWND);
extern	BOOL AutoFeedOn(VOID);
extern	float FIX32ToFloat(TW_FIX32);
extern	TW_FIX32 FloatToFIX32(float);
extern	void ShowImageInfo(HWND);
extern	void ShowImageLayout(HWND);
extern	void ShowCapability(HWND, TW_UINT16);
extern	void ShowPendingXfers(HWND);
extern	void ShowRC_CC(HWND, TW_UINT16, TW_UINT16, TW_UINT16, LPSTR, LPSTR);
extern	void ShowTW_ID(HWND, TW_IDENTITY, LPSTR);
extern	void SetMessageLevel(int);
extern	int MessageLevel(VOID);
extern	TW_INT16 	MatchTwainInt(LPSTR, TW_INT32, LPSTR, TW_INT16);
extern	VOID	WriteDibFile(HBITMAP, LPSTR);

/* dca_acq.c */
extern	void	TWAcquire(HWND, BOOL);
extern	BOOL ProcessTWMessage(LPMSG, HWND);
extern	void TWTransferImage(HWND);

/* dca_glue.c */
extern	VOID TWInitialize(pTW_IDENTITY, HWND);

// Routines for DSM
extern	BOOL TWOpenDSM (VOID);
extern	BOOL TWCloseDSM (VOID);
extern	BOOL TWIsDSMOpen (VOID);

// Routines for DS
extern	BOOL TWOpenDS (VOID);
extern	BOOL TWCloseDS (VOID);
extern	BOOL TWIsDSOpen (VOID);

extern	BOOL TWEnableDS (TW_BOOL);
extern	BOOL TWDisableDS (VOID);
extern	BOOL TWIsDSEnabled (VOID);

extern	BOOL TWSelectDS (VOID);
extern	BOOL TWGetDefault (TW_IDENTITY *pDsID);
