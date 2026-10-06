// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "MMLib.h"
#include "MMLibDoc.h"
#include "MainFrm.h"
#include "MMLibListView.h"
#include "MMLibPictView.h"
#include "MMLibInfoView.h"
#include "MMLibTextView.h"
#include "Regist.h"
#include "VerRes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_INITMENU()
	ON_COMMAND(IDM_TOOLBAR_PICT, OnToolbarPict)
	ON_UPDATE_COMMAND_UI(IDM_TOOLBAR_PICT, OnUpdateToolbarPict)
	ON_COMMAND(IDM_TOOLBAR_STANDARD, OnToolbarStandard)
	ON_UPDATE_COMMAND_UI(IDM_TOOLBAR_STANDARD, OnUpdateToolbarStandard)
	ON_COMMAND(IDM_TOOLBAR_STRING, OnToolbarString)
	ON_UPDATE_COMMAND_UI(IDM_TOOLBAR_STRING, OnUpdateToolbarString)
	ON_WM_SIZE()
	ON_COMMAND(IDM_TOOLBAR_VIEW, OnToolbarView)
	ON_UPDATE_COMMAND_UI(IDM_TOOLBAR_VIEW, OnUpdateToolbarView)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_INFO, OnUpdateInfo)
	ON_MESSAGE(WM_ENTERSIZEMOVE, OnEnterSizeMove)
	ON_MESSAGE(WM_MYCOMMAND, OnMyCommand)
	ON_MESSAGE(WM_CHECK_LICENSE1, OnGetCheckLicense1)
	ON_MESSAGE(WM_CHECK_LICENSE2, OnGetCheckLicense2)
	ON_MESSAGE(WM_REGIST_LICENSE, OnRegistLicense)
	ON_MESSAGE(WM_NOTIFY_UPDATE, OnNotifyUpdate)
END_MESSAGE_MAP()

static const UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケータ
	ID_INDICATOR_INFO,
	ID_INDICATOR_KANA,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの構築/消滅

CMainFrame::CMainFrame()
{
	m_bShowPict = FALSE;
	m_bMaxPict = FALSE;
	m_nTransMenu = 0;
	m_pTransMenu = NULL;
	m_nFixedHeight = 0;
	m_nClientHeight = 0;
	m_nCheckData = GET_CHECK_DATA;
	m_nSaveHeight = 0;
	m_bHidePict = FALSE;
	m_pTextSearchDlg = NULL;
}

CMainFrame::~CMainFrame()
{
	if (m_pTransMenu != NULL)
		delete [] m_pTransMenu;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndMainToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndMainToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // 作成に失敗
	}

	if (!m_wndViewToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndViewToolBar.LoadToolBar(IDR_VIEW))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // 作成に失敗
	}

	if (!m_wndPictToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndPictToolBar.LoadToolBar(IDR_PICT))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // 作成に失敗
	}

	// create a combo box for the address bar
	if (!m_wndComboBox.Create(CBS_DROPDOWN | WS_CHILD | WS_VISIBLE, CRect(0, 1, 60, 200), &m_wndPictToolBar, IDW_COMBO))
	{
		TRACE0("Failed to create combobox\n");
		return -1;      // fail to create
	}

	m_wndComboBox.SetFont(&g_oDefaultFont);

	m_wndComboBox.AddString("200%");
	m_wndComboBox.AddString("150%");
	m_wndComboBox.AddString("100%");
	m_wndComboBox.AddString("75%");
	m_wndComboBox.AddString("50%");
	m_wndComboBox.AddString("25%");
	m_wndComboBox.LimitText(4);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // 作成に失敗
	}

	m_wndStatusBar.SetPaneStyle(0, SBPS_NORMAL);
	m_wndStatusBar.SetPaneStyle(1, SBPS_STRETCH);

	// TODO: ツール バーをドッキング可能にしない場合は以下の３行を削除
	//       してください。
	m_wndMainToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndViewToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPictToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndMainToolBar);
