// MainFrm.h : CMainFrame クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TextSearchDlg.h"

#define IDM_TRANS_APP	40000

struct TransMenu {
	CString sFileName;
	int nSupportFile;
};

class CMainFrame : public CFrameWnd
{
protected: // シリアライズ機能のみから作成します。
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	virtual ~CMainFrame();

	int m_nTransMenu;
	TransMenu *m_pTransMenu;
	BOOL m_bHidePict;
	int m_nSaveHeight;

	void ShowPictView(BOOL bShowPict);
	void MaxPictView(BOOL bMaxPict);
	BOOL CheckShowPict() {return m_bShowPict;}
	CComboBox &GetComboBox() {return m_wndComboBox;}
	void RefreshTransMenu();
	void MaxInfoView(int nMaxHeight);
	void SizePictView(BOOL bHide);
	void OpenTextSearchDlg();
	void CloseTextSearchDlg();
	void ClosedTextSearchDlg();
	BOOL IsOpenTextView();
	BOOL IsEnableTextView();
	void ResetTextSearch();

protected:  // コントロール バー用メンバ
	CStatusBar m_wndStatusBar;
	CMyToolBar m_wndMainToolBar;
	CMyToolBar m_wndViewToolBar;
	CMyToolBar m_wndPictToolBar;
	CComboBox m_wndComboBox;
	CTextSearchDlg *m_pTextSearchDlg;

	CSplitterWnd m_wndSplitter;
	CSplitterWnd m_wndSplitter2;
	CSplitterWnd m_wndSplitter3;
	CView *m_pListView;
	CView *m_pPictView;
	CView *m_pTextView;
	BOOL m_bShowPict;
	BOOL m_bMaxPict;
	BOOL m_bToolbarStandard;
	BOOL m_bToolbarPict;
	BOOL m_bToolbarString;
	int m_nFixedHeight;
	int m_nClientHeight;
	ULONG m_nCheckData;

	void DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);
	void SetToolbar();

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnToolbarPict();
	afx_msg void OnUpdateToolbarPict(CCmdUI* pCmdUI);
	afx_msg void OnToolbarStandard();
	afx_msg void OnUpdateToolbarStandard(CCmdUI* pCmdUI);
	afx_msg void OnToolbarString();
	afx_msg void OnUpdateToolbarString(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnToolbarView();
	afx_msg void OnUpdateToolbarView(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInfo(CCmdUI *pCmdUI);
	afx_msg LRESULT OnNotifyUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnterSizeMove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyCommand(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetCheckLicense1(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetCheckLicense2(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRegistLicense(WPARAM wParam, LPARAM lParam);
};
