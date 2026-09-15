// MainFrm.h : CMainFrame クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMyToolBar : public CToolBar
{
public:
	void DrawGripper(CDC* pDC, const CRect& rect);
	void EraseNonClient();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoPaint(CDC* pDC);
	afx_msg void OnNcPaint();
};

class CMainFrame : public CFrameWnd
{
protected: // シリアライズ機能のみから作成します。
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	CComboBox &GetComboBox() {return m_wndComboBox;}
	void SwitchView(UINT nIndex);
	void ShowTextView(BOOL bShow);

protected:
	CStatusBar  m_wndStatusBar;
	CMyToolBar  m_wndToolBar;
	CComboBox	m_wndComboBox;

	CView *m_pViews[2];
	UINT m_nViewIndex;
	CSplitterWnd m_wndSplitter;
	int m_nHeight;
	BOOL m_bShowText;
	int m_nFixedHeight;
	int m_nClientHeight;

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateInfo(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePage(CCmdUI *pCmdUI);
	afx_msg LRESULT OnEnterSizeMove(WPARAM wParam, LPARAM lParam);
};
