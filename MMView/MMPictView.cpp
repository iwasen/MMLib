// MMPictView.cpp : CMMPictView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "MMView.h"
#include "MainFrm.h"
#include "MMViewDoc.h"
#include "MMPictView.h"

/////////////////////////////////////////////////////////////////////////////
// CMMPictView

IMPLEMENT_DYNCREATE(CMMPictView, CScrollView)

BEGIN_MESSAGE_MAP(CMMPictView, CScrollView)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDM_PICT_LEFT_ROTATE, OnPictLeftRotate)
	ON_UPDATE_COMMAND_UI(IDM_PICT_LEFT_ROTATE, OnUpdatePictLeftRotate)
	ON_COMMAND(IDM_PICT_RIGHT_ROTATE, OnPictRightRotate)
	ON_UPDATE_COMMAND_UI(IDM_PICT_RIGHT_ROTATE, OnUpdatePictRightRotate)
	ON_COMMAND(IDM_PICT_ENLARGE, OnPictEnlarge)
	ON_UPDATE_COMMAND_UI(IDM_PICT_ENLARGE, OnUpdatePictEnlarge)
	ON_COMMAND(IDM_PICT_REDUCE, OnPictReduce)
	ON_UPDATE_COMMAND_UI(IDM_PICT_REDUCE, OnUpdatePictReduce)
	ON_COMMAND(IDM_PICT_AUTOSIZE, OnPictAutosize)
	ON_UPDATE_COMMAND_UI(IDM_PICT_AUTOSIZE, OnUpdatePictAutosize)
	ON_COMMAND(IDM_PICT_AUTOSIZE2, OnPictAutosize2)
	ON_UPDATE_COMMAND_UI(IDM_PICT_AUTOSIZE2, OnUpdatePictAutosize2)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_UPDATE_COMMAND_UI(IDW_COMBO, OnUpdateCombo)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_CBN_SELCHANGE(IDW_COMBO, OnSelchangeCombo)
	ON_CBN_EDITUPDATE(IDW_COMBO, OnEditchangeCombo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMMPictView クラスの構築/消滅

CMMPictView::CMMPictView()
{
	m_fPictRatio = 1.0;
	m_nAutoSize = AfxGetApp()->GetProfileInt(g_sSectionSettings, "AutoSize", AUTOSIZE_BOTH);
	m_bScrollPicture = FALSE;
	m_bIgnoreUpdate = FALSE;

	m_oBrushBkgndPict.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
	m_oBrushBkgndIcon.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
}

CMMPictView::~CMMPictView()
{
	AfxGetApp()->WriteProfileInt(g_sSectionSettings, "AutoSize", m_nAutoSize);
}

BOOL CMMPictView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.dwExStyle |= WS_EX_CLIENTEDGE;

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMMPictView クラスの描画

void CMMPictView::OnDraw(CDC* pDC)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();

	switch (pDoc->m_nViewMode) {
	case VIEWMODE_ICON:
		DrawIcon(pDC);
		break;
	case VIEWMODE_PICT:
		DrawPicture(pDC);
		break;
	}
}

void CMMPictView::OnInitialUpdate()
{
//	CScrollView::OnInitialUpdate();

	SIZE sizeTotal;
	// TODO: このビューのサイズの合計を計算します。
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CMMPictView クラスの印刷

BOOL CMMPictView::OnPreparePrinting(CPrintInfo* pInfo)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();

	switch (pDoc->m_nViewMode) {
	case VIEWMODE_PICT:
		if (pDoc->GetCurrentItem()->m_hDIB1 == NULL)
			return FALSE;

		pInfo->SetMaxPage(1);
		break;
	case VIEWMODE_ICON:
		if (pDoc->m_nMaxPage == 0)
			return FALSE;

		pInfo->SetMaxPage(pDoc->m_nMaxPage);
		break;
	}
	return DoPreparePrinting(pInfo);
}

void CMMPictView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (g_pPrintDevMode != NULL)
		pDC->ResetDC(g_pPrintDevMode);

	CScrollView::OnBeginPrinting(pDC, pInfo);
}