//	DockControlBarLeftOf(&m_wndPictToolBar, &m_wndMainToolBar);

	CWinApp *pWinApp = ::AfxGetApp();
	m_bToolbarString = pWinApp->GetProfileInt(g_sSectionMainWindow, "ShowToolBarString", 0);
	ShowControlBar(&m_wndMainToolBar, pWinApp->GetProfileInt(g_sSectionMainWindow, "ShowMainToolBar", 1), FALSE);
	ShowControlBar(&m_wndViewToolBar, pWinApp->GetProfileInt(g_sSectionMainWindow, "ShowViewToolBar", 1), FALSE);
	ShowControlBar(&m_wndPictToolBar, pWinApp->GetProfileInt(g_sSectionMainWindow, "ShowPictToolBar", 1), FALSE);

	SetToolbar();

	CRect rect;
	m_wndSplitter.GetClientRect(rect);
	m_wndSplitter2.SetRowInfo(0, rect.Height() - 100, 0);
	m_wndSplitter2.RecalcLayout();
	RecalcLayout();

	RefreshTransMenu();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	CWinApp *pWinApp = ::AfxGetApp();

	const int nLeft = pWinApp->GetProfileInt(g_sSectionMainWindow, "Left", 0);
	const int nTop = pWinApp->GetProfileInt(g_sSectionMainWindow, "Top", 0);
	const int nRight = pWinApp->GetProfileInt(g_sSectionMainWindow, "Right", 0);
	const int nBottom = pWinApp->GetProfileInt(g_sSectionMainWindow, "Bottom", 0);

	if (nRight != 0 && nBottom != 0) {
//		cs.x = left;
//		cs.y = top;
		cs.cx = nRight - nLeft;
		cs.cy = nBottom - nTop;
	}

	cs.style &= ~FWS_ADDTOTITLE;

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージ ハンドラ

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	CWinApp *pWinApp = ::AfxGetApp();

	// create a splitter with 1 row, 2 columns
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(pWinApp->GetProfileInt(g_sSectionMainWindow, "Width", 200), 0), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	// create a splitter with 2 row, 1 columns
	if (!m_wndSplitter2.CreateStatic(
		&m_wndSplitter,     // 親ウィンドウは 1 番め目のスプリッター
		2, 1,               // 新しいスプリッターは 2 行、1 列
		WS_CHILD | WS_VISIBLE | WS_BORDER,  // スタイル WS_BORDER が必要です。
		m_wndSplitter.IdFromRowCol(0, 1)
			// 新しいスプリッターは 1 番目の列にあります、 1 番目のスプリッターの 2 行目
	   ))
	{
		TRACE0("Failed to create nested splitter\n");
		return FALSE;
	}

	// create a splitter with 2 row, 1 columns
	if (!m_wndSplitter3.CreateStatic(
		&m_wndSplitter2,
		2, 1,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		m_wndSplitter2.IdFromRowCol(0, 0)
	   ))
	{
		TRACE0("Failed to create nested splitter\n");
		return FALSE;
	}

	CRect rect;
	GetClientRect(rect);
	const int nHeight1 = rect.Height() - 150;//pWinApp->GetProfileInt(g_sSectionMainWindow, "Height", rect.Height() - 150);
	const int nHeight2 = 200;//pWinApp->GetProfileInt(g_sSectionMainWindow, "Height2", 200);
