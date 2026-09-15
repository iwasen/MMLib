#pragma once

// FindKeywordFrame.h : ヘッダー ファイル
//

#include "FindKeywordReBar.h"

/////////////////////////////////////////////////////////////////////////////
// CFindKeywordFrame フレーム

class CFindKeywordFrame : public CFrameWnd
{
protected:
	DECLARE_DYNCREATE(CFindKeywordFrame)

	CFindKeywordReBar m_wndReBar;
	CEdit m_wndKeyword;
	CButton m_wndFind;
	CMyToolBar m_wndToolBar;

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
