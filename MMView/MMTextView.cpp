// MMTextView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMView.h"
#include "MainFrm.h"
#include "MMViewDoc.h"
#include "MMTextView.h"

/////////////////////////////////////////////////////////////////////////////
// CMMTextView

IMPLEMENT_DYNCREATE(CMMTextView, CEditView)


BEGIN_MESSAGE_MAP(CMMTextView, CEditView)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMMTextView メッセージ ハンドラ

BOOL CMMTextView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL | ES_READONLY;
	return CEditView::PreCreateWindow(cs);
}

void CMMTextView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	switch (lHint) {
	case VIEWHINT_SETFONT:
		SetFont((CFont *)pHint, TRUE);
		break;
	default:
		DrawText();
		break;
	}
}

void CMMTextView::DrawText()
{
	CMainFrame *pMainWnd = (CMainFrame *)AfxGetMainWnd();

	if (pMainWnd != NULL) {
		CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
		CString sText;

		if (pDoc->m_nViewMode == VIEWMODE_PICT) {
			CItem *pItem = pDoc->GetCurrentItem();
			if (pItem != NULL)
				sText = pItem->m_sText;
		} else
			sText = pDoc->m_sText;

		if (sText.GetLength() != 0) {
			SetWindowText(sText);
			pMainWnd->ShowTextView(TRUE);
		} else {
			SetWindowText("");
			pMainWnd->ShowTextView(FALSE);
		}
	}
}

void CMMTextView::OnInitialUpdate()
{
	CMMViewDoc *pDoc = (CMMViewDoc *)GetDocument();
	SetFont(&pDoc->m_oFont, FALSE);

	CEditView::OnInitialUpdate();
}
