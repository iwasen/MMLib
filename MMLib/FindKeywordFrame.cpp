// FindFrame.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "FindKeywordFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindKeywordFrame

IMPLEMENT_DYNCREATE(CFindKeywordFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CFindKeywordFrame, CFrameWnd)
	ON_WM_CLOSE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindKeywordFrame メッセージ ハンドラ

BOOL CFindKeywordFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	CWinApp *pWinApp = ::AfxGetApp();

	const int left = pWinApp->GetProfileInt(g_sSectionFindWindow, "Left", 0);
	const int top = pWinApp->GetProfileInt(g_sSectionFindWindow, "Top", 0);
	const int right = pWinApp->GetProfileInt(g_sSectionFindWindow, "Right", 0);
	const int bottom = pWinApp->GetProfileInt(g_sSectionFindWindow, "Bottom", 0);

	if (right != 0 && bottom != 0) {
		cs.x = left;
		cs.y = top;
		cs.cx = right - left;
		cs.cy = bottom - top;
	}

	cs.style &= ~FWS_ADDTOTITLE;

	return CFrameWnd::PreCreateWindow(cs);
}

void CFindKeywordFrame::OnClose()
{
	CWinApp *pWinApp = ::AfxGetApp();

	WINDOWPLACEMENT winpl{
		.length = sizeof(WINDOWPLACEMENT)
	};
	GetWindowPlacement(&winpl);

	pWinApp->WriteProfileInt(g_sSectionFindWindow, "Left", winpl.rcNormalPosition.left);
	pWinApp->WriteProfileInt(g_sSectionFindWindow, "Top", winpl.rcNormalPosition.top);
	pWinApp->WriteProfileInt(g_sSectionFindWindow, "Right", winpl.rcNormalPosition.right);
	pWinApp->WriteProfileInt(g_sSectionFindWindow, "Bottom", winpl.rcNormalPosition.bottom);

	CFrameWnd::OnClose();
}

int CFindKeywordFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndReBar.Create(this, 0)) {
		TRACE0("Failed to create rebar\n");
		return -1;
	}

	LOGFONT lf;
	CFont *pFont = m_wndReBar.GetFont();
	pFont->GetLogFont(&lf);
	const int lfHeight = -MulDiv(lf.lfHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY), 72);

	if (!m_wndKeyword.Create(WS_CHILD | WS_TABSTOP, CRect(0, 0, 100, lfHeight + 2), &m_wndReBar, IDC_KEYWORD)) {
		TRACE0("Failed to create textbox\n");
		return -1;      // fail to create
	}
	if (!m_wndFind.Create(GetString(IDS_FIND), WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON, CRect(0, 0, 60, lfHeight + 2), &m_wndReBar, IDC_FIND)) {
		TRACE0("Failed to create push button\n");
		return -1;      // fail to create
	}
	if (!m_wndToolBar.CreateEx(&m_wndReBar, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED) ||
		!m_wndToolBar.LoadToolBar(IDR_KEYWORD))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // 作成に失敗
	}

	m_wndKeyword.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_wndFind.EnableWindow();

	m_wndKeyword.SetFont(pFont);
	m_wndFind.SetFont(pFont);

	m_wndReBar.AddBar(&m_wndKeyword, GetString(IDS_KEYWORD), NULL, RBBS_NOGRIPPER);
	m_wndReBar.AddBar(&m_wndToolBar, NULL, NULL, RBBS_NOGRIPPER);
	m_wndReBar.AddBar(&m_wndFind, NULL, NULL, RBBS_NOGRIPPER);

	SetWindowText(GetString(IDS_KEYWORD_FIND));

	return 0;
}
