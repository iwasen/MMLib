// PictView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "PictView.h"
#include "PictDoc.h"
#include "PrintDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CPictView

IMPLEMENT_DYNCREATE(CPictView, CScrollView)

CPictView::CPictView()
{
	m_bIgnoreUpdate = FALSE;
	m_bScrollPicture = FALSE;
	m_hBrushBkgnd.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
}

BEGIN_MESSAGE_MAP(CPictView, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictView 描画

void CPictView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: このビュー全体のサイズを計算してください。
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CPictView::OnDraw(CDC* pDC)
{
	DrawPicture(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CPictView メッセージ ハンドラ

void CPictView::DrawPicture(CDC *pDC)
{
	int nWidthSrc, nHeightSrc;
	int nWidthDst, nHeightDst;
	int nXDst, nYDst;
	CRect rcView;
	CPictDoc *pDoc = (CPictDoc *)GetDocument();

	if (pDoc->m_hDIB == NULL)
		return;

	if (m_bIgnoreUpdate) {
		m_bIgnoreUpdate = FALSE;
		return;
	}

	GetClientRect(rcView);

	short nWidth, nHeight, nColor;
	MMPictureGetInfo(pDoc->m_hDIB, &nWidth, &nHeight, &nColor);
	nWidthSrc = nWidth;
	nHeightSrc = nHeight;

	switch (pDoc->m_nAutoSize) {
	case AUTOSIZE_NONE:
		nWidthDst = (int)(nWidthSrc * pDoc->m_fPictRatio + 0.5);
		nHeightDst = (int)(nHeightSrc * pDoc->m_fPictRatio + 0.5);
		break;
	case AUTOSIZE_WIDTH:
		nWidthDst = rcView.right;
		nHeightDst = nHeightSrc * rcView.right / nWidthSrc;
		pDoc->m_fPictRatio = (double)rcView.right / nWidthSrc;
		break;
	case AUTOSIZE_BOTH:
		nWidthDst = rcView.right;
		nHeightDst = nHeightSrc * rcView.right / nWidthSrc;
		pDoc->m_fPictRatio = (double)rcView.right / nWidthSrc;
		if (nHeightDst > rcView.bottom) {
			nWidthDst = nWidthSrc * rcView.bottom / nHeightSrc;
			nHeightDst = rcView.bottom;
			pDoc->m_fPictRatio = (double)rcView.bottom / nHeightSrc;
		}
		break;
	default:
		return;
	}

	CString sStr;
	CComboBox &comboBox = ((CPictFrame *)GetParent())->GetComboBox();
	sStr.Format("%d%%", (int)(pDoc->m_fPictRatio * 100 + 0.5));
	comboBox.SetWindowText(sStr);
	comboBox.SetEditSel(0, -1);

	nXDst = (rcView.right - nWidthDst) / 2;
	if (nXDst < 0)
		nXDst = 0;

	nYDst = (rcView.bottom - nHeightDst) / 2;
	if (nYDst < 0)
		nYDst = 0;

	if (m_sizeDraw.cx != nWidthDst || m_sizeDraw.cy != nHeightDst) {
		m_sizeDraw.cx = nWidthDst;
		m_sizeDraw.cy = nHeightDst;
		SetScrollSizes(MM_TEXT, m_sizeDraw);

		CRect rcView2;
		GetClientRect(rcView2);
		if (pDoc->m_nAutoSize == AUTOSIZE_WIDTH && rcView.right > rcView2.right) {
			m_sizeDraw.cx = rcView2.right;
			m_sizeDraw.cy = nHeightDst;
			SetScrollSizes(MM_TEXT, m_sizeDraw);
			m_bIgnoreUpdate = TRUE;
			UpdateWindow();
		} else {
			if (rcView.right != rcView2.right || rcView.bottom != rcView2.bottom)
				return;
		}
	}

	MMPictureDraw(pDoc->m_hDIB, pDC->m_hDC, (SHORT)nXDst, (SHORT)nYDst, (SHORT)nWidthDst, (SHORT)nHeightDst, 0, 0, (SHORT)nWidthSrc, (SHORT)nHeightSrc);

	if (!CHECK_LICENSE2(0x01))
		DispTrialVersion(this, pDC);
}

BOOL CPictView::OnEraseBkgnd(CDC* pDC)
{
	if (m_bIgnoreUpdate)
		return TRUE;

	CRect rect;
	pDC->GetClipBox(rect);
	pDC->FillRect(rect, &m_hBrushBkgnd);
	return TRUE;
}

void CPictView::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	m_bScrollPicture = TRUE;
	CPoint scrollPos = GetScrollPosition();
	m_posPicture = (CPoint)(scrollPos + point);
	::SetCursor(theApp.m_hCursorHandClose);

	SetCapture();
}

void CPictView::OnLButtonUp(UINT /*nFlags*/, CPoint /*point*/)
{
	if (GetCapture() != this)
		return;

	ReleaseCapture();
	m_bScrollPicture = FALSE;
	::SetCursor(theApp.m_hCursorHandOpen);
}

void CPictView::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	if (m_bScrollPicture) {
		CPoint scrollPos = m_posPicture - point;

		CRect rectView;
		GetClientRect(rectView);
		if (rectView.right >= m_sizeDraw.cx)
			scrollPos.x = 0;
		if (rectView.bottom >= m_sizeDraw.cy)
			scrollPos.y = 0;
		ScrollToPosition(scrollPos);
		::SetCursor(theApp.m_hCursorHandClose);
	} else
		::SetCursor(theApp.m_hCursorHandOpen);
}

BOOL CPictView::OnPreparePrinting(CPrintInfo* pInfo)
{
	delete pInfo->m_pPD;
	pInfo->m_pPD = new CPrintDlg(FALSE, PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOSELECTION, NULL);

	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(1);

	return DoPreparePrinting(pInfo);
}

void CPictView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (g_pPrintDevMode != NULL)
		pDC->ResetDC(g_pPrintDevMode);

	CScrollView::OnBeginPrinting(pDC, pInfo);
}

void CPictView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	CPictDoc *pDoc = (CPictDoc *)GetDocument();

	PrintPicture(pDC, pDoc->m_hDIB, g_bPrintMargin ? g_rcPrintMargin : CRect(0, 0, 0, 0));
}