#ifdef _DEBUG
CMMViewDoc* CMMPictView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMMViewDoc)));
	return (CMMViewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMMPictView クラスのメッセージ ハンドラ

void CMMPictView::DrawPicture(CDC *pDC)
{
	if (m_bIgnoreUpdate) {
		m_bIgnoreUpdate = FALSE;
		return;
	}

	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	CItem *pItem = pDoc->GetCurrentItem();
	if (pItem == NULL || pItem->m_hDIB1 == NULL)
		return;

	RECT rcView;
	GetClientRect(&rcView);

	short nWidth, nHeight, nColor;
	MMPictureGetInfo(pItem->m_hDIB1, &nWidth, &nHeight, &nColor);
	const int nWidthSrc = nWidth;
	const int nHeightSrc = nHeight;
	int nWidthDst, nHeightDst;

	switch (m_nAutoSize) {
	case AUTOSIZE_NONE:
		nWidthDst = (int)(nWidthSrc * m_fPictRatio + 0.5);
		nHeightDst = (int)(nHeightSrc * m_fPictRatio + 0.5);
		break;
	case AUTOSIZE_WIDTH:
		nWidthDst = rcView.right;
		nHeightDst = nHeightSrc * rcView.right / nWidthSrc;
		m_fPictRatio = (double)rcView.right / nWidthSrc;
		break;
	case AUTOSIZE_BOTH:
		nWidthDst = rcView.right;
		nHeightDst = nHeightSrc * rcView.right / nWidthSrc;
		m_fPictRatio = (double)rcView.right / nWidthSrc;
		if (nHeightDst > rcView.bottom) {
			nWidthDst = nWidthSrc * rcView.bottom / nHeightSrc;
			nHeightDst = rcView.bottom;
			m_fPictRatio = (double)rcView.bottom / nHeightSrc;
		}
		break;
	default:
		return;
	}

	CString sStr;
	CComboBox &comboBox = ((CMainFrame *)AfxGetMainWnd())->GetComboBox();
	sStr.Format("%d%%", (int)(m_fPictRatio * 100 + 0.5));
	comboBox.SetWindowText(sStr);
	comboBox.SetEditSel(0, -1);

	int nXDst = (rcView.right - nWidthDst) / 2;
	if (nXDst < 0)
		nXDst = 0;

	int nYDst = (rcView.bottom - nHeightDst) / 2;
	if (nYDst < 0)
		nYDst = 0;

	if (m_sizeDraw.cx != nWidthDst || m_sizeDraw.cy != nHeightDst) {
		m_sizeDraw.cx = nWidthDst;
		m_sizeDraw.cy = nHeightDst;
		SetScrollSizes(MM_TEXT, m_sizeDraw);

		RECT rcView2;
		GetClientRect(&rcView2);

		if (m_nAutoSize == AUTOSIZE_WIDTH && rcView.right > rcView2.right) {
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

	MMPictureDraw(pItem->m_hDIB1, pDC->m_hDC, (SHORT)nXDst, (SHORT)nYDst, (SHORT)nWidthDst, (SHORT)nHeightDst, 0, 0, (SHORT)nWidthSrc, (SHORT)nHeightSrc);
}

void CMMPictView::DrawIcon(CDC *pDC)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	RECT rcDest;
	int	nCount = 0;
	int	x, y, nMod, nRow;
	short nWidth, nHeight,nColor;
	RECT rcVisible;
	RECT rcText;
	CSize sizeScroll(0, 0);
	CItem *pItem;

	if (pDoc->m_pMMFile == NULL)
		return;

	pDC->SetBkMode(TRANSPARENT);

	RECT rect;
	GetClientRect(&rect);

	const int nColumn = max((rect.right - PICT_MARGIN) / (PICT_WIDTH + PICT_MARGIN), 1);

	CFont *pFontOrg = pDC->SelectObject(&pDoc->m_oFont);

	for (int i = 0; i < pDoc->m_nMaxPage; i++) {
		nMod = nCount % nColumn;
		x = (PICT_WIDTH + PICT_MARGIN) * nMod + PICT_MARGIN;
		rcDest.left = x;
		rcDest.right = x + PICT_WIDTH;

		nRow = nCount / nColumn;
		y = (PICT_HEIGHT + PICT_TEXT + PICT_MARGIN) * nRow + PICT_MARGIN;
		rcDest.top = y;
		rcDest.bottom = y + PICT_HEIGHT;

		rcVisible.left = rcDest.left - PICT_FRAME;
		rcVisible.top = rcDest.top - PICT_FRAME;
		rcVisible.right = rcDest.right + PICT_FRAME;
		rcVisible.bottom = rcDest.bottom + PICT_TEXT + PICT_MARGIN;

		if (rcVisible.right > sizeScroll.cx)
			sizeScroll.cx = rcVisible.right;

		if (rcVisible.bottom > sizeScroll.cy)
			sizeScroll.cy = rcVisible.bottom;

		nCount++;

		if (!pDC->RectVisible(&rcVisible))
			continue;

		pItem = &pDoc->m_pItem[i];

		if (pItem->m_hDIB2 != NULL) {
			MMPictureGetInfo(pItem->m_hDIB2, &nWidth, &nHeight, &nColor);

			MMPictureDraw(pItem->m_hDIB2, pDC->m_hDC, (short)(rcDest.left + (PICT_WIDTH - nWidth) / 2),
					(short)(rcDest.top + (PICT_HEIGHT - nHeight) / 2), nWidth, nHeight, 0, 0, -1, -1);
		}

		rcDest.left -= PICT_FRAME;
		rcDest.top -= PICT_FRAME;
		rcDest.right += PICT_FRAME;
		rcDest.bottom += PICT_FRAME;
		pDC->DrawEdge(&rcDest, BDR_RAISEDINNER, BF_RECT);

		pItem->m_rcIcon = rcDest;

		rcText.left = x;
		rcText.top = y + PICT_HEIGHT + PICT_FRAME + 1;
		rcText.right = rcText.left + PICT_WIDTH;
		rcText.bottom = rcText.top + PICT_TEXT;
		pDC->DrawText(pItem->m_sTitle, &rcText, DT_WORDBREAK);
	}

	SetScrollSizes(MM_TEXT, sizeScroll);

	pDC->SelectObject(pFontOrg);

	m_sizeDraw.cx = m_sizeDraw.cy = 0;
}

BOOL CMMPictView::OnEraseBkgnd(CDC* pDC)
{
	if (m_bIgnoreUpdate)
		return TRUE;

	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	RECT rect;
	pDC->GetClipBox(&rect);
	pDC->FillRect(&rect, pDoc->m_nViewMode == VIEWMODE_PICT ? &m_oBrushBkgndPict : &m_oBrushBkgndIcon);
	return TRUE;
}

void CMMPictView::OnPictLeftRotate()
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	CItem *pItem = pDoc->GetCurrentItem();

	if (pItem != NULL && pItem->m_hDIB1 != NULL) {
		MMPictureRotate(pItem->m_hDIB1, 3);
		Invalidate(TRUE);
	}
}

void CMMPictView::OnUpdatePictLeftRotate(CCmdUI* pCmdUI)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	CItem *pItem = pDoc->GetCurrentItem();

	pCmdUI->Enable(pDoc->m_nViewMode == VIEWMODE_PICT && pItem != NULL && pItem->m_hDIB1 != NULL);
}

