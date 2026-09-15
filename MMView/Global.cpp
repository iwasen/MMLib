#include "stdafx.h"
#include "MMView.h"

const char g_sSectionSettings[] = "Settings";
const char g_sSectionMainWindow[] = "MainWindow";
const char g_sSectionDetails[] = "Details";
CString g_sStatusInfo;
CString g_sPageInfo;
CImageList g_ImageList;
CImageList g_ImageList2;
DEVMODE *g_pPrintDevMode;
CRect g_rcPrintMargin;

void FormatDate(CString &strIn, CString &strOut)
{
	LPCTSTR pDate = strIn;

	strOut.Format("%.4s/%.2s/%.2s %.2s:%.2s",
			pDate, pDate + 4, pDate + 6,
			pDate + 8, pDate + 10);
}

HANDLE CopyHandle(HANDLE h)
{
	BYTE *lpCopy;
	BYTE *lp;
	HANDLE     hCopy;
	DWORD      dwLen;

	if (h == NULL)
		return NULL;

	dwLen = (DWORD)::GlobalSize((HGLOBAL) h);

	if ((hCopy = (HANDLE) ::GlobalAlloc (GHND, dwLen)) != NULL)
	{
		lpCopy = (BYTE *) ::GlobalLock((HGLOBAL) hCopy);
		lp     = (BYTE *) ::GlobalLock((HGLOBAL) h);

		while (dwLen--)
			*lpCopy++ = *lp++;

		::GlobalUnlock((HGLOBAL) hCopy);
		::GlobalUnlock((HGLOBAL) h);
	}

	return hCopy;
}

void PrintPicture(CDC *pDC, HDIB hDIB, CRect rcMargin)
{
	short nSrcWidth, nSrcHeight, nColor;
	int nDstWidth, nDstHeight;

	if (hDIB == NULL)
		return;

	MMPictureGetInfo(hDIB, &nSrcWidth, &nSrcHeight, &nColor);

	int nMapMode = pDC->SetMapMode(MM_HIMETRIC);
	pDC->LPtoDP(rcMargin);
	pDC->SetMapMode(nMapMode);

	int nDevWidth = pDC->GetDeviceCaps(HORZRES) - (rcMargin.left + rcMargin.right);
	int nDevHeight = pDC->GetDeviceCaps(VERTRES) - (rcMargin.top + rcMargin.bottom);

	nDstWidth = nDevWidth;
	nDstHeight = nSrcHeight * nDevWidth / nSrcWidth;
	if (nDstHeight > nDevHeight) {
		nDstWidth = nSrcWidth * nDevHeight / nSrcHeight;
		nDstHeight = nDevHeight;
	}

	MMPictureDraw(hDIB, pDC->m_hDC, (SHORT)rcMargin.left, (SHORT)-rcMargin.top, (SHORT)nDstWidth, (SHORT)nDstHeight, 0, 0, (SHORT)nSrcWidth, (SHORT)nSrcHeight);
}

HDIB MakeSmallPicture(HDIB hDIB1, int nWidth, int nHeight)
{
	HDIB hDIB2;
	short nWidth1, nHeight1, nColor;

	if (MMPictureGetInfo(hDIB1, &nWidth1, &nHeight1, &nColor) != 0)
		return NULL;

	if (nHeight1 * nWidth / nWidth1 < nHeight)
		nHeight = nHeight1 * nWidth / nWidth1;
	else
		nWidth = nWidth1 * nHeight / nHeight1;

	if (MMPictureCopy(hDIB1, &hDIB2, (SHORT)nWidth, (SHORT)nHeight) != 0)
		return NULL;

	return hDIB2;
}

int CompareTitle(const CString &sStr1, const CString &sStr2)
{
	int n1, n2;
	int n;
	static const char num[] = "0123456789";

	n1 = sStr1.FindOneOf(num);
	n2 = sStr2.FindOneOf(num);

	if (n1 != -1 && n1 == n2) {
		n = sStr1.Left(n1).CompareNoCase(sStr2.Left(n2));
		if (n == 0)
			n = atoi(sStr1.Mid(n1)) - atoi(sStr2.Mid(n2));
		return n;
	} else
		return sStr1.CompareNoCase(sStr2);
}

CString &GetString(UINT nID, int nIndex)
{
	static CString sStr[2];

	sStr[nIndex].LoadString(nID);
	return sStr[nIndex];
}
