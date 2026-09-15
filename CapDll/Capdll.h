/*************************************************************************
 |
 |	キャプチャーライブラリ
 |
 |					for MS-WINDOWS Ver 3.1
 |
 |		capdll.h
 |
 |		Ver 1.00    1996/07/25  HAMA
 |
 ************************************************************************/

#define WM_FRAMEBMP		(WM_USER + 1998)

extern	int	APIENTRY PSCD_GetDib(HWND, LPSTR);
extern	HANDLE	APIENTRY PSCD_GetBmp(HWND);
extern	void	APIENTRY PSCD_GetBmpContinue(HWND);
extern	int APIENTRY PSCD_GetAvi(HWND, LPSTR);