void CMMPictView::OnPictRightRotate()
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	CItem *pItem = pDoc->GetCurrentItem();

	if (pItem != NULL && pItem->m_hDIB1 != NULL) {
		MMPictureRotate(pItem->m_hDIB1, 1);
		Invalidate(TRUE);
	}
}

void CMMPictView::OnUpdatePictRightRotate(CCmdUI* pCmdUI)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	CItem *pItem = pDoc->GetCurrentItem();

	pCmdUI->Enable(pDoc->m_nViewMode == VIEWMODE_PICT && pItem != NULL && pItem->m_hDIB1 != NULL);
}

void CMMPictView::OnPictEnlarge()
{
	m_nAutoSize = AUTOSIZE_NONE;
	m_fPictRatio *= 1.5;
	Invalidate(TRUE);
}

void CMMPictView::OnUpdatePictEnlarge(CCmdUI* pCmdUI)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	CItem *pItem = pDoc->GetCurrentItem();

	pCmdUI->Enable(pDoc->m_nViewMode == VIEWMODE_PICT && pItem != NULL && pItem->m_hDIB1 != NULL);
}

void CMMPictView::OnPictReduce()
{
	m_nAutoSize = AUTOSIZE_NONE;
	m_fPictRatio /= 1.5;
	Invalidate(TRUE);
}