//	if (nHeight1 < nHeight2 || nHeight1 <= 10 || nHeight2 <= 10) {
//		nHeight1 = rect.Height() - 150;
//		nHeight2 = 200;
//	}

	m_wndSplitter2.SetRowInfo(0, nHeight1, 0);

	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter3.CreateView(0, 0,
		RUNTIME_CLASS(CMMLibInfoView), CSize(0, nHeight2), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter3.CreateView(1, 0,
		RUNTIME_CLASS(CMMLibListView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter2.CreateView(1, 0,
		RUNTIME_CLASS(CMMLibTextView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	m_nFixedHeight = nHeight1;
	m_nClientHeight = rect.bottom;

	m_pListView = (CView *)m_wndSplitter3.GetDlgItem(m_wndSplitter3.IdFromRowCol(1, 0));

	m_pPictView = (CView *)RUNTIME_CLASS(CMMLibPictView)->CreateObject();
	m_pPictView->Create(NULL, NULL, WS_CHILD | WS_BORDER, rectDefault, &m_wndSplitter3, m_wndSplitter3.IdFromRowCol(1, 0) + 2, pContext);

	m_pTextView = (CView *)m_wndSplitter2.GetDlgItem(m_wndSplitter2.IdFromRowCol(1, 0));

	return TRUE;
}

void CMainFrame::OnClose()
{
	CWinApp *pWinApp = ::AfxGetApp();

	WINDOWPLACEMENT winpl{
		.length = sizeof(WINDOWPLACEMENT)
	};
	GetWindowPlacement(&winpl);

	pWinApp->WriteProfileInt(g_sSectionMainWindow, "Left", winpl.rcNormalPosition.left);
	pWinApp->WriteProfileInt(g_sSectionMainWindow, "Top", winpl.rcNormalPosition.top);
	pWinApp->WriteProfileInt(g_sSectionMainWindow, "Right", winpl.rcNormalPosition.right);
	pWinApp->WriteProfileInt(g_sSectionMainWindow, "Bottom", winpl.rcNormalPosition.bottom);
	pWinApp->WriteProfileInt(g_sSectionMainWindow, "Show", winpl.showCmd);

	if (m_wndMainToolBar.GetParent()->IsWindowVisible())
		pWinApp->WriteProfileInt(g_sSectionMainWindow, "ShowMainToolBar", m_wndMainToolBar.IsWindowVisible());
	if (m_wndViewToolBar.GetParent()->IsWindowVisible())
		pWinApp->WriteProfileInt(g_sSectionMainWindow, "ShowViewToolBar", m_wndViewToolBar.IsWindowVisible());
	if (m_wndPictToolBar.GetParent()->IsWindowVisible())
		pWinApp->WriteProfileInt(g_sSectionMainWindow, "ShowPictToolBar", m_wndPictToolBar.IsWindowVisible());
	pWinApp->WriteProfileInt(g_sSectionMainWindow, "ShowToolBarString", m_bToolbarString);

	int cxCur, cxMin;
	m_wndSplitter.GetColumnInfo(0, cxCur, cxMin);
	pWinApp->WriteProfileInt(g_sSectionMainWindow, "Width", cxCur);
	m_wndSplitter2.GetRowInfo(0, cxCur, cxMin);
	pWinApp->WriteProfileInt(g_sSectionMainWindow, "Height", cxCur);
	m_wndSplitter3.GetRowInfo(0, cxCur, cxMin);
	pWinApp->WriteProfileInt(g_sSectionMainWindow, "Height2", cxCur);

	CFrameWnd::OnClose();
}

void CMainFrame::ShowPictView(BOOL bShowPict)
{
	if (bShowPict != m_bShowPict) {
		CView *pViewAdd, *pViewRemove;

		if (bShowPict) {
			pViewAdd = m_pPictView;
			pViewRemove = m_pListView;
		} else {
			pViewAdd = m_pListView;
			pViewRemove = m_pPictView;

			SizePictView(FALSE);
		}

		const int nSwitchChildID = pViewAdd->GetDlgCtrlID();
		pViewAdd->SetDlgCtrlID(pViewRemove->GetDlgCtrlID());
		pViewRemove->SetDlgCtrlID(nSwitchChildID);

		pViewRemove->ShowWindow(SW_HIDE);
		pViewAdd->ShowWindow(SW_SHOW);
		SetActiveView(pViewAdd);

		if (m_bMaxPict)
			RecalcLayout();
		else
			m_wndSplitter3.RecalcLayout();

		m_bShowPict = bShowPict;
	}
}

void CMainFrame::MaxPictView(BOOL bMaxPict)
{
	if (bMaxPict != m_bMaxPict) {
		CView *pView;

		if (bMaxPict) {
			m_wndSplitter.ShowWindow(SW_HIDE);
			m_pListView->SetParent(this);
			m_pPictView->SetParent(this);
			if (m_bShowPict)
				pView = m_pPictView;
			else
				pView = m_pListView;
			const int nSwitchChildID = pView->GetDlgCtrlID();
			pView->SetDlgCtrlID(m_wndSplitter.GetDlgCtrlID());
			m_wndSplitter.SetDlgCtrlID(nSwitchChildID);
			RecalcLayout();
		} else {
			m_pListView->SetParent(&m_wndSplitter3);
			m_pPictView->SetParent(&m_wndSplitter3);
			if (m_bShowPict)
				pView = m_pPictView;
			else
				pView = m_pListView;
			const int nSwitchChildID = m_wndSplitter.GetDlgCtrlID();
			m_wndSplitter.SetDlgCtrlID(pView->GetDlgCtrlID());
			pView->SetDlgCtrlID(nSwitchChildID);
			RecalcLayout();
			m_wndSplitter3.RecalcLayout();
			m_wndSplitter.ShowWindow(SW_SHOW);
		}
		m_bMaxPict = bMaxPict;
	}
}

void CMainFrame::OnUpdateInfo(CCmdUI *pCmdUI)
{
    pCmdUI->Enable();
    pCmdUI->SetText(g_sStatusInfo);
}

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

#define MENU_VIEW			3
#define MENU_DETAIL_LIB		13
#define MENU_DETAIL_FOLDER	14
#define MENU_DETAIL_DATA	15

void CMainFrame::OnInitMenu(CMenu* pMenu)
{
	CFrameWnd::OnInitMenu(pMenu);

	CMenu *pSubMenu = pMenu->GetSubMenu(MENU_VIEW);
	if (pSubMenu == NULL)
		return;

	CMMLibDoc *pDoc = (CMMLibDoc *)GetActiveDocument( );

	pSubMenu->EnableMenuItem(13, MF_BYPOSITION | MF_GRAYED);
	pSubMenu->EnableMenuItem(MENU_DETAIL_FOLDER, MF_BYPOSITION | MF_GRAYED);
	pSubMenu->EnableMenuItem(MENU_DETAIL_DATA, MF_BYPOSITION | MF_GRAYED);

	if (pDoc->m_nViewMode == VIEWMODE_DETAIL) {
		switch (g_nDetailMode) {
		case DETAIL_LIBRARY:
			pSubMenu->EnableMenuItem(MENU_DETAIL_LIB, MF_BYPOSITION | MF_ENABLED);
			break;
		case DETAIL_FOLDER:
			pSubMenu->EnableMenuItem(MENU_DETAIL_FOLDER, MF_BYPOSITION | MF_ENABLED);
			break;
		case DETAIL_DATA:
			pSubMenu->EnableMenuItem(MENU_DETAIL_DATA, MF_BYPOSITION | MF_ENABLED);
			break;
		}
	}
}

void CMainFrame::OnToolbarPict()
{
	ShowControlBar(&m_wndPictToolBar, !m_wndPictToolBar.IsWindowVisible(), FALSE);
}

void CMainFrame::OnUpdateToolbarPict(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPictToolBar.IsWindowVisible());
}

void CMainFrame::OnToolbarStandard()
{
	ShowControlBar(&m_wndMainToolBar, !m_wndMainToolBar.IsWindowVisible(), FALSE);
}

void CMainFrame::OnUpdateToolbarStandard(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndMainToolBar.IsWindowVisible());
}

void CMainFrame::OnToolbarString()
{
	m_bToolbarString = !m_bToolbarString;
	SetToolbar();
}

void CMainFrame::OnUpdateToolbarString(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bToolbarString);
}

void CMainFrame::OnToolbarView()
{
	ShowControlBar(&m_wndViewToolBar, !m_wndViewToolBar.IsWindowVisible(), FALSE);
}

void CMainFrame::OnUpdateToolbarView(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndViewToolBar.IsWindowVisible());
}

