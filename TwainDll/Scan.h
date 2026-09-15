/****************************************************************************
 |
 |   			TWAINëŒâûÅ@Ω∑¨≈∞âÊëúéÊÇËçûÇ›ÇcÇkÇk
 |
 |			Ãß≤Ÿñº :		scan.h
 |			çÏê¨é“ :		m.hama
 |			ì˙ïtÅF			96/09/03
 |
 |
 ***************************************************************************/

#define WM_SCANBMP	(WM_USER + 1999)

typedef struct {
	int XResolution;
	int YResolution;
	int	ImageWidth;
	int	ImageLength;
	int	BitsPerPixel;
} SCAN_IMAGE_INFO;

extern	int	APIENTRY ScanDib(HWND, LPSTR);
extern	HANDLE	APIENTRY ScanBmp(HWND hWnd, SCAN_IMAGE_INFO *pImageInfo);
extern	void APIENTRY ScanBmpContinue(HWND hWnd);
extern	BOOL APIENTRY SelectTwainDriver(HWND hWnd);
extern	BOOL APIENTRY GetDefaultDriver(HWND hWnd, TW_IDENTITY *dsID);