void CMMPictView::OnUpdatePictReduce(CCmdUI* pCmdUI)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	CItem *pItem = pDoc->GetCurrentItem();

	pCmdUI->Enable(pDoc->m_nViewMode == VIEWMODE_PICT && pItem != NULL && pItem->m_hDIB1 != NULL);
}

void CMMPictView::OnPictAutosize()
{
	m_nAutoSize = (m_nAutoSize == AUTOSIZE_BOTH ? AUTOSIZE_NONE : AUTOSIZE_BOTH);
	Invalidate(TRUE);
}

void CMMPictView::OnUpdatePictAutosize(CCmdUI* pCmdUI)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	CItem *pItem = pDoc->GetCurrentItem();

	pCmdUI->Enable(pDoc->m_nViewMode == VIEWMODE_PICT && pItem != NULL && pItem->m_hDIB1 != NULL);
	pCmdUI->SetCheck(m_nAutoSize == AUTOSIZE_BOTH);
}

void CMMPictView::OnPictAutosize2()
{
	m_nAutoSize = (m_nAutoSize == AUTOSIZE_WIDTH ? AUTOSIZE_NONE : AUTOSIZE_WIDTH);
	Invalidate(TRUE);
}

void CMMPictView::OnUpdatePictAutosize2(CCmdUI* pCmdUI)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	CItem *pItem = pDoc->GetCurrentItem();

	pCmdUI->Enable(pDoc->m_nViewMode == VIEWMODE_PICT && pItem != NULL && pItem->m_hDIB1 != NULL);
	pCmdUI->SetCheck(m_nAutoSize == AUTOSIZE_WIDTH);
}

void CMMPictView::OnSelchangeCombo()
{
	CComboBox &comboBox = ((CMainFrame *)AfxGetMainWnd())->GetComboBox();

	const int n = comboBox.GetCurSel();
	if (n != LB_ERR) {
		CString sStr;

		comboBox.GetLBText(n, sStr);
		comboBox.SetWindowText(sStr);
		m_nAutoSize = AUTOSIZE_NONE;
		comboBox.GetWindowText(sStr);
		m_fPictRatio = atof(sStr) / 100;
		Invalidate(TRUE);
	}
}

void CMMPictView::OnEditchangeCombo()
{
	CComboBox &comboBox = ((CMainFrame *)AfxGetMainWnd())->GetComboBox();
	CString sStr1, sStr2;

	comboBox.GetWindowText(sStr1);
	sStr2 = sStr1.SpanIncluding("0123456789%");
	if (sStr1.GetLength() != sStr2.GetLength()) {
		comboBox.SetWindowText(sStr2);
		comboBox.SetEditSel(sStr2.GetLength(), sStr2.GetLength());
	}
}

void CMMPictView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	CItem *pItem;

	switch (pDoc->m_nViewMode) {
	case VIEWMODE_PICT:
		pItem = pDoc->GetCurrentItem();
		if (pItem != NULL && pItem->m_hDIB1 != NULL)
			PrintPicture(pDC, pItem->m_hDIB1, g_rcPrintMargin);
		break;
	case VIEWMODE_ICON:
		pItem = &pDoc->m_pItem[pInfo->m_nCurPage - 1];
		if (pItem->m_hDIB1 != NULL)
			PrintPicture(pDC, pItem->m_hDIB1, g_rcPrintMargin);
		break;
	}
}

