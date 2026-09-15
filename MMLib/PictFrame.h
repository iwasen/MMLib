#pragma once

// PictFrame.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPictFrame フレーム

class CPictFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CPictFrame)

public:
	CComboBox &GetComboBox() {return m_wndComboBox;}

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	CMyToolBar m_wndPictToolBar;
	CComboBox m_wndComboBox;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