void CMainFrame::SetToolbar()
{
	if (m_bToolbarString) {
		CFont *pFont = m_wndMainToolBar.GetFont();
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		const int nHeight = abs(lf.lfHeight) + 25;
		const int nWidth = abs(lf.lfHeight) * 3 + 25;

		m_wndMainToolBar.GetToolBarCtrl().SetMaxTextRows(1);
		m_wndViewToolBar.GetToolBarCtrl().SetMaxTextRows(1);
		m_wndPictToolBar.GetToolBarCtrl().SetMaxTextRows(1);

		m_wndMainToolBar.SetSizes(CSize(nWidth, nHeight), CSize(16, 17));
		m_wndMainToolBar.GetToolBarCtrl().SetButtonWidth(nWidth, nWidth);
		m_wndMainToolBar.SetHeight(nHeight + 4);
		m_wndMainToolBar.SetButtonText(0, GetString(IDS_NEW_DOCUMENT));
		m_wndMainToolBar.SetButtonText(1, GetString(IDS_OPEN));
		m_wndMainToolBar.SetButtonText(2, GetString(IDS_NEW_DATA));
		m_wndMainToolBar.SetButtonText(4, GetString(IDS_PDF));
		m_wndMainToolBar.SetButtonText(6, GetString(IDS_PRINT));
		m_wndMainToolBar.SetButtonText(7, GetString(IDS_PREVIEW));
		m_wndMainToolBar.SetButtonText(9, GetString(IDS_SEND));
		m_wndMainToolBar.SetButtonText(11, GetString(IDS_CUT));
		m_wndMainToolBar.SetButtonText(12, GetString(IDS_COPY));
		m_wndMainToolBar.SetButtonText(13, GetString(IDS_PASTE));
		m_wndMainToolBar.SetButtonText(15, GetString(IDS_FIND));
		m_wndMainToolBar.SetButtonText(16, GetString(IDS_KEYWORD2));
		m_wndMainToolBar.SetButtonText(17, GetString(IDS_IMAGE_SEARCH));
		m_wndMainToolBar.SetButtonText(18, GetString(IDS_TEXT_SEARCH));
		m_wndMainToolBar.SetButtonText(20, GetString(IDS_ACCESS_HISTORY));
		m_wndMainToolBar.SetButtonText(22, GetString(IDS_RETURN));
		m_wndMainToolBar.SetButtonText(24, GetString(IDS_HELP));

		m_wndViewToolBar.SetSizes(CSize(nWidth, nHeight), CSize(16, 17));
		m_wndViewToolBar.GetToolBarCtrl().SetButtonWidth(nWidth, nWidth);
		m_wndViewToolBar.SetHeight(nHeight + 4);
		m_wndViewToolBar.SetButtonText(0, GetString(IDS_PREV_DATA));
		m_wndViewToolBar.SetButtonText(1, GetString(IDS_NEXT_DATA));
		m_wndViewToolBar.SetButtonText(3, GetString(IDS_FULL_VIEW));
		m_wndViewToolBar.SetButtonText(4, GetString(IDS_WORD_WRAP));
		m_wndViewToolBar.SetButtonText(6, GetString(IDS_REDUCTION_PICT));
		m_wndViewToolBar.SetButtonText(7, GetString(IDS_LIST_VIEW));
		m_wndViewToolBar.SetButtonText(8, GetString(IDS_DETAIL_VIEW));

		m_wndPictToolBar.SetButtonInfo(0, IDW_COMBO, TBBS_DROPDOWN, 62);
		//m_wndComboBox.SetWindowPos(NULL, 0, 8, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

		m_wndPictToolBar.SetSizes(CSize(nWidth, nHeight), CSize(16, 17));
		m_wndPictToolBar.GetToolBarCtrl().SetButtonWidth(nWidth, nWidth);
		m_wndPictToolBar.SetHeight(nHeight + 4);
		m_wndPictToolBar.SetButtonText(0, GetString(IDS_SCALE));
		m_wndPictToolBar.SetButtonText(1, GetString(IDS_AUTO_SIZE));
		m_wndPictToolBar.SetButtonText(2, GetString(IDS_AUTO_WIDTH));
		m_wndPictToolBar.SetButtonText(3, GetString(IDS_ENLARGE));
		m_wndPictToolBar.SetButtonText(4, GetString(IDS_REDUCTION));
		m_wndPictToolBar.SetButtonText(6, GetString(IDS_LEFT_ROTATION2));
		m_wndPictToolBar.SetButtonText(7, GetString(IDS_RIGHT_ROTATION2));
		m_wndPictToolBar.SetButtonText(8, GetString(IDS_ALL_ROTATION));
		m_wndPictToolBar.SetButtonText(10, GetString(IDS_OTHER_WINDOW));

		CRect rect;
		DockControlBar(&m_wndMainToolBar);
		m_wndMainToolBar.GetWindowRect(&rect);
		rect.OffsetRect(0, rect.Height());
		DockControlBar(&m_wndPictToolBar, AFX_IDW_DOCKBAR_TOP, &rect);
		DockControlBar(&m_wndViewToolBar, AFX_IDW_DOCKBAR_TOP, &rect);
	} else {
		m_wndMainToolBar.GetToolBarCtrl().SetMaxTextRows(0);
		m_wndViewToolBar.GetToolBarCtrl().SetMaxTextRows(0);
		m_wndPictToolBar.GetToolBarCtrl().SetMaxTextRows(0);

		m_wndMainToolBar.SetSizes(CSize(23,23), CSize(16,15));
		m_wndMainToolBar.SetHeight(29);

		m_wndViewToolBar.SetSizes(CSize(23,23), CSize(16,15));
		m_wndViewToolBar.SetHeight(29);

		m_wndPictToolBar.SetButtonInfo(0, IDW_COMBO, TBBS_SEPARATOR, 62);
		m_wndComboBox.SetWindowPos(NULL, 0, 1, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

		m_wndPictToolBar.SetSizes(CSize(23,23), CSize(16,15));
		m_wndPictToolBar.SetHeight(29);

		DockControlBar(&m_wndMainToolBar);
		DockControlBarLeftOf(&m_wndViewToolBar, &m_wndMainToolBar);
		DockControlBarLeftOf(&m_wndPictToolBar, &m_wndViewToolBar);
	}

	RecalcLayout();
}

void CMainFrame::RefreshTransMenu()
{
	CWinApp *pWinApp = ::AfxGetApp();
	CMenu* pTopMenu = GetMenu();
	CMenu* pTransMenu = pTopMenu->GetSubMenu(5);
	CString sEntry;
	CString sAppName;
	MENUITEMINFO mii;
	HICON hIcon = NULL;
	int nIcon;

	while (pTransMenu->GetMenuItemCount() > 2) {
		pTransMenu->GetMenuItemInfo(0, &mii, TRUE);
		if (mii.fMask & MIIM_BITMAP)
			::DeleteObject(mii.hbmpItem);
		pTransMenu->DeleteMenu(0, MF_BYPOSITION);
	}

	m_nTransMenu = pWinApp->GetProfileInt(g_sSectionTransfer, "Num", 0);
	const int nMenuIcon = pWinApp->GetProfileInt(g_sSectionTransfer, "MenuIcon", 1);

	if (m_pTransMenu != NULL)
		delete [] m_pTransMenu;
	m_pTransMenu = new TransMenu[m_nTransMenu];

	for (int i = 0; i < m_nTransMenu; i++) {
		sEntry.Format("AppName%d", i);
		sAppName = " " + pWinApp->GetProfileString(g_sSectionTransfer, sEntry);

		sEntry.Format("AppFile%d", i);
		m_pTransMenu[i].sFileName = pWinApp->GetProfileString(g_sSectionTransfer, sEntry);

		sEntry.Format("SupportFile%d", i);
		m_pTransMenu[i].nSupportFile = pWinApp->GetProfileInt(g_sSectionTransfer, sEntry, PICTFILE_TRANSDEF);

//		pTransMenu->InsertMenu(i, MF_BYPOSITION, IDM_TRANS_APP + i, sAppName);

		switch (nMenuIcon) {
		case 1:
			nIcon = ::ExtractIconEx(m_pTransMenu[i].sFileName, 0, NULL, &hIcon, 1);
			break;
		case 2:
			nIcon = ::ExtractIconEx(m_pTransMenu[i].sFileName, 0, &hIcon, NULL, 1);
			break;
		default:
			nIcon = 0;
			break;
		}

		ICONINFO iconInfo{};
		if (nIcon > 0) {
			::GetIconInfo(hIcon, &iconInfo);
			::DeleteObject(iconInfo.hbmMask);
			::DestroyIcon(hIcon);
		}

		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STRING | MIIM_ID | MIIM_FTYPE | MIIM_STATE;
		if (nIcon > 0)
			mii.fMask |= MIIM_BITMAP;
		mii.fType = 0;
		mii.fState = MFS_ENABLED;
		mii.wID = IDM_TRANS_APP + i;
		mii.hSubMenu = NULL;
		mii.hbmpChecked = NULL;
		mii.hbmpUnchecked = NULL;
		mii.dwItemData = 0;
		mii.dwTypeData = (LPSTR)(LPCSTR)sAppName;
		mii.cch = sAppName.GetLength();
		mii.hbmpItem = iconInfo.hbmColor;
		::InsertMenuItem(pTransMenu->m_hMenu, i, MF_BYPOSITION, &mii);
	}
}

void CMainFrame::MaxInfoView(int nMaxHeight)
{
	int cyCur, cyMin;

	m_wndSplitter3.GetRowInfo(0, cyCur, cyMin);
	if (cyCur > nMaxHeight) {
		m_wndSplitter3.SetRowInfo(0, nMaxHeight, 0);
		m_wndSplitter3.RecalcLayout();
	}
}

void CMainFrame::SizePictView(BOOL bHide)
{
	if (bHide != m_bHidePict) {
		m_bHidePict = bHide;

		if (bHide) {
			int cyCur, cyMin;

			m_wndSplitter2.GetRowInfo(0, cyCur, cyMin);
			m_nSaveHeight = cyCur;

			m_wndSplitter3.GetRowInfo(0, cyCur, cyMin);
			m_wndSplitter2.SetRowInfo(0, cyCur + 5, 0);
		} else {
			m_wndSplitter2.SetRowInfo(0, m_nSaveHeight , 0);
		}

		m_wndSplitter2.RecalcLayout();
	}
}

LRESULT CMainFrame::OnEnterSizeMove(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	int cyCur, cyMin;

	m_wndSplitter2.GetRowInfo(0, cyCur, cyMin);
	m_nFixedHeight = cyCur;

	CRect rect;
	GetClientRect(rect);
	m_nClientHeight = rect.bottom;

	return 0;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	if (!m_bHidePict && m_nClientHeight != 0) {
		int nTextViewHeight = m_nFixedHeight + (cy - m_nClientHeight);
		if (nTextViewHeight < 0)
			nTextViewHeight = 0;

		m_wndSplitter2.SetRowInfo(0, nTextViewHeight, 0);
	}

	CFrameWnd::OnSize(nType, cx, cy);
}

LRESULT CMainFrame::OnMyCommand(WPARAM /*wParam*/, LPARAM lParam)
{
	HANDLE hMem = (HANDLE)lParam;
	char *pMem = (char *)::GlobalLock(hMem);
	if (pMem != NULL) {
		::AfxMessageBox(pMem);
		::GlobalUnlock(hMem);
		::GlobalFree(hMem);
	} else
		::AfxMessageBox("ERR");

	::AfxGetMainWnd()->SetActiveWindow();
	return 0;
}

LRESULT CMainFrame::OnGetCheckLicense1(WPARAM wParam, LPARAM /*lParam*/)
{
	LPCSTR pRegistryName;

	if (m_nCheckData == GetCheckData2())
		pRegistryName = g_sRegistryName;
	else
		pRegistryName = NULL;

	return CheckLicense(g_sProgName, g_hIcon, pRegistryName, (int)wParam, FALSE, PRODUCT_CODE, NULL);
}

LRESULT CMainFrame::OnGetCheckLicense2(WPARAM wParam, LPARAM /*lParam*/)
{
	LPCSTR pRegistryName;

	if (m_nCheckData == GetCheckData2())
		pRegistryName = g_sRegistryName;
	else
		pRegistryName = NULL;

	switch (CheckLicense3(pRegistryName, (int)wParam) - GET_CHECK_DATA) {
	case LICENSE_TRIAL:
		return LICENSE_TRIAL;
	case LICENSE_REGIST:
		return LICENSE_REGIST;
	}
	return 0;
}

LRESULT CMainFrame::OnRegistLicense(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (m_nCheckData == GetCheckData2()) {
		CVerRes res;
		return RegistLicense(g_sProgName, g_sRegistryName, PRODUCT_CODE, res.GetProductVersion(), 0x01);
	} else
		return 0;
}

LRESULT CMainFrame::OnNotifyUpdate(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CMMLibDoc *pDoc = (CMMLibDoc *)GetActiveDocument();
	if (pDoc != NULL)
		pDoc->Reload();

	return 0;
}

void CMainFrame::OpenTextSearchDlg()
{
	CloseTextSearchDlg();

	m_pTextSearchDlg = new CTextSearchDlg;
	m_pTextSearchDlg->m_pDocument = GetActiveDocument();
	m_pTextSearchDlg->Create(CTextSearchDlg::IDD, this);

	CRect rectView;
	m_pTextView->GetWindowRect(rectView);

	CRect rectDlg;
	m_pTextSearchDlg->GetWindowRect(rectDlg);

	const int nLeft = rectView.right - rectDlg.Width() - 20;
	const int nTop = (rectView.Height() > rectDlg.Height() + 20) ? rectView.top + 20 : rectView.top - rectDlg.Height() - 20;
	m_pTextSearchDlg->SetWindowPos(NULL, nLeft, nTop, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_pTextSearchDlg->ShowWindow(SW_SHOW);
}

void CMainFrame::CloseTextSearchDlg()
{
	if (m_pTextSearchDlg != NULL) {
		m_pTextSearchDlg->DestroyWindow();
		m_pTextSearchDlg = NULL;
	}
}

void CMainFrame::ClosedTextSearchDlg()
{
	m_pTextSearchDlg = NULL;
}

BOOL CMainFrame::IsOpenTextView()
{
	return m_pTextSearchDlg != NULL;
}

BOOL CMainFrame::IsEnableTextView()
{
	return m_pTextView->IsWindowEnabled();
}

void CMainFrame::ResetTextSearch()
{
	if (m_pTextSearchDlg != NULL) {
		m_pTextSearchDlg->ResetTextSearch();
	}
}