void CMMPictView::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	CMMViewApp *pApp = (CMMViewApp *)AfxGetApp();

	switch (pDoc->m_nViewMode) {
	case VIEWMODE_ICON:
		break;
	case VIEWMODE_PICT:
		m_bScrollPicture = TRUE;
		CPoint scrollPos = GetScrollPosition();
		m_posPicture = (CPoint)(scrollPos + point);
		::SetCursor(pApp->m_hCursorHandClose);
		break;
	}

	SetCapture();
}

void CMMPictView::OnLButtonUp(UINT /*nFlags*/, CPoint point)
{
	if (GetCapture() != this)
		return;

	ReleaseCapture();

	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	CMMViewApp *pApp = (CMMViewApp *)AfxGetApp();
	int nPage;

	switch (pDoc->m_nViewMode) {
	case VIEWMODE_ICON:
		if ((nPage = GetItemByPoint(point + (CSize)GetDeviceScrollPosition())) != -1)
			pDoc->ShowPicture(nPage);
		break;
	case VIEWMODE_PICT:
		m_bScrollPicture = FALSE;
		::SetCursor(pApp->m_hCursorHandOpen);
		break;
	}
}

void CMMPictView::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	CMMViewApp *pApp = (CMMViewApp *)AfxGetApp();

	switch (pDoc->m_nViewMode) {
	case VIEWMODE_ICON:
		break;
	case VIEWMODE_PICT:
		if (m_bScrollPicture) {
			CPoint scrollPos = m_posPicture - point;

			RECT rectView;
			GetClientRect(&rectView);
			if (rectView.right >= m_sizeDraw.cx)
				scrollPos.x = 0;
			if (rectView.bottom >= m_sizeDraw.cy)
				scrollPos.y = 0;
			ScrollToPosition(scrollPos);
			::SetCursor(pApp->m_hCursorHandClose);
		} else
			::SetCursor(pApp->m_hCursorHandOpen);
		break;
	}
}

int CMMPictView::GetItemByPoint(CPoint point)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();

	for (int i = 0; i < pDoc->m_nMaxPage; i++) {
		if (pDoc->m_pItem[i].m_rcIcon.PtInRect(point))
			return i;
	}

	return -1;
}

BOOL CMMPictView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();

	if (pDoc->m_nViewMode == VIEWMODE_PICT && nHitTest == HTCLIENT)
		return TRUE;

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CMMPictView::OnUpdateCombo(CCmdUI* pCmdUI)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();

	if (pDoc->m_nViewMode == VIEWMODE_PICT)
		pCmdUI->Enable(TRUE);
	else {
		pCmdUI->Enable(FALSE);
		CComboBox &comboBox = ((CMainFrame *)AfxGetMainWnd())->GetComboBox();
		comboBox.SetEditSel(-1, -1);
	}
}

void CMMPictView::OnEditCopy()
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	CItem *pItem = pDoc->GetCurrentItem();

	if (pItem != NULL && pItem->m_hDIB1 != NULL) {
		if (AfxGetApp()->m_pMainWnd->OpenClipboard()) {
			BeginWaitCursor();
			::EmptyClipboard();
			::SetClipboardData(CF_DIB, CopyHandle((HANDLE)pItem->m_hDIB1));
			::CloseClipboard();
			EndWaitCursor();
		}
	}
}

void CMMPictView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	CItem *pItem = pDoc->GetCurrentItem();

	pCmdUI->Enable(pDoc->m_nViewMode == VIEWMODE_PICT && pItem != NULL && pItem->m_hDIB1 != NULL);
}

void CMMPictView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
	switch (lHint) {
	case VIEWHINT_SETFONT:
		DrawItems();
		break;
	default:
		DrawItems();
		break;
	}
}

void CMMPictView::DrawItems()
{
	if (m_nMapMode > 0) {
		POINT point{};
		ScrollToPosition(point);
	}

	Invalidate(TRUE);
}

BOOL CMMPictView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nID == 1 && nCode == 0) {
		CString sStr;
		m_nAutoSize = AUTOSIZE_NONE;
		((CMainFrame *)GetParent())->GetComboBox().GetWindowText(sStr);
		m_fPictRatio = atof(sStr) / 100;
		Invalidate(TRUE);
	}

	return CScrollView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
