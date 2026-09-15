#define VIEWMODE_PICT		0
#define VIEWMODE_ICON		1
#define VIEWMODE_LIST		2

#define AUTOSIZE_NONE		0
#define AUTOSIZE_WIDTH		1
#define AUTOSIZE_HEIGHT		2
#define AUTOSIZE_BOTH		3

#define VIEWWND_PICT		0
#define VIEWWND_LIST		1

#define VIEWHINT_SETFONT	1

#define PICT_WIDTH			128
#define PICT_HEIGHT			128
#define PICT_FRAME			5
#define PICT_TEXT			30
#define PICT_MARGIN			20

#define DETAIL_NAME			0
#define DETAIL_CODE			1
#define DETAIL_CDATE		2
#define DETAIL_UDATE		3
#define DETAIL_CREATOR		4
#define DETAIL_REMARK1		5
#define DETAIL_REMARK2		6
#define DETAIL_NUM			7

#define IMAGE_ROOT			0
#define IMAGE_LIBRARY		1
#define IMAGE_ERROR			2
#define IMAGE_GFOLDER		3
#define IMAGE_GFOLDER_OPEN	4
#define IMAGE_DFOLDER		5
#define IMAGE_DFOLDER_OPEN	6
#define IMAGE_FFOLDER		7
#define IMAGE_FFOLDER_OPEN	8
#define IMAGE_FDATA			9
#define IMAGE_FDATA_OPEN	10
#define IMAGE_DATA			11

extern const char g_sSectionSettings[];
extern const char g_sSectionMainWindow[];
extern const char g_sSectionDetails[];
extern CString g_sStatusInfo;
extern CString g_sPageInfo;
extern CImageList g_ImageList;
extern CImageList g_ImageList2;
extern DEVMODE *g_pPrintDevMode;
extern CRect g_rcPrintMargin;

extern void FormatDate(CString &strIn, CString &strOut);
extern HANDLE CopyHandle(HANDLE h);
extern void PrintPicture(CDC *pDC, HDIB hDIB, CRect rcMargin);
extern HDIB MakeSmallPicture(HDIB hDIB1, int nWidth, int nHeight);
extern int CompareTitle(const CString &sStr1, const CString &sStr2);
extern CString &GetString(UINT nID, int nIndex = 0);
